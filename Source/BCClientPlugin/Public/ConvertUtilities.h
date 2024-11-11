#pragma once

class BCCLIENTPLUGIN_API ConvertUtilities
{
public:
    static FString BCBytesToString(const TArray<uint8>& in);
    static int32 BCStringToBytes(const FString &in_string, uint8 *out_bytes, int32 in_maxBufferSize);
};
