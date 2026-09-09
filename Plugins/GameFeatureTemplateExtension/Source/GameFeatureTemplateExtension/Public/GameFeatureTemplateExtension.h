// Copyright https://github.com/SparkleBoom-Tutorials

#pragma once

#include "Modules/ModuleManager.h"

struct FGameFeaturePluginTemplateDescription;

/**
 * Editor module responsible for dynamically registering project-relative Game Feature templates
 * with Unreal Engine's PluginsEditor modular feature.
 */
class FGameFeatureTemplateExtensionModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/** IModuleInterface implementation */

private:
	/** Callbacks for IModularFeatures registration state changes. */
	void OnModularFeatureRegistered(const FName& Type, class IModularFeature* ModularFeature);
	void OnModularFeatureUnregistered(const FName& Type, class IModularFeature* ModularFeature);

	/** Resolves project-relative paths and registers templates into the Plugin Creation Wizard. */
	void RegisterGameFeatureTemplates();
	/** Unregisters all active custom templates from the Editor. */
	void UnregisterGameFeatureTemplates();
	/** Flushes current templates and re-registers them (used for hot-reloading on setting changes). */
	void ResetPluginTemplates();

	/** Triggered when UGameFeatureTemplateExtensionEditorSettings is modified in Project Settings. */
	void OnSettingsChanged(UObject* Settings, FPropertyChangedEvent& PropertyChangedEvent);

	/** Holds thread-safe references to registered template descriptions for clean lifecycle management. */
	TArray<TSharedPtr<FGameFeaturePluginTemplateDescription, ESPMode::ThreadSafe>> RegisteredTemplates;
};
