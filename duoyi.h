#ifndef _DUOYI_H
#define _DUOYI_H

#include <gtk/gtk.h>
#include <locale.h>
#include <libintl.h>
#include <stdlib.h>

#define _(string) gettext(string)
#define N_(string) string

#define TO_UTF8(str) g_locale_to_utf8(str,-1,NULL,NULL,NULL)

typedef struct
{
	GtkWidget *widget;
	GtkWidget *item;
}TrayData;

typedef void (*callback)(GtkWidget *widget,gpointer data);

void duoyi_quit(GtkWidget *widget,gpointer data);

void duoyi_hide_to_tray(GtkWidget *widget,GdkEventWindowState *event,
		gpointer data);

void duoyi_hide_window(GtkWidget *widget,TrayData *data);

void duoyi_read_from_file(GtkWidget *widget,gpointer data);

void duoyi_about_dialog(GtkWidget *widget,gpointer data);

void duoyi_baidu_select(GtkWidget *widget,gpointer data);

void duoyi_bing_select(GtkWidget *widget,gpointer data);

void duoyi_king_select(GtkWidget *widget,gpointer data);

void duoyi_youdao_select(GtkWidget *widget,gpointer data);

#endif
