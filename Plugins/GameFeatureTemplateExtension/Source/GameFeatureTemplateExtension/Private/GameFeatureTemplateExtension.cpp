// Copyright https://github.com/SparkleBoom-Tutorials

#include "GameFeatureTemplateExtension.h"
#include "Misc/EngineVersionComparison.h"
#include "GameFeaturePluginTemplate.h"
#include "GameFeatureTemplateExtensionEditorSettings.h"
#include "PluginDescriptor.h"
#include "Features/EditorFeatures.h"
#include "Features/IPluginsEditorFeature.h"

#define LOCTEXT_NAMESPACE "FGameFeatureProjectTemplateModule"

void FGameFeatureTemplateExtensionModule::StartupModule()
{
	IModularFeatures& ModularFeatures = IModularFeatures::Get();

	// Subscribe to modular feature events to handle timing safely if PluginsEditor loads later
	ModularFeatures.OnModularFeatureRegistered().AddRaw(this, &FGameFeatureTemplateExtensionModule::OnModularFeatureRegistered);
	ModularFeatures.OnModularFeatureUnregistered().AddRaw(this, &FGameFeatureTemplateExtensionModule::OnModularFeatureUnregistered);

	// Register templates immediately if PluginsEditor is already available
	if (ModularFeatures.IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
	{
		RegisterGameFeatureTemplates();
	}

	// Listen for live updates in Project Settings (Hot-Reloading)
	if (UGameFeatureTemplateExtensionEditorSettings* Settings = GetMutableDefault<UGameFeatureTemplateExtensionEditorSettings>())
	{
		Settings->OnSettingChanged().AddRaw(this, &FGameFeatureTemplateExtensionModule::OnSettingsChanged);
	}
}

void FGameFeatureTemplateExtensionModule::ShutdownModule()
{
	// Unbind settings listener
	if (UObjectInitialized())
	{
		if (UGameFeatureTemplateExtensionEditorSettings* Settings = GetMutableDefault<UGameFeatureTemplateExtensionEditorSettings>())
		{
			Settings->OnSettingChanged().RemoveAll(this);
		}
	}

	// Unbind modular features callbacks
	IModularFeatures& ModularFeatures = IModularFeatures::Get();
	ModularFeatures.OnModularFeatureRegistered().RemoveAll(this);
	ModularFeatures.OnModularFeatureUnregistered().RemoveAll(this);

	// Clean up registered templates from memory and editor registry
	UnregisterGameFeatureTemplates();
}

void FGameFeatureTemplateExtensionModule::OnModularFeatureRegistered(const FName& Type, IModularFeature* ModularFeature)
{
	if (Type == EditorFeatures::PluginsEditor)
	{
		RegisterGameFeatureTemplates();
	}
}

void FGameFeatureTemplateExtensionModule::OnModularFeatureUnregistered(const FName& Type, IModularFeature* ModularFeature)
{
	if (Type == EditorFeatures::PluginsEditor)
	{
		UnregisterGameFeatureTemplates();
	}
}

void FGameFeatureTemplateExtensionModule::RegisterGameFeatureTemplates()
{
	if (!IModularFeatures::Get().IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
	{
		return;
	}

	IPluginsEditorFeature& PluginEditor = IModularFeatures::Get().GetModularFeature<IPluginsEditorFeature>(EditorFeatures::PluginsEditor);

	if (const UGameFeatureTemplateExtensionEditorSettings* Settings = GetDefault<UGameFeatureTemplateExtensionEditorSettings>())
	{
		for (const FGameFeatureTemplateExtensionData& TemplateData : Settings->PluginTemplates)
		{
			// CORE FIX: Convert project-relative paths explicitly against FPaths::ProjectDir() 
			// to avoid engine fallback to FPlatformProcess::BaseDir()
			FString FixedPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), TemplateData.Path.Path);
			
#if UE_VERSION_NEWER_THAN_OR_EQUAL(5,8,0)
			// UE 5.8+ constructor overload including Python post-creation script support
			TSharedRef<FGameFeaturePluginTemplateDescription> FixedTemplate = MakeShareable(new FGameFeaturePluginTemplateDescription(
				TemplateData.Label,
				TemplateData.Description,
				FixedPath,
				TemplateData.DefaultSubfolder,
				TemplateData.DefaultPluginName,
				TemplateData.DefaultGameFeatureDataClass,
				TemplateData.DefaultGameFeatureDataName,
				TemplateData.bIsEnabledByDefault ? EPluginEnabledByDefault::Enabled : EPluginEnabledByDefault::Disabled,
				TemplateData.PostCreatePythonScriptPath.FilePath,
				TemplateData.PostCreatePythonScriptArguments
			));
#else
			// Legacy engine versions constructor overload
			TSharedRef<FGameFeaturePluginTemplateDescription> FixedTemplate = MakeShareable(new FGameFeaturePluginTemplateDescription(
				TemplateData.Label,
				TemplateData.Description,
				FixedPath,
				TemplateData.DefaultSubfolder,
				TemplateData.DefaultPluginName,
				TemplateData.DefaultGameFeatureDataClass,
				TemplateData.DefaultGameFeatureDataName,
				TemplateData.bIsEnabledByDefault ? EPluginEnabledByDefault::Enabled : EPluginEnabledByDefault::Disabled
			));
#endif

			RegisteredTemplates.Add(FixedTemplate);
			PluginEditor.RegisterPluginTemplate(FixedTemplate);
		}
	}
}

void FGameFeatureTemplateExtensionModule::UnregisterGameFeatureTemplates()
{
	if (IModularFeatures::Get().IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
	{
		IPluginsEditorFeature& PluginEditor = IModularFeatures::Get().GetModularFeature<IPluginsEditorFeature>(EditorFeatures::PluginsEditor);

		for (const auto& Template : RegisteredTemplates)
		{
			PluginEditor.UnregisterPluginTemplate(Template.ToSharedRef());
		}
	}
	RegisteredTemplates.Empty();
}

void FGameFeatureTemplateExtensionModule::ResetPluginTemplates()
{
	UnregisterGameFeatureTemplates();
	RegisterGameFeatureTemplates();
}

void FGameFeatureTemplateExtensionModule::OnSettingsChanged(UObject* Settings, FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	const FName MemberPropertyName = PropertyChangedEvent.MemberProperty != nullptr ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	const FName PluginTemplatePropertyName = GET_MEMBER_NAME_CHECKED(UGameFeatureTemplateExtensionEditorSettings, PluginTemplates);

	// Refresh registered templates live whenever the PluginTemplates array is edited
	if (PropertyName == PluginTemplatePropertyName || MemberPropertyName == PluginTemplatePropertyName)
	{
		ResetPluginTemplates();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameFeatureTemplateExtensionModule, GameFeatureTemplateExtension)
