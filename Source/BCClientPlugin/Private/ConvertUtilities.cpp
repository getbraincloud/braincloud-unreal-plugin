#include "ConvertUtilities.h"

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
