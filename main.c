#include "tray.h"

void create_menu(GtkWidget *win,GtkWidget *vbox,GtkWidget *reader);
GtkWidget *create_combox(GtkWidget *hbox,char *label);
void init_select_data(SelectionData *select_data,GtkWidget *from,
		GtkWidget *to,DuoYiData *duoyi_data);
void add_dic_selection(GtkWidget *win,GtkWidget *hbox,SelectionData *data);

int main(int argc,char **argv)
{
	GtkWidget *win;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *scrolled;
	GtkWidget *reader;
	GtkWidget *displayer;
	GtkWidget *button;
	GtkWidget *frame;
	GtkWidget *from;
	GtkWidget *to;
	PangoFontDescription *font_name;
	SelectionData select_data;
	DuoYiData duoyi_data;
	TranData tran_data;

	setlocale(LC_ALL,"");
	setenv("LANG","zh_CN.UTF-8",1);
	duoyi_read_config(&duoyi_data);
	/*if(!g_thread_supported())
		g_thread_init(NULL);
	gdk_threads_init();*/
	gtk_init(&argc,&argv);

	win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),"多译");
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file(GTK_WINDOW(win),"img/64x64/yi.png",NULL);
	gtk_window_set_resizable(GTK_WINDOW(win),FALSE); 
	g_signal_connect(G_OBJECT(win),"delete-event",
			G_CALLBACK(duoyi_quit),NULL);

	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_container_add(GTK_CONTAINER(win),vbox);

	tray_create(win);
	reader=gtk_text_view_new();
	displayer=gtk_text_view_new();
	create_menu(win,vbox,reader);

	if(duoyi_data.font)
	{
		font_name=pango_font_description_from_string(duoyi_data.font);
		gtk_widget_override_font(reader,font_name);
		gtk_widget_override_font(displayer,font_name);
	}

	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
	select_data.select=duoyi_data.dic;
	//init_select_data(&select_data,from,to,&duoyi_data);
	add_dic_selection(win,hbox,&select_data);

	//from=create_combox(hbox,"从:");
	//to=create_combox(hbox,"翻译到:");

	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);
	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
	from=create_combox(hbox,"从:");
	to=create_combox(hbox,"翻译到:");
	init_select_data(&select_data,from,to,&duoyi_data);

	gtk_box_pack_start(GTK_BOX(vbox),
			gtk_separator_new(GTK_ORIENTATION_HORIZONTAL),
			FALSE,FALSE,5);

	scrolled=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
			GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	//displayer=gtk_text_view_new();
	gtk_widget_set_size_request(scrolled,0x260,0x95);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(displayer),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(displayer),GTK_WRAP_CHAR);
	gtk_container_add(GTK_CONTAINER(scrolled),displayer);
	frame=gtk_frame_new("翻译结果");
	gtk_container_add(GTK_CONTAINER(frame),scrolled);
	gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,0);
	gtk_container_set_border_width(GTK_CONTAINER(scrolled),0x5);

	gtk_box_pack_start(GTK_BOX(vbox),
			gtk_separator_new(GTK_ORIENTATION_HORIZONTAL),
			FALSE,FALSE,10);

	scrolled=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
			GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(scrolled,0x260,0x60);
	//reader=gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(reader),GTK_WRAP_CHAR);
	gtk_container_add(GTK_CONTAINER(scrolled),reader);
	frame=gtk_frame_new("输入");
	gtk_container_add(GTK_CONTAINER(frame),scrolled);
	gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,0);

	gtk_container_set_border_width(GTK_CONTAINER(scrolled),0x5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox),0x10);
	gtk_box_pack_start(GTK_BOX(vbox),
			gtk_separator_new(GTK_ORIENTATION_HORIZONTAL),
			FALSE,FALSE,10);

	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,0);

	button=gtk_button_new_with_label("清空");
	gtk_box_pack_end(GTK_BOX(hbox),button,FALSE,FALSE,10);
	g_signal_connect(G_OBJECT(button),"clicked",
			G_CALLBACK(duoyi_reader_cleanup),reader);

	button=gtk_button_new_with_label("从文件加载");
	gtk_box_pack_end(GTK_BOX(hbox),button,FALSE,FALSE,10);
	g_signal_connect(G_OBJECT(button),"clicked",
			G_CALLBACK(duoyi_read_from_file),reader);

	button=gtk_button_new_with_label("翻译");
	gtk_box_pack_end(GTK_BOX(hbox),button,TRUE,TRUE,80);
	
	tran_data.data=&select_data;
	tran_data.reader=reader;
	tran_data.displayer=displayer;
	g_signal_connect(G_OBJECT(button),"clicked",
			G_CALLBACK(duoyi_translate),&tran_data);

	gtk_widget_show_all(win);

	switch(select_data.select)
	{
		case 0:
			duoyi_baidu_select(NULL,&select_data);
			break;
		case 1:
			duoyi_bing_select(NULL,&select_data);
			break;
		case 2:
			duoyi_king_select(NULL,&select_data);
			break;
		case 3:
			duoyi_youdao_select(NULL,&select_data);
			break;
	}
	//gdk_threads_enter();
	gtk_main();
	//gdk_threads_leave();

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

GtkWidget *create_combox(GtkWidget *hbox,char *label)
{
	GtkWidget *box;
	GtkWidget *l;

	box=gtk_combo_box_text_new();
	l=gtk_label_new(label);
	gtk_box_pack_start(GTK_BOX(hbox),l,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox),box,TRUE,TRUE,10);

	return box;
}

void init_select_data(SelectionData *select_data,GtkWidget *from,
		GtkWidget *to,DuoYiData *duoyi_data)
{
	int i;

	//select_data->select=duoyi_data->dic;
	select_data->from=from;
	select_data->to=to;

	for(i=0;i != 4;++i)
		select_data->api[i]=duoyi_data->api[i];
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

	frame=gtk_frame_new("选择网络词典");
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
	gtk_widget_add_events(button,GDK_BUTTON_PRESS_MASK);
	if(data->select == 0)
	{
		gtk_button_clicked(GTK_BUTTON(button));
		gtk_button_clicked(GTK_BUTTON(button));
	}

	for(i=1;i != 4;++i)
	{
		group=gtk_radio_button_get_group(GTK_RADIO_BUTTON(button));
		image=gtk_image_new_from_file(path[i]);
		button=gtk_radio_button_new(group);
		gtk_button_set_image(GTK_BUTTON(button),image);
		gtk_button_set_label(GTK_BUTTON(button),name[i]);
		gtk_button_set_image_position(GTK_BUTTON(button),GTK_POS_TOP);

		gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
		g_signal_connect(G_OBJECT(button),"pressed",
				G_CALLBACK(func[i]),data);

		if(i == data->select)
			gtk_button_clicked(GTK_BUTTON(button));
	}
}
