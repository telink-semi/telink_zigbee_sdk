// readelf.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <windows.h>
#include "read_elf.h"


#define DEBUG_GET_KEY_PAIR_EN           0	// just for get key, not for exe.
#define DEBUG_FILE_NAME_EN              0	// just for debug
#define DEBUG_GET_UNLOCK_SIGNATURE_EN   0   // just debug to get signature of unlock DP, not for 'exe'.


#define DEBUG_GET_KEY_PAIR_EN           0	// just for get key, not for exe.
#define DEBUG_FILE_NAME_EN              0	// just for debug
#define DEBUG_GET_UNLOCK_SIGNATURE_EN   0   // just debug to get signature of unlock DP, not for 'exe'.

#define ENCODE_BIN_KEY_VAR_NAME     "otaAesKey"
#define ENCODE_BIN_FILE_VAR_NAME    "NORMAL_BIN_USER_NAME_CONST"
#define NORMAL_BIN_FILE_VAR_NAME    "NORMAL_BIN_USER_NAME_CONST"    // normal bin

#define DEVICE_ID_INFO              "sampleLight_simpleDesc"
#define MODE_ID_INFO                "sampleLight_simpleDesc"


int g_user_enc_bin_name_len = ELF_INVALID_LEN;
extern "C" char g_user_enc_bin_name[1024] = {0};
extern "C"  char g_key_hex[1024] = { 0 };
extern "C"  int g_key_len = ELF_INVALID_LEN;
extern "C"  int g_user_normal_bin_name_len = ELF_INVALID_LEN;
extern "C" char g_user_normal_bin_name[1024] = { 0 };
extern "C" char g_user_dev_info[1024] = { 0 };
extern "C" char g_user_dev_info_len = 16;


//int main(int argc, char* argv[])
extern "C"
int bin_file_name_get(char *file)
{   
    

    ElfReader elfReader;
    if (0 != elfReader.read_elf_file(file)) {
       // err = -1;
        printf("Error: Can not open ELF file: %s", file);
    }
    else {
        elfReader.read_all();
        char constant_name[] = "otaAesKey";// "key_encode_bin";
        g_key_len = elfReader.read_constant(constant_name, g_key_hex, sizeof(g_key_hex));
        //printf("aes in the elf file:%x, %x\n", g_key_hex[0], g_key_hex[1]);

        //char constant_bin_name[] = ENCODE_BIN_FILE_VAR_NAME;
        //g_user_enc_bin_name_len = elfReader.read_fw_file_name(constant_bin_name, g_user_enc_bin_name, sizeof(g_user_enc_bin_name));
        //g_user_enc_bin_name_len = elfReader.read_constant(constant_bin_name, g_user_enc_bin_name, sizeof(g_user_enc_bin_name));

        char constant_bin_name[] = NORMAL_BIN_FILE_VAR_NAME;
        g_user_normal_bin_name_len = elfReader.read_fw_file_name(constant_bin_name, g_user_normal_bin_name, sizeof(g_user_normal_bin_name));
        //printf("elf file name:%s\n", file);
        //printf("bin file name:%s\n", g_user_normal_bin_name);
       
        char constant_device_info[] = DEVICE_ID_INFO;
        g_user_dev_info_len = elfReader.read_fw_file_name(constant_device_info, g_user_dev_info, sizeof(g_user_dev_info));
        printf("device Id is: %x\n", g_user_dev_info[2] | (g_user_dev_info[3] << 8));
    }

    return 0;
}

