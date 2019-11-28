/********************************************************************************************************
 * @file     string.c
 *
 * @brief
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "types.h"
#include "string.h"
#include "assert.h"
#include "../os/ev_buffer.h"
#include "../os/ev.h"
#include "zb_buffer.h"

char* strcpy(char * dst0, const char * src0) {
	char *s = dst0;
	while ((*dst0++ = *src0++))
		;
	return s;
}

char * strchr(const char *s, int c) {
	do {
		if (*s == c) {
			return (char*) s;
		}
	} while (*s++);
	return (0);
}

int memcmp(const void * m1, const void *m2, unsigned int n) {

	unsigned char *s1 = (unsigned char *) m1;
	unsigned char *s2 = (unsigned char *) m2;

	while (n--) {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		s1++;
		s2++;
	}
	return 0;
}

void *
memchr(register const void * src_void, int c, unsigned int length) {
	const unsigned char *src = (const unsigned char *) src_void;

	while (length-- > 0) {
		if (*src == c)
			return (void *) src;
		src++;
	}
	return NULL;
}

void * memmove(void * dest, const void * src, unsigned int n) {
	char * d = (char *)dest;
	char * s = (char *)src;

	while (n--)
		*d++ = *s++;

	return dest;
}

void bcopy(register char * src, register char * dest, int len) {
	if (dest < src)
		while (len--)
			*dest++ = *src++;
	else {
		char *lasts = src + (len - 1);
		char *lastd = dest + (len - 1);
		while (len--)
			*(char *) lastd-- = *(char *) lasts--;
	}
}



#if 1
void * memset(void * dest, int val, unsigned int len) {
	if ( dest == NULL ) {
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
	}
    if(is_ev_buf(dest) && (len > LARGE_BUFFER)){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }
    if(is_zb_buf(dest) && (len > ZB_BUF_SIZE)){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }

	register unsigned char *ptr = (unsigned char*) dest;
	while (len-- > 0)
		*ptr++ = (unsigned char)val;
	return dest;
}
void* memcpy(void * out, const void * in, unsigned int length) {
	if ( length == 0 ) {
		return NULL;
	}
	if(out == NULL){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
	}
	if(in == NULL){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
	}
    if(is_ev_buf(out) && (length > LARGE_BUFFER)){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }
    if(is_zb_buf(out) && (length > ZB_BUF_SIZE) && (length != (ZB_BUF_SIZE + sizeof(zb_buf_hdr_t) - 1))){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }

	bcopy((char *) in, (char *) out, (int) length);
	return out;
}
#else
volatile unsigned int AAtestLine0 = 0;
void * mymemset(void * dest, int val, unsigned int len, unsigned int line) {
	AAtestLine0 = line;

	if ( dest == NULL ) {
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
	}

    if(is_ev_buf(dest) && (len > LARGE_BUFFER)){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }

    if(is_zb_buf(dest) && (len > ZB_BUF_SIZE)){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }

	register unsigned char *ptr = (unsigned char*) dest;
	while (len-- > 0)
		*ptr++ = (unsigned char)val;
	return dest;
}

volatile unsigned int AAtestLine = 0;
void *  mymemcpy(void * out, const void * in, unsigned int length, unsigned int line){
	AAtestLine = line;

	if ( length == 0 ) {
		return NULL;
	}

	if(out == NULL){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
	}

	if(in == NULL){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
	}

    if(is_ev_buf(out) && (length > LARGE_BUFFER)){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }

    if(is_zb_buf(out) && (length > ZB_BUF_SIZE) && (length != (ZB_BUF_SIZE + sizeof(zb_buf_hdr_t) - 1))){
    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_MEM_ACCESS);
    }

	bcopy((char *) in, (char *) out, (int) length);
	return out;
}
#endif

// for performance, assume lenght % 4 == 0,  and no memory overlapped
void memcpy4(void * d, const void * s, unsigned int length){
	int* dst = (int*)d;
	int* src = (int*)s;
	assert((((int)dst) >> 2) << 2 == ((int)dst));			// address must alighn to 4
	assert((((int)src) >> 2) << 2 == ((int)src));			// address must alighn to 4
	assert((length >> 2) << 2 == length);					// lenght % 4 == 0
	assert(( ((char*)dst) + length <= (const char*)src) || (((const char*)src) + length <= (char*)dst));	//  no overlapped
	unsigned int len = length >> 2;
	while(len --){
		*dst++ = *src++;
	}
}

unsigned int strlen(const char *str) {

	unsigned int len = 0;

	if (str != NULL) {
		while (*str++) {

			len++;

		}
	}

	return len;
}

int strcmp(const char* firstString, const char* secondString) {
	while (*firstString == *secondString) {
		if (*firstString == '\0') {
			return 0;
		}
		++firstString;
		++secondString;
	}
	if (((unsigned char) *firstString - (unsigned char) *secondString) < 0) {
		return -1;
	}
	return 1;
}

char * strncpy(char *s, const char *t, unsigned int n) {
	char *p = s;
	unsigned int i = 0;

	if (!s)
		return s;

	while (t && i < n) {
		*s++ = *t++;
		i++;
	}

	if (!t) {
		do
			*s++ = '\0';
		while (i++ < n);
	}
	return p;
}

int ismemzero4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}

int ismemf4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(int i = 0; i < len; ++i){
		if(*p != 0xffffffff){
			return 0;
		}
		++p;
	}
	return 1;
}

void * memset4(void * dest, int val, unsigned int len) {
	int *p = (int*)dest;
	len = len >> 2;
	for(int i = 0; i < len; ++i){
		*p++ = val;
	}
	return dest;
}

void zeromem4(void *data, unsigned int len){
	memset4(data, 0, len);
}


