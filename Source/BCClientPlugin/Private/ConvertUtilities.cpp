#include "ConvertUtilities.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include <Misc/Compression.h>
#include <string>
#include <zconf.h>
#include "zlib.h"
#include "Policies/CondensedJsonPrintPolicy.h"

FString ConvertUtilities::BCBytesToString(const TArray<uint8>& in)
{
	FUTF8ToTCHAR Converter(reinterpret_cast<const ANSICHAR*>(in.GetData()), in.Num());
	return FString(Converter.Length(), Converter.Get());
}

int32 ConvertUtilities::BCStringToBytes(const FString &in_string, uint8 *out_bytes, int32 in_maxBufferSize)
{
	int32 numBytes = 0;
	const TCHAR *charPos = *in_string;

	while (*charPos && numBytes < in_maxBufferSize)
	{
		out_bytes[numBytes] = (uint8)(*charPos);
		charPos++;
		++numBytes;
	}
	return numBytes;
}

TArray<uint8> ConvertUtilities::BCStringToBytesArray(const FString& in_string)
{
	std::string Utf8String = TCHAR_TO_UTF8(*in_string);

	// Convert std::string to TArray<uint8>
	TArray<uint8> Utf8Bytes;
	Utf8Bytes.Append(reinterpret_cast<const uint8*>(Utf8String.c_str()), Utf8String.length());

	return Utf8Bytes;
}

FString ConvertUtilities::BCBytesArrayToString(const TArray<uint8> in_array)
{
	// Ensure the byte array isn't empty
	if (in_array.Num() == 0)
	{
		return FString();
	}

	// Find the actual string length (up to the first null terminator)
	int32 length = 0;
	for (; length < in_array.Num(); ++length)
	{
		if (in_array[length] == 0)
		{
			break;
		}
	}

	// Convert only the portion of the byte array up to the null terminator
	return FUTF8ToTCHAR(reinterpret_cast<const ANSICHAR*>(in_array.GetData()), length).Get();
}

TArray<uint8> ConvertUtilities::CompressBytes(const TArray<uint8>& UncompressedData, bool enableLogging)
{
	TArray<uint8> CompressedData;

	if (UncompressedData.Num() > 0)
	{
		int32 CompressedSize = FCompression::CompressMemoryBound(NAME_Gzip, UncompressedData.Num());
		CompressedData.SetNumUninitialized(CompressedSize);

		// Compress the data
		if (FCompression::CompressMemory(NAME_Gzip, CompressedData.GetData(), CompressedSize, UncompressedData.GetData(), UncompressedData.Num(), COMPRESS_GZIP))
		{
			if(enableLogging)
				UE_LOG(LogTemp, Log, TEXT("Compress using GZIP successful"));
		}
		else
		{
			// Handle compression failure
			CompressedData.Empty();
			UE_LOG(LogTemp, Error, TEXT("Failed to compress data."));
		}
	}

	return CompressedData;
}

FString ConvertUtilities::MinifyJson(const FString& JsonString)
{
	// Parse JSON string into an object
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // Serialize it back into a compact JSON string
		FString CompactJsonString;
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = 
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&CompactJsonString);

		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

        return CompactJsonString;
    }

    // If parsing fails, return the original string
    return JsonString;
}


