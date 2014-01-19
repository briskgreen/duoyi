#include "tray.h"

void tray_create(GtkWidget *widget)
{
	GtkStatusIcon *tray;
	GtkWidget *menu;
	GtkWidget *show;
	GtkWidget *hide;
	GtkWidget *quit;
	TrayData *show_data;
	TrayData *hide_data;

	tray=gtk_status_icon_new_from_file("img/64x64/yi.png");
	gtk_status_icon_set_tooltip_text(tray,TO_UTF8("多译"));
	gtk_status_icon_set_visible(tray,TRUE);
	g_signal_connect(G_OBJECT(tray),"activate",
			G_CALLBACK(tray_on_clicked),widget);

	menu=gtk_menu_new();

	show=gtk_menu_item_new_with_mnemonic(TO_UTF8("显示(_S)"));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),show);
	gtk_widget_set_sensitive(show,FALSE);

	hide=gtk_menu_item_new_with_mnemonic(TO_UTF8("隐藏(_i)"));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),hide);

	show_data=malloc(sizeof(TrayData));
	hide_data=malloc(sizeof(TrayData));
	show_data->widget=hide_data->widget=widget;
	show_data->item=hide;
	hide_data->item=show;

	g_signal_connect(G_OBJECT(show),"activate",
			G_CALLBACK(tray_on_clicked),show_data);
	g_signal_connect(G_OBJECT(hide),"activate",
			G_CALLBACK(duoyi_hide_window),hide_data);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());

	quit=gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),quit);
	g_signal_connect(G_OBJECT(quit),"activate",
			G_CALLBACK(duoyi_quit),widget);

	gtk_widget_show_all(menu);
	g_signal_connect(G_OBJECT(tray),"popup-menu",
			G_CALLBACK(tray_on_menu),menu);
	g_signal_connect(G_OBJECT(widget),"window-state-event",
			G_CALLBACK(duoyi_hide_to_tray),tray);
}

void tray_on_clicked(GtkWidget *widget,TrayData *data)
{
	if(!gtk_widget_get_visible(GTK_WIDGET(data->widget)))
	{
		gtk_widget_show_all(GTK_WIDGET(data->widget));
		gtk_window_present(GTK_WINDOW(data->widget));

		gtk_widget_set_sensitive(widget,FALSE);
		gtk_widget_set_sensitive(data->item,TRUE);
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(data->widget));

		gtk_widget_set_sensitive(widget,TRUE);
		gtk_widget_set_sensitive(data->item,FALSE);
	}
}

void tray_on_menu(GtkStatusIcon *tray,guint button,
		guint activate_item,gpointer data)
{
	gtk_menu_popup(GTK_MENU(data),NULL,NULL,
			gtk_status_icon_position_menu,tray,
			button,activate_item);
}
