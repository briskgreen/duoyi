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

char *string_add(const char *format,...);

char *stradd(char *from,const char *add);

char *url_encode(char *str);

size_t curl_read_to_data(char *ptr,size_t size,size_t nmemb,DATA *data);

#endif
