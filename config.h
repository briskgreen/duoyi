/*读取配置文件头文件*/
#ifndef _CONFIG_H
#define _CONFIG_H

#include <json-glib/json-glib.h>

/*font 字体
 * dic 默认词典
 * api API Key列表
 */
typedef struct
{
	char *font;
	int dic;
	char *api[4];
}DuoYiData;

/*读取配置文件*/
void duoyi_read_config(DuoYiData *data);

/*写入配置文件*/
void duoyi_wite_config(DuoYiData *data);

#endif
