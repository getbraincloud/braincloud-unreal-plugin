// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCConfigStore.h"

#include <string>

/**
 * An app to save in the app configuration. An empty AppSecret keeps the secret already
 * saved for the app.
 */
struct FBCEditorAppCredentials
{
    FString AppId;
    FString AppName;
    FString AppSecret;
};

/**
 * Writes the app configuration read by FBCConfigStore. Editor only.
 */
class BCWIDGET_API FBCEditorConfigStore
{
public:
    /**
     * Saves the app and its child apps, replacing the saved configuration.
     *
     * @param App The app
     * @param ChildApps The app's child apps
     * @param ServerUrl The url to the brainCloud server
     * @param OutError The reason the configuration could not be saved
     * @return True if the configuration was saved
     */
    static bool Save(const FBCEditorAppCredentials& App, const TArray<FBCEditorAppCredentials>& ChildApps,
        const FString& ServerUrl, FString& OutError);

    /**
     * Updates the url to the brainCloud server, keeping the saved apps.
     *
     * @param ServerUrl The url to the brainCloud server
     * @return True if the configuration was updated
     */
    static bool SaveServerUrl(const FString& ServerUrl);

    /**
     * Reads the saved app and child apps, with their names.
     *
     * @param Out The saved app
     * @return True if an app is saved
     */
    static bool Resolve(FBCStoredCredentials& Out);

    /**
     * Reads the saved secret of the app or one of its child apps, to show in the editor panel.
     *
     * @param AppId The app id or a child app id
     * @return The app's secret, or an empty string if the app is not saved
     */
    static FString ReadSecret(const FString& AppId);

    /**
     * Saves plain text credentials from Config/BrainCloudSettings.ini in the app configuration,
     * then removes the plain text app secrets.
     *
     * @param OutError The reason the credentials could not be saved
     * @return True if the credentials were saved
     */
    static bool MigrateLegacy(FString& OutError);

    /**
     * Removes the saved app configuration.
     */
    static void Clear();

    static std::string ToNative(const FString& Value);
    static FString FromNative(const std::string& Value);
};
