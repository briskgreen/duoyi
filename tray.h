#ifndef _TRAY_H
#define _TRAY_H

#include "duoyi.h"

typedef struct
{
	TrayData *show;
	TrayData *hide;
}Tray;

void tray_create(GtkWidget *widget);

void tray_on_clicked_with_tray(GtkWidget *widget,Tray *data);

void tray_on_clicked(GtkWidget *widget,TrayData *data);

void tray_on_menu(GtkStatusIcon *tray,guint button,
		guint activate_item,gpointer data);

#endif
