//---------------------------------------------------------------------------
#include "Common.h"
#pragma hdrstop

#include "ExtensionBlockClass.h"
//---------------------------------------------------------------------------
const int ExtensionBlockClass::MinType = 0;
const int ExtensionBlockClass::MaxType = 4;

const char *ExtensionBlockClass::TypeText[] =
{
	"CTA-861",
	"VTB-EXT",
	"DisplayID 1.3",
	"DisplayID 2.0",
	"Default",
};

unsigned char *ExtensionBlockClass::CopyExtensionData = NULL;
//---------------------------------------------------------------------------
ExtensionBlockClass::ExtensionBlockClass()
{
	OldExtensionData = new unsigned char[128];
	std::memset(OldExtensionData, 0, 128);
	std::memcpy(OldExtensionData, "\x02\x03\x04", 3);

	NewExtensionData = new unsigned char[128];
	std::memcpy(NewExtensionData, OldExtensionData, 128);

	ExtensionType = 0;

	ColorFormatList = new ColorFormatListClass;
	DetailedResolutionList = new DetailedResolutionListClass(6);
	CEADataList = new CEADataListClass(123);
	StandardResolutionList = new StandardResolutionListClass(61);
	DIDDataList = new DIDDataListClass(40);
}
//---------------------------------------------------------------------------
void ExtensionBlockClass::Copy(const ExtensionBlockClass &Source)
{
	OldExtensionData = new unsigned char[128];
	std::memcpy(OldExtensionData, Source.OldExtensionData, 128);

	NewExtensionData = new unsigned char[128];
	std::memcpy(NewExtensionData, Source.NewExtensionData, 128);

	ExtensionType = Source.ExtensionType;

	ColorFormatList = new ColorFormatListClass(*Source.ColorFormatList);
	DetailedResolutionList = new DetailedResolutionListClass(*Source.DetailedResolutionList);
	CEADataList = new CEADataListClass(*Source.CEADataList);
	StandardResolutionList = new StandardResolutionListClass(*Source.StandardResolutionList);
	DIDDataList = new DIDDataListClass(*Source.DIDDataList);
}
//---------------------------------------------------------------------------
void ExtensionBlockClass::Delete()
{
	delete[] OldExtensionData;
	delete[] NewExtensionData;

	delete ColorFormatList;
	delete DetailedResolutionList;
	delete CEADataList;
	delete StandardResolutionList;
	delete DIDDataList;
}
//---------------------------------------------------------------------------
ExtensionBlockClass::ExtensionBlockClass(const ExtensionBlockClass &Source)
{
	Copy(Source);
}
//---------------------------------------------------------------------------
ExtensionBlockClass &ExtensionBlockClass::operator=(const ExtensionBlockClass &Source)
{
	Delete();
	Copy(Source);
	return *this;
}
//---------------------------------------------------------------------------
ExtensionBlockClass::~ExtensionBlockClass()
{
	Delete();
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::Read(const unsigned char *Data, int MaxSize)
{
	if (!Data)
		return false;

	if (MaxSize < 128)
		return false;

	std::memcpy(NewExtensionData, Data, 128);
	ExtensionInit();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::Write(unsigned char *Data, int MaxSize)
{
	if (!Data)
		return false;

	if (MaxSize < 128)
		return false;

	ExtensionWrite();
	std::memcpy(Data, NewExtensionData, 128);
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::GetText(char *Text, int TextSize)
{
	int Count1;
	int Count2;
	const char *HDMISupported;

	switch (NewExtensionData[0])
	{
		case 0x00:
			std::snprintf(Text, TextSize, "Default extension block");
			break;

		case 0x02:
			Count1 = DetailedResolutionList->GetCount();
			Count2 = CEADataList->GetCount();
			HDMISupported = "";

			if (CEADataList->HDMISupported())
			{
				if (CEADataList->HDMI21Supported())
					HDMISupported = " (HDMI 2.1)";
				else if (CEADataList->HDMI2Supported())
					HDMISupported = " (HDMI 2.0)";
				else
					HDMISupported = " (HDMI)";
			}

			std::snprintf(Text, TextSize, "%s: %d detailed resolution%s, %d data block%s%s", TypeText[ExtensionType], Count1, Count1 != 1 ? "s" : "", Count2, Count2 != 1 ? "s" : "", HDMISupported);
			break;

		case 0x10:
			Count1 = DetailedResolutionList->GetCount();
			Count2 = StandardResolutionList->GetCount();
			std::snprintf(Text, TextSize, "%s: %d detailed resolution%s, %d standard resolution%s", TypeText[ExtensionType], Count1, Count1 != 1 ? "s" : "", Count2, Count2 != 1 ? "s" : "");
			break;

		case 0x20:
			std::snprintf(Text, TextSize, "EDID 2.0 extension block");
			break;

		case 0x40:
			std::snprintf(Text, TextSize, "DI-EXT: Display Information");
			break;

		case 0x50:
			std::snprintf(Text, TextSize, "LS-EXT: Localized String");
			break;

		case 0x60:
			std::snprintf(Text, TextSize, "DPVL-EXT: Digital Packet Video Link");
			break;

		case 0x70:
			Count1 = DIDDataList->GetCount();
			std::snprintf(Text, TextSize, "%s: %d data block%s", TypeText[ExtensionType], Count1, Count1 != 1 ? "s" : "");
			break;

		case 0xF0:
			std::snprintf(Text, TextSize, "Extension block map");
			break;

		default:
			std::snprintf(Text, TextSize, "Other extension block (%d)", NewExtensionData[0]);
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::GetTypeText(int Type, char *Text, int TextSize)
{
	if (Type < MinType || Type > MaxType)
		return false;

	std::snprintf(Text, TextSize, "%s", TypeText[Type]);
	return true;
}
//---------------------------------------------------------------------------
int ExtensionBlockClass::GetType()
{
	if (!IsValidType())
		return -1;

	return ExtensionType;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::SetType(int Value)
{
	ExtensionType = Value;

	switch (ExtensionType)
	{
		case CEA_EXT:
			if (ExtensionGetBytesLeft() < 0)
				CEADataList->DeleteAll();

			break;

		case VTB_EXT:
			if (ExtensionGetBytesLeft() < 0)
				StandardResolutionList->DeleteAll();

			break;

		case DISPLAYID_EXT:
			DIDDataList->SetVersion(1);
			break;

		case DISPLAYID2_EXT:
			DIDDataList->SetVersion(2);
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ReadType()
{
	switch (NewExtensionData[0])
	{
		case 0x00:
			ExtensionType = DEFAULT_EXT;
			break;

		case 0x02:
			ExtensionType = CEA_EXT;
			break;

		case 0x10:
			ExtensionType = VTB_EXT;
			break;

		case 0x70:
			if (NewExtensionData[1] < 0x20)
			{
				ExtensionType = DISPLAYID_EXT;
				DIDDataList->SetVersion(1);
			}
			else
			{
				ExtensionType = DISPLAYID2_EXT;
				DIDDataList->SetVersion(2);
			}

			break;

		default:
			ExtensionType = OTHER_EXT;
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionInit()
{
	ReadType();

	switch (ExtensionType)
	{
		case CEA_EXT:
			DetailedInit();
			CEADataInit();

			if (CEADataList->HDMISupported())
				ColorFormatsInit();

			break;

		case VTB_EXT:
			DetailedInit();
			StandardInit();
			break;

		case DISPLAYID_EXT:
		case DISPLAYID2_EXT:
			DIDDataInit();
			break;
	}

	ExtensionWrite();
	std::memcpy(OldExtensionData, NewExtensionData, 128);
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionRead()
{
	ReadType();

	switch (ExtensionType)
	{
		case CEA_EXT:
			DetailedRead();
			CEADataRead();

			if (CEADataList->HDMISupported())
				ColorFormatsRead();

			break;

		case VTB_EXT:
			DetailedRead();
			StandardRead();
			break;

		case DISPLAYID_EXT:
		case DISPLAYID2_EXT:
			DIDDataRead();
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionWrite()
{
	if (ExtensionType == OTHER_EXT)
		return true;

	std::memset(NewExtensionData, 0, 128);

	switch (ExtensionType)
	{
		case CEA_EXT:
			NewExtensionData[0] = 0x02;
			NewExtensionData[1] = 0x03;
			DetailedWrite();
			CEADataWrite();

			if (CEADataList->HDMISupported())
				ColorFormatsWrite();

			if (CEADataList->AudioSupported())
				NewExtensionData[3] |= 64;

			if (CEADataList->UnderscanSupported())
				NewExtensionData[3] |= 128;

			break;

		case VTB_EXT:
			NewExtensionData[0] = 0x10;
			NewExtensionData[1] = 0x01;
			DetailedWrite();
			StandardWrite();
			break;

		case DISPLAYID_EXT:
			NewExtensionData[0] = 0x70;
			NewExtensionData[1] = 0x12;
			DIDDataWrite();
			break;

		case DISPLAYID2_EXT:
			NewExtensionData[0] = 0x70;
			NewExtensionData[1] = 0x20;
			DIDDataWrite();
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionCopyPossible()
{
	if (!CopyExtensionData)
		return true;

	ExtensionWrite();

	if (std::memcmp(CopyExtensionData, NewExtensionData, 128) != 0)
		return true;

	return false;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionCopy()
{
	if (!CopyExtensionData)
		CopyExtensionData = new unsigned char[128];

	ExtensionWrite();
	std::memcpy(CopyExtensionData, NewExtensionData, 128);
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionPastePossible()
{
	if (!CopyExtensionData)
		return false;

	ExtensionWrite();

	if (std::memcmp(NewExtensionData, CopyExtensionData, 128) != 0)
		return true;

	return false;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionPaste()
{
	if (!CopyExtensionData)
		return false;

	std::memcpy(NewExtensionData, CopyExtensionData, 128);
	ExtensionRead();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionResetPossible()
{
	ExtensionWrite();

	if (std::memcmp(NewExtensionData, OldExtensionData, 128) != 0)
		return true;

	return false;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ExtensionReset()
{
	std::memcpy(NewExtensionData, OldExtensionData, 128);
	ExtensionRead();
	return true;
}
//---------------------------------------------------------------------------
int ExtensionBlockClass::ExtensionGetBytesLeft()
{
	int Bytes;

	switch (ExtensionType)
	{
		case CEA_EXT:
			Bytes = 123;
			Bytes -= DetailedResolutionList->GetSize();
			Bytes -= CEADataList->GetSize();
			break;

		case VTB_EXT:
			Bytes = 122;
			Bytes -= DetailedResolutionList->GetSize();
			Bytes -= StandardResolutionList->GetSize();
			break;

		case DISPLAYID_EXT:
		case DISPLAYID2_EXT:
			Bytes = 121;
			Bytes -= DIDDataList->GetSize();
			break;
	}

	return Bytes;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ColorFormatsInit()
{
	ColorFormatsRead();
	ColorFormatList->UpdateUndo();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ColorFormatsRead()
{
	if (!ColorFormatList->Read(&NewExtensionData[3]))
		return false;

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::ColorFormatsWrite()
{
	if (!ColorFormatList->Write(&NewExtensionData[3]))
		return false;

	return true;
}
//---------------------------------------------------------------------------
ColorFormatListClass *ExtensionBlockClass::ColorFormats()
{
	return ColorFormatList;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DetailedInit()
{
	DetailedRead();
	DetailedResolutionList->UpdateUndo();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DetailedRead()
{
	int Offset;
	int Size;

	switch (ExtensionType)
	{
		case CEA_EXT:
			Offset = NewExtensionData[2];

			if (Offset < 4)
				return true;

			if (Offset > 127)
				Offset = 127;

			Size = 127 - Offset;

			if (!DetailedResolutionList->Read(&NewExtensionData[Offset], Size))
				return false;

			break;

		case VTB_EXT:
			Offset = 5;
			Size = NewExtensionData[2] * 18;

			if (Size > 127 - Offset)
				Size = 127 - Offset;

			if (!DetailedResolutionList->Read(&NewExtensionData[Offset], Size))
				return false;

			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DetailedWrite()
{
	int Offset;
	int Size;

	switch (ExtensionType)
	{
		case CEA_EXT:
			Offset = 4 + CEADataList->GetSize();

			if (Offset > 127)
				Offset = 127;

			Size = DetailedResolutionList->GetSize();

			if (Size > 127 - Offset)
				Size = 127 - Offset;

			if (!DetailedResolutionList->Write(&NewExtensionData[Offset], Size))
				return false;

			break;

		case VTB_EXT:
			Offset = 5;
			Size = DetailedResolutionList->GetSize();

			if (Size > 127 - Offset)
				Size = 127 - Offset;

			if (!DetailedResolutionList->Write(&NewExtensionData[Offset], Size))
				return false;

			NewExtensionData[2] = DetailedResolutionList->GetCount();
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DetailedGetExtensionText(int Slot, char *Text, int TextSize)
{
	switch (ExtensionType)
	{
		case CEA_EXT:
			return DetailedResolutionList->GetExtensionText(Slot, Text, TextSize, CEADataList->GetBytesLeft());

		case VTB_EXT:
			return DetailedResolutionList->GetExtensionStandardText(Slot, Text, TextSize, StandardResolutionList->GetSlotsLeft());
	}

	return false;
}
//---------------------------------------------------------------------------
DetailedResolutionListClass *ExtensionBlockClass::DetailedResolutions()
{
	switch (ExtensionType)
	{
		case CEA_EXT:
			DetailedResolutionList->SetMaxSize(123 - CEADataList->GetSize());
			break;

		case VTB_EXT:
			DetailedResolutionList->SetMaxSize(122 - StandardResolutionList->GetSize());
			break;
	}

	return DetailedResolutionList;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::CEADataInit()
{
	CEADataRead();
	CEADataList->UpdateUndo();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::CEADataRead()
{
	int Size;

	if (ExtensionType == CEA_EXT)
	{
		Size = NewExtensionData[2] - 4;

		if (Size < 0)
			return true;

		if (Size > 123)
			Size = 123;

		if (!CEADataList->Read(&NewExtensionData[4], Size))
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::CEADataWrite()
{
	int Size;

	if (ExtensionType == CEA_EXT)
	{
		Size = CEADataList->GetSize();

		if (Size > 123)
			Size = 123;

		if (!CEADataList->Write(&NewExtensionData[4], Size))
			return false;

		NewExtensionData[2] = Size + 4;
	}

	return true;
}
//---------------------------------------------------------------------------
CEADataListClass *ExtensionBlockClass::CEAData()
{
	if (ExtensionType == CEA_EXT)
		CEADataList->SetMaxSize(123 - DetailedResolutionList->GetSize());

	return CEADataList;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::StandardInit()
{
	StandardRead();
	StandardResolutionList->UpdateUndo();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::StandardRead()
{
	int Offset;
	int Size;

	if (ExtensionType == VTB_EXT)
	{
		Offset = 5 + NewExtensionData[2] * 18 + NewExtensionData[3] * 3;

		if (Offset > 127)
			Offset = 127;

		Size = NewExtensionData[4] * 2;

		if (Size > 127 - Offset)
			Size = 127 - Offset;

		if (!StandardResolutionList->ReadExtension(&NewExtensionData[Offset], Size))
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::StandardWrite()
{
	int Offset;
	int Size;

	if (ExtensionType == VTB_EXT)
	{
		Offset = 5 + DetailedResolutionList->GetSize();

		if (Offset > 127)
			Offset = 127;

		Size = StandardResolutionList->GetSize();

		if (Size > 127 - Offset)
			Size = 127 - Offset;

		if (!StandardResolutionList->WriteExtension(&NewExtensionData[Offset], Size))
			return false;

		NewExtensionData[4] = StandardResolutionList->GetCount();
	}

	return true;
}
//---------------------------------------------------------------------------
StandardResolutionListClass *ExtensionBlockClass::StandardResolutions()
{
	if (ExtensionType == VTB_EXT)
		StandardResolutionList->SetMaxSize(122 - DetailedResolutionList->GetSize());

	return StandardResolutionList;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DIDDataInit()
{
	DIDDataRead();
	DIDDataList->UpdateUndo();
	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DIDDataRead()
{
	int Size;

	if (ExtensionType == DISPLAYID_EXT || ExtensionType == DISPLAYID2_EXT)
	{
		Size = NewExtensionData[2];

		if (Size > 121)
			Size = 121;

		if (!DIDDataList->Read(&NewExtensionData[5], Size))
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DIDDataWrite()
{
	int Size;

	if (ExtensionType == DISPLAYID_EXT || ExtensionType == DISPLAYID2_EXT)
	{
		Size = DIDDataList->GetSize();

		if (Size != 121)
			Size = 121;

		if (!DIDDataList->Write(&NewExtensionData[5], Size))
			return false;

		NewExtensionData[2] = Size;
		DIDDataChecksum(Size + 5);
	}

	return true;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::DIDDataChecksum(int Size)
{
	int Index;

	NewExtensionData[Size] = 0;

	for (Index = 1; Index < Size; Index++)
		NewExtensionData[Size] -= NewExtensionData[Index];

	return true;
}
//---------------------------------------------------------------------------
DIDDataListClass *ExtensionBlockClass::DIDData()
{
	if (ExtensionType == DISPLAYID_EXT || ExtensionType == DISPLAYID2_EXT)
		DIDDataList->SetMaxSize(121);

	return DIDDataList;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::IsValidType()
{
	return ExtensionType >= MinType && ExtensionType <= MaxType;
}
//---------------------------------------------------------------------------
bool ExtensionBlockClass::IsSupported()
{
	if (ExtensionType == DEFAULT_EXT)
		return false;

	return IsValidType();
}
//---------------------------------------------------------------------------
