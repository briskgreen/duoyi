#ifndef _YOUDAO_H
#define _YOUDAO_H

#include <json-glib/json-glib.h>
#include <curl/curl.h>

char *youdao_translate(char *api,char *word);

char *youdao_error(int code);

char *youdao_parser(char *data);

#endif
