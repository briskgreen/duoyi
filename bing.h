#ifndef _BING_H
#define _BING_H

#include <json-glib/json-glib.h>
#include <curl/curl.h>

char *bing_translate(char *from,char *to,char *api,char *word);

char *bing_error(char *code);

char *bing_parser(char *data);

#endif
