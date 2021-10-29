#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "settings.h"


#pragma pack (1)
typedef struct ota_hdrFields
{
	/* The value is a unique 4-byte value that is included at the beginning of all ZigBee OTA upgrade image
	files in order to quickly identify and distinguish the file as being a ZigBee OTA cluster upgrade file,
	without having to examine the whole file content. This helps distinguishing the file from other file
	types on disk. The value is defined to be "0x0BEEF11E". */
	u32	otaUpgradeFileID;

	/* The current OTA header version shall be 0x0100 with major version of "01" and minor version of "00". */
	u16	otaHdrVer;

	/* This value indicates full length of the OTA header in bytes, including the OTA upgrade file identifier,
	OTA header length itself to any optional fields.*/
	u16	otaHdrLen;
	
	/* The bit mask indicates whether additional information such as Image Signature or Signing Certificate
	are included as part of the OTA Upgrade Image. */
	u16	otaHdrFC;

	/* This is the zigbee assigned identify for each member company. */
	u16	manufacturerCode;

	/* The manufacturer should assign an appropriate and unique image type value to each of its devices in
	order to distinguish the products. */
	u16	imageType;

	/* For firmware image. */
	u32	fileVer;
	
	/* This information indicates the zigbee stack version that is used by application. */
	u16	zbStackVer;
	
	/* This is a manufacturer specific string. */
	u8	otaHdrString[32];
	
	/* The value represents the total image size in bytes. */
	u32	totalImageSize;

	/* The following fields are optional. */
	u8	securityCredVer;
	u8	fileDest[8];
	u16	minHdrVer;
	u16	maxHdrVer;
}ota_hdrFields_t;
#pragma pack ()

#pragma pack (1)
typedef struct ota_subElement
{
	/* The tag identifier denotes the type and format of the data contained with the sub-element. */
	u16	tagID;
	
	/* This value decates the length of the rest of the data within the sub-element in bytes. */
	u32	lengthField;
	
	/* Manufacturer specific. */
	u8	info;
	u8	fillNum;
}ota_subElement_t;
#pragma pack ()



ota_hdrFields_t otaHdr =
{
	.otaUpgradeFileID 	= OTA_UPGRADE_FILEID,
	.otaHdrVer 			= OTA_HDR_VER,
	.zbStackVer 		= ZB_STACK_VER,
	.otaHdrString 		= OTA_HDR_STRING,
	.otaHdrFC 			= OTA_HDR_FC,
};

ota_subElement_t otaImageSector =
{
	.tagID 				= TAG_ID_UPGRADE_IMAGE,
};


static const char* usage[] =
{
	"*******************************************************",
	"Usage:  zigbee_ota_tool [arguments]",
	"REQUIRED ARGUMENTS:",
	"[argument 1]",
	"-- file name, the nane of the file to be converted.",
	"   e.g. sampleLight_8258.bin",
	"[argument 2]",
	"-- 16-byte AES key, if parameter 2 is present,",
	"   the firmware will use AES-128 ECB encryption.",
	"   e.g. 83FED3407A939723A5C639B26916D505",
	"*******************************************************",
	NULL,
};

static void printUsage(void)
{
	u8 i = 0;
	while(usage[i] != NULL){
		printf("%s\n", usage[i]);
		i++;
	}
}

static u8 char2Byte(u8 c)
{
	if(c >= 'A' && c <= 'F'){
		return c - 'A' + 0x0A;
	}else if (c >= 'a' && c <= 'f'){
		return c - 'a' + 0x0A;
	}else if (c >= '0' && c <= '9'){
		return c - '0';
	}
	return 0xFF;
}

FILE *openFWFile(u8 fileName[], u8 strBuf[])
{
	u8 i = 0;
	u8 bootFlgOffset = 0;
	
	FILE *fBin = fopen(fileName, "rb");
	if(!fBin){
		printf("File name not found!\n");
		return NULL;
	}
	
	u8 len = fread(strBuf, 1, 64, fBin);
	
	if(len != 64){
		printf("File length is less than 64 bytes!\n");
		fclose(fBin);
		return NULL;
	}else{
		for(i = 0; i < 64; i++){
			/* match boot flag */
			if((strBuf[i] == 'K') && (strBuf[i + 1] == 'N') && (strBuf[i + 2] == 'L') && (strBuf[i + 3] == 'T')){
				bootFlgOffset = i;
				break;
			}
		}
	}
	
	if(bootFlgOffset == 0){
		printf("File not allowed!\n");
		fclose(fBin);
		return NULL;
	}
	
	return fBin;
}

FILE *creatOTAFile(u8 fileName[], u8 strBuf[])
{
	printf("\r\n");
	printf("************** Current bin info **************\n");
	
	u16 manuCode = MANUFACTURER_CODD_GET(strBuf);
	printf("Manufacturer Code is (hex): %04x\n", manuCode);
	
	u16 imageType = IMAGE_TYPE_GET(strBuf);
	printf("Image type is (hex): %04x\n", imageType);
	
	u32 fileVer = FILE_VERSION_GET(strBuf);
	printf("File version is (hex): %08x\n", fileVer);
	
	u32 binSize = BIN_SIZE_GET(strBuf);
	printf("Bin size is (dec): %d\n", binSize);

	printf("**********************************************\n");
	printf("\r\n");
	
	u8 newFileName[128] = {0};
	sprintf(newFileName, "%04x-%04x-%08x-%s.zigbee", manuCode, imageType, fileVer, fileName);
	printf("%s\n", newFileName);
	
	FILE *newBin = fopen(newFileName, "wb");
	if(!newBin){
		printf("Creat failure!\n");
		return NULL;
	}
	
	/* Build ota header fields. */
	otaHdr.manufacturerCode = manuCode;
	otaHdr.imageType = imageType;
	otaHdr.fileVer = fileVer;
	otaHdr.otaHdrLen = sizeof(otaHdr);
	if(!(otaHdr.otaHdrFC & 1)){
		otaHdr.otaHdrLen -= 1;//no security credential ver, 1 byte.
	}
	if(!(otaHdr.otaHdrFC & 2)){
		otaHdr.otaHdrLen -= 8;//no uprade file destination, 8 bytes.
	}
	if(!(otaHdr.otaHdrFC & 4)){
		otaHdr.otaHdrLen -= 4;//no min and max hardware ver, 4bytes.
	}
	
	return newBin;
}

int getAESKey(u8 strBuf[], u8 key[16])
{
	u8 idx = 0;
	u8 i = 0;
	
	for(i = 0; i < 32; i += 2){
		u8 a = char2Byte(strBuf[i]);
		u8 b = char2Byte(strBuf[i + 1]);
		
		if(a != 0xFF && b != 0xFF){
			key[idx++] = (a << 4) | b;
		}else{
			printf("Invaild hex input!\r\n");
			return -1;
		}
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	u8 fileName[128] = {0};
	u8 aesKeyStr[64] = {0};
	u8 aesKey[16] = {0};
	u8 strBuf[64] = {0};
	u8 aesEncrypt = 0;
	
	if(argc == 1){
		int inputKey = 0;
		
		printf("Please enter the nane of the file to be converted:\r\n");
		scanf("%s", fileName);
		
		printf("Do you want to use AES-128 ECB to encrypt the image? 'Y' or 'N'.\r\n");
		do{
			scanf("%s", &inputKey);
		}while(inputKey != 'Y' && inputKey != 'N');
		
		if(inputKey == 'Y'){
			printf("Please input 16 bytes AES key in hex, e.g. 83FED3407A939723A5C639B26916D505\r\n");
			scanf("%s", aesKeyStr);
			aesEncrypt = 1;
		}
	}else{
		if((argc <= 1) || (argc > 3)){
			printUsage();
			return -1;
		}
		
		u8 i = 0;
		for(i = 0; i < argc; i++){
			switch(i){
				case 1:
					if(strlen(argv[1]) >= 128){
						printf("Argument 1 is too long, exceeding 128 bytes.\r\n");
						return -1;
					}else{
						strcpy(fileName, argv[1]);
					}
					break;
				case 2:
					if(strlen(argv[2]) != 32){
						printf("Argument 2 length error.\r\n");
						return -1;
					}else{
						strcpy(aesKeyStr, argv[2]);
						aesEncrypt = 1;
					}
					break;
				default:
					break;
			}
		}
	}
	
	/* Open files. */
	FILE *fwBin = openFWFile(fileName, strBuf);
	if(fwBin == NULL){
		return -1;
	}
	
	/* Whether to use AES encryption. */
	if(aesEncrypt){
		if(getAESKey(aesKeyStr, aesKey)){
			return -1;
		}
	}
	
	fileName[strlen(fileName) - 4] = 0;//Cut off the suffix '.bin'
	if(aesEncrypt){
		strcpy(&fileName[strlen(fileName)], FILE_NAME_ADD_SEC);
		
		otaImageSector.tagID = TAG_ID_UPGRADE_IMAGE_AES;
		otaImageSector.info = SUB_ELEM_INFO_AES_ECB;
	}
	
	/* Creat files. */
	FILE *otaBin = creatOTAFile(fileName, strBuf);
	if(otaBin == NULL){
		fclose(fwBin);
		return -1;
	}
	
	/* Point to the payload of the OTA image. */
	u8 offset = aesEncrypt ? (6 + 2) : 6;
	fpos_t pos = otaHdr.otaHdrLen + offset;
	fsetpos(otaBin, &pos);
	
	pos = 0;
	fsetpos(fwBin, &pos);
	
	u8 *ptr = NULL;
	u8 enBuf[64] = {0};
	u8 tmpBuf[64];
	u8 readDataLen = 0;
	u8 writeDataLen = 0;
	do{
		readDataLen = 0;
		writeDataLen = 0;
		memset(tmpBuf, 0xff, 64);
		
		readDataLen = fread(tmpBuf, 1, 64, fwBin);
		
		//printf("Read data len %d\n", readDataLen);
		
		if(readDataLen){
			if(aesEncrypt){
				u8 j = 0;
				for(j = 0; j < readDataLen; j += 16){
					aes_encrypt(aesKey, &tmpBuf[j], &enBuf[j]);
					writeDataLen += 16;
				}
				
				ptr = enBuf;
			}else{
				writeDataLen = readDataLen;
				
				ptr = tmpBuf;
			}
			
			pos += readDataLen;
			fsetpos(fwBin, &pos);
			
			//printf("Write data len %d\n", writeDataLen);
			
			otaImageSector.lengthField += writeDataLen;
			fwrite(ptr, writeDataLen, 1, otaBin);
		}
	}while(readDataLen >= 64);
	
	otaImageSector.fillNum = (u8)(otaImageSector.lengthField - BIN_SIZE_GET(strBuf));
	printf("Sub-element length: %d\n", otaImageSector.lengthField);
	printf("Sub-element ManuInfo: Info: %x, fillNum: %d\n", otaImageSector.info, otaImageSector.fillNum);//fill N 0xff
	
	otaHdr.totalImageSize = otaHdr.otaHdrLen + offset + otaImageSector.lengthField;
	
	pos = 0;
	fsetpos(otaBin, &pos);
	fwrite((u8 *)&otaHdr, otaHdr.otaHdrLen, 1, otaBin);
	pos += otaHdr.otaHdrLen;
	fsetpos(otaBin, &pos);
	fwrite((u8 *)&otaImageSector, offset, 1, otaBin);
	
	printf("Creat file success!\n");
	
	fclose(fwBin);
	fclose(otaBin);
	
	return 0;
}
