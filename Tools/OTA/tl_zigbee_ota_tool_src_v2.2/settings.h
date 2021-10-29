
/* OTA const */
#define	OTA_UPGRADE_FILEID					0x0BEEF11E
#define	OTA_HDR_VER							0x0100
#define OTA_HDR_FC							0x0000
#define ZB_STACK_VER						0x0002
#define OTA_HDR_STRING						"Telink OTA Sample Usage"

#define TAG_ID_UPGRADE_IMAGE				0x0000
#define TAG_ID_UPGRADE_IMAGE_AES			0xF000//manufacturer specific

#define SUB_ELEM_INFO_AES_ECB				0x01

#define FILE_NAME_ADD_SEC					"_security\0"

#define MANUFACTURER_CODD_GET(p)			((p[19] << 8) + p[18])
#define IMAGE_TYPE_GET(p)					((p[21] << 8) + p[20])
#define FILE_VERSION_GET(p)					((p[5] << 24) + (p[4] << 16) + (p[3] << 8) + p[2])
#define BIN_SIZE_GET(p)						((p[27] << 24) + (p[26] << 16) + (p[25] << 8) + p[24])



