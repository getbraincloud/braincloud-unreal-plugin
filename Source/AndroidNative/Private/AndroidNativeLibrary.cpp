// Georgy Treshchev 2024.

#include "AndroidNativeLibrary.h"
#include "AndroidNative.h"

#include "AndroidNativeUtils.h"

static const ANSICHAR* DeviceInfoClassName = "com/Plugins/AndroidNative/DeviceInfo";

void UAndroidNativeLibrary::EnableNsdService(int32 InPort)
{
	AndroidNativeUtils::CallJavaStaticMethod<void>(DeviceInfoClassName, "startNsdService", FAndroidGameActivity(), InPort);
}

bool UAndroidNativeLibrary::IsInternetAvailable()
{
	return AndroidNativeUtils::CallJavaStaticMethod<bool>(DeviceInfoClassName, "IsInternetAvailable");
}

FString UAndroidNativeLibrary::GetGeoLocation()
{
	return AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetGeoLocation", FAndroidGameActivity());
}

FBaseDeviceInfo UAndroidNativeLibrary::GetBaseDeviceInfo()
{
	
	FString UniqueID{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetUniqueID", FAndroidGameActivity())};
	FString OSVersion{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetOSVersion")};
	const int32 SDKVersion{AndroidNativeUtils::CallJavaStaticMethod<int32>(DeviceInfoClassName, "GetSDKVersion")};
	FString Brand{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetBrand")};
	FString Model{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetModel")};
	FString Product{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetProduct")};
	FString Language{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetLanguage")};
	FString LanguageCode{AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetLanguageCode")};

	FBaseDeviceInfo Result;
	{
		Result.UniqueID = MoveTemp(UniqueID);
		Result.OSVersion = MoveTemp(OSVersion);
		Result.SDKVersion = SDKVersion;
		Result.Brand = MoveTemp(Brand);
		Result.Model = MoveTemp(Model);
		Result.Product = MoveTemp(Product);
		Result.Language = MoveTemp(Language);
		Result.LanguageCode = MoveTemp(LanguageCode);
	}

	return Result;
}

FString UAndroidNativeLibrary::GetCountryCode()
{
	//First get country from SIM card
	FString countryCode = "";
	countryCode = AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetCountryCodeFromSIM", FAndroidGameActivity());
	
	if (countryCode == "") {
		//If SIM failed, get country from connected network
		countryCode = AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetCountryCodeFromNetwork", FAndroidGameActivity());
	}

	if (countryCode == "") {
		//If not connected to network, get country from locale as last resort (least reliable region accuracy)
		countryCode = AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetCountryCodeFromLocale", FAndroidGameActivity());
	}

	return countryCode;
}

FString UAndroidNativeLibrary::GetCountryCodeSIM()
{
	return AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetCountryCodeFromSIM", FAndroidGameActivity());
}

FString UAndroidNativeLibrary::GetCountryCodeNetwork()
{
	return AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetCountryCodeFromNetwork", FAndroidGameActivity());
}

FString UAndroidNativeLibrary::GetCountryCodeLocale()
{
	return AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetCountryCodeFromLocale", FAndroidGameActivity());
}


EAndroidTheme UAndroidNativeLibrary::GetCurrentSystemTheme()
{
	const uint8 Theme{AndroidNativeUtils::CallJavaStaticMethod<uint8>(DeviceInfoClassName, "GetCurrentSystemTheme", FAndroidGameActivity())};

	switch (Theme)
	{
	case 0: return EAndroidTheme::Undefined;
	case 1: return EAndroidTheme::Light;
	case 2: return EAndroidTheme::Dark;
	default: return EAndroidTheme::Undefined;
	}
}

FString UAndroidNativeLibrary::GetExternalPath()
{
	return AndroidNativeUtils::CallJavaStaticMethod<FString>(DeviceInfoClassName, "GetExternalPath", FAndroidGameActivity());
}
