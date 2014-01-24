/*首选项程序头文件*/
#ifndef _PREFERENCES_H
#define _PREFERENCES_H

#include <gtk/gtk.h>
#include "config.h"

/*changed 当前设置是否已经做过更改
 * data 多译数据
 * api API Key
 */
typedef struct
{
	gboolean changed;
	DuoYiData data;
	GtkWidget *api[4];
}PREData;

/*退出回调*/
void pre_quit(GtkWidget *widget,gpointer data);

/*设置字体回调*/
void pre_set_font(GtkWidget *widget,gpointer data);

/*设置默认词典*/
void pre_set_default_dic(GtkWidget *widget,gpointer data);

/*更改操作状态*/
void pre_save_flag(GtkWidget *widget,gpointer data);

/*保存退出*/
void pre_quit_with_save(GtkWidget *widget,gpointer data);

#endif
