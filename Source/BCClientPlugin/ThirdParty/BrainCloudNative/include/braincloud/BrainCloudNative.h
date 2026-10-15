#pragma once

#include <cstdint>
#include <string>
#include <vector>

#if defined(_WIN32) && defined(BCNATIVE_SHARED)
#if defined(BCNATIVE_BUILDING)
#define BCNATIVE_API __declspec(dllexport)
#else
#define BCNATIVE_API __declspec(dllimport)
#endif
#elif defined(BCNATIVE_SHARED)
#define BCNATIVE_API __attribute__((visibility("default")))
#else
#define BCNATIVE_API
#endif

namespace braincloud {

/**
 * The state of the stored app configuration.
 */
enum class ConfigState {
    /** No configuration is stored. */
    None,
    /** Credentials are stored in plain text. */
    Legacy,
    /** A configuration is stored and can be read. */
    Ready,
    /** A configuration is stored but cannot be read. */
    Invalid,
};

/**
 * Reads an app configuration and signs requests with it.
 */
class BCNATIVE_API NativeConfig {
public:
    NativeConfig();
    ~NativeConfig();

    NativeConfig(const NativeConfig&) = delete;
    NativeConfig& operator=(const NativeConfig&) = delete;

    /**
     * Reads the configuration from a file.
     *
     * @param path The file to read
     * @return True if the configuration was read
     */
    bool Resolve(const std::string& path);

    /**
     * Reads the configuration from the contents of a file.
     *
     * @param contents The file's contents
     * @return True if the configuration was read
     */
    bool ResolveContents(const std::string& contents);

    /**
     * Returns true if Resolve or ResolveContents succeeded.
     */
    bool IsResolved() const;

    /**
     * Returns the app id, or an empty string if the configuration has not been read.
     */
    const std::string& GetAppId() const;

    /**
     * Returns the ids of the child apps in the configuration.
     */
    const std::vector<std::string>& GetChildAppIds() const;

    /**
     * Returns the server url, or an empty string if none is stored.
     */
    const std::string& GetServerUrl() const;

    /**
     * Returns the app version, or an empty string if none is stored.
     */
    const std::string& GetAppVersion() const;

    /**
     * Returns true if the configuration can sign for an app.
     *
     * @param appId The app id or a child app id
     */
    bool HasApp(const std::string& appId) const;

    /**
     * Signs a request payload for the app.
     *
     * @param payload The request payload
     * @return The signature, or an empty string if the configuration has not been read
     */
    std::string Sign(const std::vector<uint8_t>& payload) const;
    std::string Sign(const std::string& payload) const;

    /**
     * Signs a request payload for the app or one of its child apps.
     *
     * @param appId The app id or a child app id
     * @param payload The request payload
     * @return The signature, or an empty string if the app is not in the configuration
     */
    std::string Sign(const std::string& appId, const std::vector<uint8_t>& payload) const;

    /**
     * Returns the state of the stored configuration.
     *
     * @param configPath The configuration file, or empty to skip it
     * @param legacyIniPath The plain text credentials file, or empty to skip it
     */
    static ConfigState DetectState(const std::string& configPath,
                                   const std::string& legacyIniPath);

    /**
     * Returns this library's version.
     */
    static const char* Version();

private:
    struct Impl;
    Impl* _impl;
};

}
