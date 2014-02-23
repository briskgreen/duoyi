#ifndef _KING_H
#define _KING_H

#include <libxml/parser.h>
#include <curl/curl.h>

char *king_translate(char *api,char *word);

char *king_error(char *code);

char *king_parser(char *data);

#endif
