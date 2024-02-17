//---------------------------------------------------------------------------
#ifndef CEADataListClassH
#define CEADataListClassH
//---------------------------------------------------------------------------
#include "ListClass.h"
//---------------------------------------------------------------------------
enum
{
	CEA_INVALID,
	CEA_AUDIO,
	CEA_VIDEO,
	CEA_HDMI,
	CEA_HDMI2,
	CEA_HDMI21,
	CEA_FREESYNC,
	CEA_VENDOR_SPECIFIC_DATA,
	CEA_SPEAKER_ALLOCATION,
	CEA_VIDEO_CAPABILITY,
	CEA_VENDOR_SPECIFIC_VIDEO,
	CEA_HDR10_VIDEO,
	CEA_DOLBY_VIDEO,
	CEA_HDMI_VIDEO,
	CEA_COLORIMETRY,
	CEA_HDR_STATIC_METADATA,
	CEA_HDR_DYNAMIC_METADATA,
	CEA_VIDEO_FORMAT_PREFERENCE,
	CEA_YCC420_VIDEO,
	CEA_YCC420_CAPABILITY_MAP,
	CEA_VENDOR_SPECIFIC_AUDIO,
	CEA_DOLBY_AUDIO,
	CEA_HDMI_AUDIO,
	CEA_ROOM_CONFIGURATION,
	CEA_SPEAKER_LOCATION,
    CEA_INFOFRAME_DATA,
	CEA_DETAILED_RESOLUTION,
	CEA_TYPE_8_RESOLUTIONS,
	CEA_TYPE_10_RESOLUTIONS,
	CEA_EXTENSION_OVERRIDE,
	CEA_SINK_CAPABILITY,
	CEA_EXTENDED,
	CEA_OTHER,
};
//---------------------------------------------------------------------------
class CEADataListClass : public ListClass
{
private:
	static const char *SlotTypeText[];

public:
	CEADataListClass(int);
	bool Read(const unsigned char *, int);
	bool Write(unsigned char *, int);
	int GetSlotType(int);
	int GetSlotSize(int);
	bool UpdateSize();
	bool SetMaxCount(int);
	bool SetMaxSize(int);
	bool GetSlotTypeText(int, char *, int);
	bool GetSlotInfoText(int, char *, int);
	bool EditPossible(int);
	bool HDMISupported();
	bool HDMI2Supported();
	bool HDMI21Supported();
	bool AudioSupported();
	bool UnderscanSupported();
};
//---------------------------------------------------------------------------
#endif
