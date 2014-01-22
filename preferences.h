#ifndef _PREFERENCES_H
#define _PREFERENCES_H

#include <gtk/gtk.h>
#include "config.h"

typedef struct
{
	gboolean changed;
	DuoYiData data;
	GtkWidget *api[4];
}PREData;

void pre_quit(GtkWidget *widget,gpointer data);

void pre_set_font(GtkWidget *widget,gpointer data);

void pre_set_default_dic(GtkWidget *widget,gpointer data);

void pre_save_flag(GtkWidget *widget,gpointer data);

void pre_quit_with_save(GtkWidget *widget,gpointer data);

#endif
