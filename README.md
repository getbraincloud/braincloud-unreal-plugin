# brainCloud Unreal Plugin
Copyright 2021 bitHeads, Inc. All Rights Reserved.

## Welcome to brainCloud!
Thanks for downloading the brainCloud Unreal plugin! Here are a few notes to get you started. Further information about the brainCloud API, including example Tutorials can be found here:

http://getbraincloud.com/apidocs/

If you haven't signed up or you want to log into the brainCloud portal, you can do that here:

https://portal.braincloudservers.com/

And join our community here:

https://forums.getbraincloud.com/

Package | Description
 ---- | ----
[**latest stable release**](https://github.com/getbraincloud/braincloud-unreal-plugin-src/releases/latest) | C++, Blueprint, and OnlineSubSystem library latest release for Unreal 4 and 5 (download zip)
[**braincloud-unreal-plugin-src**](https://github.com/getbraincloud/braincloud-unreal-plugin-src/) | brainCloud Unreal on GitHub (source)
[**examples-unreal**](https://github.com/getbraincloud/examples-unreal/) | Examples
[**Unreal - Older Versions**](http://getbraincloud.com/apidocs/tutorials/unreal-tutorials/building-the-plugin-for-older-versions-of-unreal/) | Instructions for building brainCloud for older versions of Unreal (for Blueprint only projects)

## Installation
1.  Open your Unreal Project's root folder (where the *.uproject file is)
2.  Open the Plugins folder, or create it if it does not exist
3.  From the downloaded release .zip file, copy the `braincloud-unreal-plugin-VersionNumber` folder into your projects Plugins folder, and rename it to `BCClient` then restart your Unreal Editor if it was open during this process.
4.  If your project has source code, be sure to regenerate your code project files
5.  Navigate to Edit -> Plugins
6.  Search for braincloud and enable plugin
7.  Restart Unreal Editor
8.  A dialog will pop up saying you're missing some modules, click yes to rebuild.
9.  Or, rebuild manually by deleting Intermediate folder, Binaries folder and .sln file 
then right click on your .uproject and generate visual studio files.
10. Restart Unreal Editor one more time. This ensures you will then be able to access the brainCloud Utility Widget which allows you to manage your projects brainCloud app settings (such as the app ID, secret, etc.)
11.  Installation complete!

Note: If you're using the plugin for RTT or Relay services and building to Mac OS platform, you will need to do the following steps to ensure the connection gets established. 
    1. Open up terminal
    2. Execute this command: rsync -a "{UE_INSTALL_PATH}/Engine/Content/Certificates/ThirdParty/cacert.pem" {PROJECT_PATH}/Content/Certificates/
    3. Open the project and go to Project Settings -> Packaging, look for "Advanced" near the bottom of Packaging section of the page and open it up.
    4. Add a element to "Additional Non-Asset Directories To Copy" and in the value field input "Certificates"

### Git Submodule Installation
BCClient plugin can be installed as a submodule of your git project.
Open a terminal at your project's root folder and enter the command:

```
$ git submodule add https://github.com/getbraincloud/braincloud-unreal-plugin-src.git Plugins/BCClient
```

Follow steps 4-10 above.

### Unreal Engine Version

The plugin will easily work with any version of UE4 and UE5. If switching, ensure that Intermediate files are cleaned up from previous build.

### Platforms

The plugin will work with all platforms. Make sure the required SDK, environment paths and licenses set up. To build for a specific platform only, edit the BCClient.uplugin file. Eg:

```
"SupportedTargetPlatforms": [
    "Win64",
    "Android",
    "Mac",
    "IOS",
    "Linux",
    "LinuxArm64"
],
```
## Example Projects

Examples of using brainCloud in your Unreal Projects can be found [here](https://github.com/getbraincloud/examples-unreal).

## Notes
To manipulate JSON in Blueprint another plugin is required.
We recommend VaRest, which you can find in Epic Games Marketplace or here: https://github.com/ufna/VaRest

When methods are deprecated and removed Unreal will throw exceptions when opening 
Blueprints that contain these functions.

Ensure you have removed these functions from your blueprints before updating to the new
version of the plugin.  If you experience issues, roll back to the previous version and
ensure you have not missed removal of these deprecated calls.

### Known Errors and Workarounds

Let us know any issues or suggestions in our [forums](https://forums.getbraincloud.com/).

Note there is a defect that UTCTimeZoneOffset will be off for one hour in case of daylight savings due to limitations of UE Core API regarding DateTime.

There is a known Clang compiler issue when using Xcode 14 with UE 4. There will be errors of redifintions of HTTP_STATUS codes in thirdparty libraries used by Unreal and brainCloud. This issue is resolved in UE 5. In order to continue development in previous versions of Unreal, the file  Engine/Source/ThirdParty/libWebSockets/libwebsockets/include/Mac/libwebsockets.h can be overwritten by the updated one downloaded from [Epic Games' developer github](https://github.com/EpicGames/UnrealEngine/blob/2cd6352a6928d0ba38138a72d1aa8d1fdd882a68/Engine/Source/ThirdParty/libWebSockets/libwebsockets/include/Mac/libwebsockets.h).

### Troubleshooting Server Connection

Here are a few common errors that you may see on your first attempt to connect to brainCloud.

- **App id not set**: Verify you've set up the app id and app secret correctly in the `initialize()` method. Check the server URL is correct including its case.
- **Platform not enabled**: Verify you've enabled your platform on the portal.

If you're still having issues, log into the portal and give us a shout through the help system (bottom right icon with the question mark and chat bubble).

## brainCloud Summary

brainCloud is a ready-made back-end platform for the development of feature-rich games, apps and things. brainCloud provides the features you need – along with comprehensive tools to support your team during development, testing and user support.

brainCloud consists of:
- Cloud Service – an advanced, Software-as-a-Service (SaaS) back-end
- Client Libraries – local client libraries (SDKs)
- Design Portal – a portal that allows you to design and debug your apps
- brainCloud Architecture

![architecture](/screenshots/bc-architecture.png?raw=true)

## What's the difference between the brainCloud Wrapper and the brainCloud Client?
The wrapper contains quality of life improvement around the brainCloud Client. It may contain device specific code, such as serializing the user's login id on an Android or iOS device.
It is recommended to use the wrapper by default.

![wrapper](/screenshots/bc-wrapper.png?raw=true)

## Hooking up Unreal project to brainCloud

***Using the brainCloud Utility Widget***

If you restarted the Unreal Editor a second time once the brainCloud plugins modules have been built and initialized, you will have the brainCloud Utility Widget at your disposal to edit the projects brainCloud app settings.

For Unreal Engine 4, this is located at the end of your main toolbar: 

![utility](/screenshots/_bc-UE4toolmenu.png?raw=true)

And for Unreal Engine 5, it is located in the Tools submenu under the brainCloud section: 

![utility](/screenshots/_bc-UE5toolmenu.png?raw=true)

When you click this button a new Editor widget window will open that looks like this. (You can also drag and dock it as a tab in any layout frame)

![utility](/screenshots/bcWidget.png?raw=true)

When you click update to save these settings, it is important to restart your Unreal Editor after for these changes to take effect.

***Manually***

If you do not want to use the brainCloud Utility Widget, you can simply edit the `BrainCloudSettings.ini` that is located in your projects Config folder, if it doesn't exist then you can create it.
Then you can fill in the values for your app settings:
```
[Credentials]
AppId=
AppSecret=
Version=
ServerUrl=
S2SKey=
S2SUrl=
```
You can then use these values in your project with the GetBCAppData blueprint function. Remember to restart your editor if you make any changes to this file.

## How do I initialize brainCloud?
![wrapper](/screenshots/_bp-initWrapper_BC5.png?raw=true)

1. Create a BrainCloudWrapper - This will store an instance of brainCloud and give you a way of accessing the services we provide.

2. Store and set the wrapper - You will want a variable of type BrainCloudWrapper.

3. Set the Default brainCloud instance to your wrapper - this allows Singleton functionality, and you won't need to pass the wrapper into each function call, unless needed.

4. Get the brainCloud app credentials using GetBCAppData

5. Initialize brainCloud with your app credentials 

Your Secret Key, and App Id, is set on the brainCloud dashboard. Under Design | Core App Info > Application IDs

![wrapper](/screenshots/bc-ids.png?raw=true)

Wrapper Name prefixes save operations that the wrapper will make. Use a Wrapper Name if you plan on having multiple instances of brainCloud running.

----------------

#### Newly upgraded?
If your app is already live, you should **NOT** specify the Wrapper Name - otherwise the library will look in the wrong location for your user's stored anonymousID and profileID information. Only add a name if you intend to alter the save data.

---------------

## For real-time networking in WebSocket with our Relay system
Make sure to modify or add the ThreadTargetFrameTimeInSeconds to increase the send rate of packets in your projects DefaultEngine.ini config file.
It would go under the [WebSockets.LibWebSockets] category, a lower value is a faster send rate, something like 0.001 or 0.0001 would be adequate for real-time.
Keep in mind that this could impact performance on the socket and CPU, for best results with real-time networking please use UDP protocol.
This is what it would look like in your `DefaultEngine.ini` file:
```
[WebSockets.LibWebSockets]
ThreadTargetFrameTimeInSeconds=0.001
```

Version is the current version of our app. Having an Version less than your minimum app version on brainCloud will prevent the user from accessing the service until they update their app to the lastest version you have provided them.

![Min Version](/screenshots/bc-minVersions.png?raw=true)

## How do I keep the brainCloud SDK updating?
In your project's update loop, you're going to want to update brainCloud client so it can check for responses.

To do this, you need to call Run Callbacks

![Min Version](/screenshots/_bp-runcallbacks.png?raw=true)

Alternatively, if you are using a GameInstance class in your project and wish to manage brainCloud in there, you can run callbacks on a timer.

![Min Version](/screenshots/_bc-runCallbacks_v5.png?raw=true)

## How do I authenticate a user with brainCloud?
The simplest form of authenticating with brainCloud is Anonymous Authentication.
![Authentication Anon](/screenshots/_bp-anonauth.png?raw=true)

## How do I attach an email to a user's brainCloud profile?
After having the user create an anonymous with brainCloud, they are probably going to want to attach an email or username, so their account can be accessed via another platform, or when their local data is discarded. Attaching email authenticate would look like this.

Attaching email authenticate would look like this.

![Authentication Anon](/screenshots/_bp-emailattach.png?raw=true)

There are many authentication types. You can also merge profiles and detach idenities. See the brainCloud documentation for more information:
http://getbraincloud.com/apidocs/apiref/?cpp#capi-auth

## Change Notes
Refer to our latest [release notes](https://getbraincloud.com/apidocs/releases/) for new brainCloud features and API changes.
