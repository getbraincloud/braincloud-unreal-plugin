#pragma once

class BCCLIENTPLUGIN_API ConvertUtilities
{
public:
    static FString BCBytesToString(const TArray<uint8>& in);
    static int32 BCStringToBytes(const FString &in_string, uint8 *out_bytes, int32 in_maxBufferSize);
    static TArray<uint8> BCStringToBytesArray(const FString& in_string);
    static FString BCBytesArrayToString(const TArray<uint8> in_array);
    static TArray<uint8> CompressBytes(const TArray<uint8>& UncompressedData, FName CompressionFormat = NAME_Gzip);
    static TArray<uint8> DecompressBytes(const TArray<uint8>& CompressedData, int32 UncompressedSize, FName CompressionFormat = NAME_Gzip);
};
