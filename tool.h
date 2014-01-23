#ifndef _TOOL_H
#define _TOOL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct
{
	char *data;
}DATA;

char *string_add(char *format,...);

char *url_encode(char *str);

unsigned char *base64_encode(char *str);

size_t curl_read_to_data(char *ptr,size_t size,size_t nmemb,DATA *data);

#endif
