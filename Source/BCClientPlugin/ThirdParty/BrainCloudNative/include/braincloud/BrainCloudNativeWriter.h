#pragma once

#include "BrainCloudNative.h"

#include <string>
#include <vector>

namespace braincloud {

/**
 * An app's id, name and secret, for writing a configuration.
 */
struct AppCredentials {
    std::string appId;
    std::string appName;
    std::string secret;
};

/**
 * An app's id and name, as read back from a configuration.
 */
struct AppName {
    std::string appId;
    std::string appName;
};

/**
 * A brainCloud portal session.
 */
struct PortalSession {
    /** The session's access token. */
    std::string accessToken;
    /** The account's email. */
    std::string email;
    /** The selected team id. */
    std::string teamId;
    /** The selected team's API key. */
    std::string apiKey;
    /** When the access token expires, in Unix seconds. */
    int64_t expiresAtUnix = 0;
};

/**
 * Writes app configurations and stores the editor's portal session.
 *
 * For editor tools only. Do not link this library into a game.
 */
class BCNATIVE_API ConfigWriter {
public:
    /**
     * Writes an app configuration, replacing the file. An app with an empty secret keeps
     * the secret already stored for its app id.
     *
     * @param path The file to write
     * @param app The app
     * @param childApps The app's child apps
     * @param serverUrl The server url
     * @param appVersion The app version
     * @return True if the file was written
     */
    static bool WriteConfig(const std::string& path, const AppCredentials& app,
                            const std::vector<AppCredentials>& childApps,
                            const std::string& serverUrl, const std::string& appVersion);

    /**
     * Updates the server url and app version, keeping the stored apps.
     *
     * @param path The file to update
     * @param serverUrl The server url
     * @param appVersion The app version
     * @return True if the file was updated
     */
    static bool WriteSettings(const std::string& path, const std::string& serverUrl,
                              const std::string& appVersion);

    /**
     * Reads the app and child app ids and names from a configuration.
     *
     * @param path The file to read
     * @param appOut The app
     * @param childAppsOut The child apps
     * @return True if the file was read
     */
    static bool ReadAppNames(const std::string& path, AppName& appOut,
                             std::vector<AppName>& childAppsOut);

    /**
     * Reads the secret of the app or one of its child apps, for an editor to show on request.
     *
     * @param path The file to read
     * @param appId The app id or a child app id
     * @param secretOut The app's secret
     * @return True if the app is in the configuration
     */
    static bool ReadSecret(const std::string& path, const std::string& appId, std::string& secretOut);

    /**
     * Stores plain text app credentials in a configuration.
     *
     * @param legacyIniPath The plain text credentials file
     * @param path The configuration file to write
     * @param serverUrl The server url
     * @param appVersion The app version
     * @param scrubPlaintext True to remove the app secret from the plain text file
     * @return True if the configuration was written
     */
    static bool ImportFromPlaintext(const std::string& legacyIniPath, const std::string& path,
                                    const std::string& serverUrl, const std::string& appVersion,
                                    bool scrubPlaintext);

    /**
     * Writes a brainCloud portal session to a file.
     *
     * @param path The file to write
     * @param session The session
     * @return True if the file was written
     */
    static bool SaveSession(const std::string& path, const PortalSession& session);

    /**
     * Reads a brainCloud portal session from a file.
     *
     * @param path The file to read
     * @param sessionOut The session
     * @return True if the file holds a session
     */
    static bool ResolveSession(const std::string& path, PortalSession& sessionOut);

    /**
     * Returns true if the session in a file has an access token that has not expired.
     * A session saved without an expiry is not active.
     *
     * @param path The file to read
     */
    static bool IsSessionActive(const std::string& path);

    /**
     * Removes the session from a file.
     *
     * @param path The file to update
     * @return True if the session was removed
     */
    static bool ClearSession(const std::string& path);
};

}
