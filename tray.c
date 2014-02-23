#include "tray.h"

void tray_create(GtkWidget *widget)
{
	GtkStatusIcon *tray;
	GtkWidget *menu;
	GtkWidget *show;
	GtkWidget *hide;
	GtkWidget *quit;
	GtkWidget *item;
	TrayData *show_data;
	TrayData *hide_data;
	Tray *data;

	/*为数据开辟内存，不然会出错哦*/
	show_data=malloc(sizeof(TrayData));
	hide_data=malloc(sizeof(TrayData));
	show_data->widget=hide_data->widget=widget;
	data=malloc(sizeof(Tray));
	data->show=show_data;
	data->hide=hide_data;

	/*创建状态栏图标*/
	tray=gtk_status_icon_new_from_file("img/64x64/yi.png");
	gtk_status_icon_set_tooltip_text(tray,"多译");
	/*默认状态栏图标可见*/
	gtk_status_icon_set_visible(tray,TRUE);
	g_signal_connect(G_OBJECT(tray),"activate",
			G_CALLBACK(tray_on_clicked_with_tray),data);

	/*创建状态栏右键菜单*/
	menu=gtk_menu_new();

	/*显示主界面*/
	show=gtk_menu_item_new_with_mnemonic("显示(_S)");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),show);
	gtk_widget_set_sensitive(show,FALSE);

	/*隐藏主界面*/
	hide=gtk_menu_item_new_with_mnemonic("隐藏(_i)");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),hide);

	/*设置相应数据，当界面可见时设置显示按钮不可用
	 * 当界面不可见时设置隐藏按钮不可用
	 */
	show_data->item=hide;
	hide_data->item=show;

	g_signal_connect(G_OBJECT(show),"activate",
			G_CALLBACK(tray_on_clicked),show_data);
	g_signal_connect(G_OBJECT(hide),"activate",
			G_CALLBACK(duoyi_hide_window),hide_data);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());

	/*添加首选项菜单项*/
	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_preferences),NULL);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());

	/*添加帮助菜单项*/
	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_help_dialog),NULL);

	/*添加关于菜单项*/
	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_about_dialog),NULL);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());

	/*添加退出程序菜单项*/
	quit=gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),quit);
	g_signal_connect(G_OBJECT(quit),"activate",
			G_CALLBACK(duoyi_quit),widget);

	gtk_widget_show_all(menu);
	/*右键弹出菜单信号*/
	g_signal_connect(G_OBJECT(tray),"popup-menu",
			G_CALLBACK(tray_on_menu),menu);
	/*主界面状态信号*/
	g_signal_connect(G_OBJECT(widget),"window-state-event",
			G_CALLBACK(duoyi_hide_to_tray),tray);
}

void tray_on_clicked_with_tray(GtkWidget *widget,Tray *data)
{
	/*判断当然界面是否可见，如不可见则显示主界面并设置状态栏显示按钮不可用隐藏按钮可用
	 * 否则与上面的操作相反
	 */
	if(!gtk_widget_get_visible(GTK_WIDGET(data->show->widget)))
	{
		gtk_widget_show_all(GTK_WIDGET(data->show->widget));
		gtk_window_present(GTK_WINDOW(data->show->widget));

		gtk_widget_set_sensitive(GTK_WIDGET(data->show->item),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(data->hide->item),FALSE);
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(data->show->widget));

		gtk_widget_set_sensitive(GTK_WIDGET(data->hide->item),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(data->show->item),FALSE);
	}
}

void tray_on_clicked(GtkWidget *widget,TrayData *data)
{
	/*与上面的函数功能相同，不同的是传递进来的参数有所不同
	 * 上面的回调函数用于处理点击状态栏图标
	 * 而本回调函数用于处理点击菜单栏隐藏按钮
	 */
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
	/*弹出菜单*/
	gtk_menu_popup(GTK_MENU(data),NULL,NULL,
			gtk_status_icon_position_menu,tray,
			button,activate_item);
}
