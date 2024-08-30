// Georgy Treshchev 2024.

#pragma once

#include "CoreMinimal.h"

#include "JavaConvert.h"

#include "Helpers/StaticNativeCaller.h"
#include "Helpers/SignatureHelper.h"
#include "CustomJavaTypes.h"

#include "AndroidNativeDefines.h"

/**
 * Helper for converting argument types from CPP to Java
 */
namespace ArgumentsConverter
{
	template <typename AnyType, TEnableIfNotCustomJavaArgument<AnyType> = true>
	AnyType ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const AnyType& Object) { return Object; }

	template <typename AnyType, TEnableIfCustomJavaArgument<AnyType> = true>
	jobject ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const AnyType& CustomObject) { return CustomObject.GetValue(); }

	jboolean ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const bool Bool) { return AndroidNative_JavaConverter::ToJavaBool(Bool); }
	jbooleanArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<bool>& BoolArray) { return AndroidNative_JavaConverter::ToJavaBoolArray(BoolArray); }

	jbyte ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const uint8 Byte) { return AndroidNative_JavaConverter::ToJavaByte(Byte); }
	jbyteArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<uint8>& ByteArray) { return AndroidNative_JavaConverter::ToJavaByteArray(ByteArray); }

	jchar ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const UTF16CHAR Char) { return AndroidNative_JavaConverter::ToJavaChar(Char); }
	jcharArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<UTF16CHAR>& CharArray) { return AndroidNative_JavaConverter::ToJavaCharArray(CharArray); }

	jshort ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const short Short) { return AndroidNative_JavaConverter::ToJavaShort(Short); }
	jshortArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<short>& ShortArray) { return AndroidNative_JavaConverter::ToJavaShortArray(ShortArray); }

	jint ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const int32 Int) { return AndroidNative_JavaConverter::ToJavaInt(Int); }
	jintArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<int32>& IntArray) { return AndroidNative_JavaConverter::ToJavaIntArray(IntArray); }

	jlong ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const long Long) { return AndroidNative_JavaConverter::ToJavaLong(Long); }
	jlongArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<long>& LongArray) { return AndroidNative_JavaConverter::ToJavaLongArray(LongArray); }

	jfloat ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const float Float) { return AndroidNative_JavaConverter::ToJavaFloat(Float); }
	jfloatArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<float>& FloatArray) { return AndroidNative_JavaConverter::ToJavaFloatArray(FloatArray); }

	jdouble ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const double Double) { return AndroidNative_JavaConverter::ToJavaDouble(Double); }
	jdoubleArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<double>& DoubleArray) { return AndroidNative_JavaConverter::ToJavaDoubleArray(DoubleArray); }

	jstring ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const FString& String) { return AndroidNative_JavaConverter::ToJavaString(String); }
	jstring ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TCHAR* String) { return AndroidNative_JavaConverter::ToJavaString(String); }
	jobjectArray ANDROIDNATIVE_API FORCEINLINE ConvertArgument(const TArray<FString>& StringArray) { return AndroidNative_JavaConverter::ToJavaStringArray(StringArray); }
}

class ANDROIDNATIVE_API AndroidNativeUtils
{
public:
	/**
	 * Main function for calling Java Static Methods
	 */
	template <typename ReturnType, typename... Args>
	static ReturnType CallJavaStaticMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, Args ... args)
	{
		const FString MethodSignature{ SignatureHelper::GetMethodSignature<ReturnType>(args...) };
		return StaticNativeCaller::CallJavaStaticMethod<ReturnType>(ClassName, MethodName, const_cast<ANSICHAR*>(StringCast<ANSICHAR>(*MethodSignature).Get()), ArgumentsConverter::ConvertArgument(args)...);
	}
};