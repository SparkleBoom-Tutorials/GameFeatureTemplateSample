// Copyright https://github.com/SparkleBoom-Tutorials

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Misc/EngineVersionComparison.h"
#include "GameFeatureTemplateExtensionEditorSettings.generated.h"

class UGameFeatureData;

/**
 * Copy pasted from FPluginTemplateData inside UGameFeaturesEditorSettings
 */
USTRUCT()
struct FGameFeatureTemplateExtensionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PluginTemplate, meta = (RelativePath))
	FDirectoryPath Path;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Label;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Description;

	/** Optional sub folder that new plugins will be created in. */
	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FString DefaultSubfolder;

	/** Optional plugin name to default the new plugin to. */
	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FString DefaultPluginName;

	/** The default class of game feature data to create for new game feature plugins (if not set, UGameFeatureData will be used) */
	UPROPERTY(Config, EditAnywhere, Category = Plugins)
	TSubclassOf<UGameFeatureData> DefaultGameFeatureDataClass;

	/** The default name of the created game feature data assets. If empty, will use the plugin name. */
	UPROPERTY(Config, EditAnywhere, Category = Plugins)
	FString DefaultGameFeatureDataName;

	/** If true, the created plugin will be enabled by default without needing to be added to the project file. */
	UPROPERTY(Config, EditAnywhere, Category = Plugins)
	bool bIsEnabledByDefault = false;

	// Those options below works only from Unreal version 5.8
	
	/** Optional path to a post create python script that will run when new plugins are created from this template. */
	UPROPERTY(Config, EditAnywhere, Category = Plugins, meta = (RelativePath))
	FFilePath PostCreatePythonScriptPath;

	/** Optional post create python script arguments. */
	UPROPERTY(Config, EditAnywhere, Category = Plugins)
	FString PostCreatePythonScriptArguments;

};

/**
 * Custom Developer Settings for Game Feature Plugin Templates.
 *
 * Exposes a project-bound template list in the Editor's Project Settings, for bypassing
 * the Engine-relative path bug present in UGameFeaturesEditorSettings.
 */
UCLASS(MinimalAPI, Config = Editor, DefaultConfig, DisplayName="Game Feature Template Editor Settings")
class UGameFeatureTemplateExtensionEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** 
	 * Array of custom plugin templates configured for this project.
	 * Paths defined here will be dynamically converted to absolute paths relative to FPaths::ProjectDir().
	 */
	UPROPERTY(config, EditAnywhere, Category = Plugins)
	TArray<FGameFeatureTemplateExtensionData> PluginTemplates;
};
