//---------------------------------------------------------------------------
#include "Common.h"
#pragma hdrstop

#include "FreeSyncRangeClass.h"
//---------------------------------------------------------------------------
const int FreeSyncRangeClass::MinVRange = 1;
const int FreeSyncRangeClass::MaxVRange = 255;
//---------------------------------------------------------------------------
FreeSyncRangeClass::FreeSyncRangeClass()
{
	MinVRate = BLANK;
	MaxVRate = BLANK;
	Version[0] = 1;
	Version[1] = 1;
	OtherSize = 1;
	MaxOtherSize = sizeof OtherData;
	std::memset(OtherData, 0, MaxOtherSize);
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::Read(const unsigned char *Data, int MaxSize)
{
	int Type;
	int Size;
	int Index;

	if (!Data)
		return false;

	if (MaxSize < 9)
		return false;

	Type = Data[0] >> 5;
	Size = Data[0] & 31;

	if (Type != 3)
		return false;

	if (Size < 8)
		return false;

	if (Data[1] != 0x1A || Data[2] != 0x00 || Data[3] != 0x00)
		return false;

	Version[0] = Data[4];
	Version[1] = Data[5];

	if (Data[6] != 0)
		MinVRate = Data[6];
	else
		MinVRate = BLANK;

	if (Data[7] != 0)
		MaxVRate = Data[7];
	else
		MaxVRate = BLANK;

	Index = 8;
	OtherSize = Size - Index + 1;

	if (OtherSize < 0 || OtherSize > MaxOtherSize)
		OtherSize = 0;

	std::memset(OtherData, 0, MaxOtherSize);

	if (OtherSize > 0)
		std::memcpy(OtherData, &Data[Index], OtherSize);

	return true;
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::Write(unsigned char *Data, int MaxSize)
{
	int Size;

	if (!Data)
		return false;

	if (MaxSize < 9)
		return false;

	std::memset(Data, 0, MaxSize);
	Data[1] = 0x1A;
	Data[2] = 0x00;
	Data[3] = 0x00;
	Data[4] = Version[0];
	Data[5] = Version[1];

	if (MinVRate != BLANK)
		Data[6] = MinVRate;

	if (MaxVRate != BLANK)
		Data[7] = MaxVRate;

	Size = 7;

	if (OtherSize > 0)
	{
		std::memcpy(&Data[Size + 1], OtherData, OtherSize);
		Size += OtherSize;
	}

	Data[0] = 3 << 5;
	Data[0] |= Size;
	return true;
}
//---------------------------------------------------------------------------
int FreeSyncRangeClass::GetMinVRate()
{
	return MinVRate;
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::SetMinVRate(int Value)
{
	MinVRate = Value;
	return true;
}
//---------------------------------------------------------------------------
int FreeSyncRangeClass::GetMaxVRate()
{
	return MaxVRate;
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::SetMaxVRate(int Value)
{
	MaxVRate = Value;
	return true;
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::IsValid()
{
	return IsValidMinVRate() && IsValidMaxVRate();
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::IsValidMinVRate()
{
	if (MinVRate < MinVRange || MinVRate > MaxVRange)
		return false;

	if (MaxVRate < MinVRange || MaxVRate > MaxVRange)
		return true;

	return MinVRate <= MaxVRate;
}
//---------------------------------------------------------------------------
bool FreeSyncRangeClass::IsValidMaxVRate()
{
	if (MaxVRate < MinVRange || MaxVRate > MaxVRange)
		return false;

	if (MinVRate < MinVRange || MinVRate > MaxVRange)
		return true;

	return MaxVRate >= MinVRate;
}
//---------------------------------------------------------------------------
