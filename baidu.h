#ifndef _BAIDU_H
#define _BAIDU_H

#include <json-glib/json-glib.h>
#include <curl/curl.h>

char *baidu_translate(char *from,char *to,char *api,char *word);

char *baidu_error(char *code);

char *baidu_parser(char *data);

#endif
