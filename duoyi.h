/*主界面的回调函数头文件*/
#ifndef _DUOYI_H
#define _DUOYI_H

#include <gtk/gtk.h>
#include <locale.h>
#include <libintl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "config.h"
#include "baidu.h"
#include "bing.h"
#include "king.h"
#include "youdao.h"

/*状态栏菜单显示与隐藏按钮可用与否数据结构
 * widget指向主界面构件
 * item指向显示/隐藏按钮
 */
typedef struct
{
	GtkWidget *widget;
	GtkWidget *item;
}TrayData;

/*select 当前选择的词典
 * api 各词典的API Key
 * from 语言选择下拉框from
 * to 语言选择下拉框to
 */
typedef struct
{
	int select;
	char *api[4];

	GtkWidget *from;
	GtkWidget *to;
}SelectionData;

/*翻译按钮传递的数据
 * data 需要使用到的一些信息，如当前词典，API Key与语言
 * reader 读取内容
 * displayer 显示内容
 */
typedef struct
{
	SelectionData *data;

	GtkWidget *reader;
	GtkWidget *displayer;
}TranData;

/*定义callback为GTK回调函数类型的函数指针*/
typedef void (*callback)(GtkWidget *widget,gpointer data);

/*退出程序回调函数*/
void duoyi_quit(GtkWidget *widget,gpointer data);

/*点击状态栏图标的回调函数*/
void duoyi_hide_to_tray(GtkWidget *widget,GdkEventWindowState *event,
		gpointer data);

//void duoyi_combox_select(GtkWidget *widget,gpointer data);

/*隐藏主界面*/
void duoyi_hide_window(GtkWidget *widget,TrayData *data);

/*从文件中读取内容*/
void duoyi_read_from_file(GtkWidget *widget,gpointer data);

/*首选项回调函数*/
void duoyi_preferences(GtkWidget *widget,gpointer data);

/*关于对话框回调函数*/
void duoyi_about_dialog(GtkWidget *widget,gpointer data);

/*帮助对话框*/
void duoyi_help_dialog(GtkWidget *widget,gpointer data);

/*选择了百度词典回调函数*/
void duoyi_baidu_select(GtkWidget *widget,gpointer data);

/*选择了必应词典回调函数*/
void duoyi_bing_select(GtkWidget *widget,gpointer data);

/*选择了金山词霸回调函数*/
void duoyi_king_select(GtkWidget *widget,gpointer data);

/*选择了有道词典回调函数*/
void duoyi_youdao_select(GtkWidget *widget,gpointer data);

/*清空输入内容回调函数*/
void duoyi_reader_cleanup(GtkWidget *widget,gpointer data);

/*翻译按钮回调函数*/
void duoyi_translate(GtkWidget *widget,gpointer data);

/*错误信息对话框*/
void duoyi_error_msgbox(char *msg);

#endif
