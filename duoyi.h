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

typedef struct
{
	GtkWidget *widget;
	GtkWidget *item;
}TrayData;

typedef struct
{
	int select;
	char *api[4];

	GtkWidget *from;
	GtkWidget *to;
}SelectionData;

typedef struct
{
	SelectionData *data;

	GtkWidget *reader;
	GtkWidget *displayer;
}TranData;

typedef void (*callback)(GtkWidget *widget,gpointer data);

void duoyi_quit(GtkWidget *widget,gpointer data);

void duoyi_hide_to_tray(GtkWidget *widget,GdkEventWindowState *event,
		gpointer data);

void duoyi_combox_select(GtkWidget *widget,gpointer data);

void duoyi_hide_window(GtkWidget *widget,TrayData *data);

void duoyi_read_from_file(GtkWidget *widget,gpointer data);

void duoyi_preferences(GtkWidget *widget,gpointer data);

void duoyi_about_dialog(GtkWidget *widget,gpointer data);

void duoyi_help_dialog(GtkWidget *widget,gpointer data);

void duoyi_baidu_select(GtkWidget *widget,gpointer data);

void duoyi_bing_select(GtkWidget *widget,gpointer data);

void duoyi_king_select(GtkWidget *widget,gpointer data);

void duoyi_youdao_select(GtkWidget *widget,gpointer data);

void duoyi_reader_cleanup(GtkWidget *widget,gpointer data);

void duoyi_translate(GtkWidget *widget,gpointer data);

void duoyi_error_msgbox(char *msg);

#endif
