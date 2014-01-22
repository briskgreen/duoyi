#ifndef _CONFIG_H
#define _CONFIG_H

#include <json-glib/json-glib.h>

typedef struct
{
	char *font;
	int dic;
	char *api[4];
}DuoYiData;

void duoyi_read_config(DuoYiData *data);

void duoyi_wite_config(DuoYiData *data);

#endif
