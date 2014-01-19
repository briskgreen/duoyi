#include "tray.h"

void create_menu(GtkWidget *win,GtkWidget *vbox,GtkWidget *reader);
void add_dic_selection(GtkWidget *win,GtkWidget *hbox);
void add_image_button(GtkWidget *vbox,char *name,char *filename,
		callback func,gpointer data);

int main(int argc,char **argv)
{
	GtkWidget *win;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *reader;
	GtkWidget *displayer;

	gtk_init(&argc,&argv);

	win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),TO_UTF8("多译"));
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file(GTK_WINDOW(win),"img/64x64/yi.png",NULL);
	g_signal_connect(G_OBJECT(win),"delete-event",
			G_CALLBACK(duoyi_quit),NULL);

	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_container_add(GTK_CONTAINER(win),vbox);

	tray_create(win);
	reader=gtk_text_view_new();
	displayer=gtk_text_view_new();
	create_menu(win,vbox,reader);

	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,10);
	add_dic_selection(win,hbox);

	gtk_widget_show_all(win);
	gtk_main();

	return 0;
}

void create_menu(GtkWidget *win,GtkWidget *vbox,GtkWidget *reader)
{
	GtkWidget *bar;
	GtkWidget *menu;
	GtkWidget *item;
	GtkAccelGroup *accel_group;

	accel_group=gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(win),accel_group);

	bar=gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(vbox),bar,FALSE,FALSE,0);
	menu=gtk_menu_new();

	item=gtk_menu_item_new_with_mnemonic("文件(_F)");
	gtk_menu_shell_append(GTK_MENU_SHELL(bar),item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),menu);

	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_read_from_file),reader);
	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_quit),NULL);

	menu=gtk_menu_new();
	item=gtk_menu_item_new_with_mnemonic("帮助(_H)");
	gtk_menu_shell_append(GTK_MENU_SHELL(bar),item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),menu);

	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT,accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_about_dialog),NULL);
}

void add_image_button(GtkWidget *vbox,char *name,char *filename,
		callback func,gpointer data)
{
	GtkWidget *button;
	GtkWidget *image;

	image=gtk_image_new_from_file(filename);
	button=gtk_toggle_button_new();

	gtk_button_set_image(GTK_BUTTON(button),image);
	gtk_button_set_image_position(GTK_BUTTON(button),GTK_POS_TOP);
	gtk_button_set_label(GTK_BUTTON(button),name);
	gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);

	g_signal_connect(G_OBJECT(button),"toggled",
			G_CALLBACK(func),data);
}

void add_dic_selection(GtkWidget *win,GtkWidget *hbox)
{
	GtkWidget *vbox;
	GtkWidget *frame;

	frame=gtk_frame_new(TO_UTF8("选择网络词典"));
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,5);
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
	gtk_container_add(GTK_CONTAINER(frame),vbox);
	gtk_container_set_border_width(GTK_CONTAINER(vbox),0x10);

	add_image_button(vbox,"百度翻译","img/baidu.png",
		duoyi_baidu_select,NULL);
	add_image_button(vbox,"必应翻译","img/bing.png",
		duoyi_bing_select,NULL);
	add_image_button(vbox,"金山词霸","img/king.png",
		duoyi_king_select,NULL);
	add_image_button(vbox,"有道翻译","img/youdao.png",
		duoyi_youdao_select,NULL);
}
