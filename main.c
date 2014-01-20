#include "tray.h"

void create_menu(GtkWidget *win,GtkWidget *vbox,GtkWidget *reader);
void add_dic_selection(GtkWidget *win,GtkWidget *hbox,SelectionData *data);

int main(int argc,char **argv)
{
	GtkWidget *win;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *reader;
	GtkWidget *displayer;
	SelectionData select_data;

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
	add_dic_selection(win,hbox,&select_data);

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
	item=gtk_menu_item_new_with_mnemonic("编辑(_E)");
	gtk_menu_shell_append(GTK_MENU_SHELL(bar),item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),menu);
	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES,accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_preferences),NULL);

	menu=gtk_menu_new();
	item=gtk_menu_item_new_with_mnemonic("帮助(_H)");
	gtk_menu_shell_append(GTK_MENU_SHELL(bar),item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),menu);

	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT,accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_about_dialog),NULL);
	item=gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP,accel_group);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
	g_signal_connect(G_OBJECT(item),"activate",
			G_CALLBACK(duoyi_help_dialog),NULL);
}

void add_dic_selection(GtkWidget *win,GtkWidget *hbox,SelectionData *data)
{
	GtkWidget *vbox;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *image;
	GSList *group;

	int i;
	char *name[]={"百度翻译","必应翻译","金山词霸","有道翻译"};
	char *path[]={"img/baidu.png","img/bing.png","img/king.png","img/youdao.png"};
	callback func[]={duoyi_baidu_select,duoyi_bing_select,
	duoyi_king_select,duoyi_youdao_select};

	frame=gtk_frame_new(TO_UTF8("选择网络词典"));
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,5);
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
	gtk_container_add(GTK_CONTAINER(frame),vbox);
	gtk_container_set_border_width(GTK_CONTAINER(vbox),0x10);

	button=gtk_radio_button_new(NULL);
	image=gtk_image_new_from_file(path[0]);
	gtk_button_set_image(GTK_BUTTON(button),image);
	gtk_button_set_label(GTK_BUTTON(button),name[0]);
	gtk_button_set_image_position(GTK_BUTTON(button),GTK_POS_TOP);
	gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);

	g_signal_connect(G_OBJECT(button),"pressed",
				G_CALLBACK(func[0]),data);
	for(i=1;i != 4;++i)
	{
		group=gtk_radio_button_get_group(GTK_RADIO_BUTTON(button));
		image=gtk_image_new_from_file(path[i]);
		button=gtk_radio_button_new(group);
		gtk_button_set_image(GTK_BUTTON(button),image);
		gtk_button_set_label(GTK_BUTTON(button),TO_UTF8(name[i]));
		gtk_button_set_image_position(GTK_BUTTON(button),GTK_POS_TOP);

		gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
		g_signal_connect(G_OBJECT(button),"pressed",
				G_CALLBACK(func[i]),data);
	}
}
