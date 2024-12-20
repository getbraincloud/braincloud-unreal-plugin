#include "ConvertUtilities.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include <Misc/Compression.h>

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
	FTCHARToUTF8 Converter(*in_string);
	const char* Utf8String = Converter.Get();
	int32 Utf8Length = Converter.Length();

	TArray<uint8> ByteArray;
	ByteArray.Append(reinterpret_cast<const uint8*>(Utf8String), Utf8Length);

	return ByteArray;
}

FString ConvertUtilities::BCBytesArrayToString(const TArray<uint8> in_array)
{
	return FUTF8ToTCHAR(reinterpret_cast<const ANSICHAR*>(in_array.GetData()), in_array.Num()).Get();
}

TArray<uint8> ConvertUtilities::CompressBytes(const TArray<uint8>& UncompressedData, FName CompressionFormat)
{
	TArray<uint8> CompressedData;

	if (UncompressedData.Num() > 0)
	{
		// Estimate the worst-case size for the compressed data
		int32 CompressedSize = FCompression::CompressMemoryBound(CompressionFormat, UncompressedData.Num());
		CompressedData.SetNumUninitialized(CompressedSize);

		// Compress the data
		if (FCompression::CompressMemory(CompressionFormat, CompressedData.GetData(), CompressedSize, UncompressedData.GetData(), UncompressedData.Num()))
		{
			// Resize the array to the actual compressed size
			CompressedData.SetNum(CompressedSize);
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

TArray<uint8> ConvertUtilities::DecompressBytes(const TArray<uint8>& CompressedData, int32 UncompressedSize, FName CompressionFormat)
{
	TArray<uint8> DecompressedData;
	DecompressedData.SetNumUninitialized(UncompressedSize);

	if (CompressedData.Num() > 0)
	{
		// Decompress the data
		if (!FCompression::UncompressMemory(CompressionFormat, DecompressedData.GetData(), UncompressedSize, CompressedData.GetData(), CompressedData.Num()))
		{
			// Handle decompression failure
			DecompressedData.Empty();
			UE_LOG(LogTemp, Error, TEXT("Failed to decompress data."));
		}
	}

	return DecompressedData;
}


