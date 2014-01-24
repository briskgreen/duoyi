#include "preferences.h"
#include <locale.h>

int main(int argc,char **argv)
{
	GtkWidget *win;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *notebook;
	GtkWidget *page;
	GtkWidget *button;
	GtkWidget *api[4];
	GtkWidget *box;
	GtkWidget *label;
	PREData data;
	char *name[]={"百度词典:","必应词典:","金山词霸:","有道词典:"};
	char *dic[]={"百度词典","必应词典","金山词霸","有道词典"};
	int i;

	/*设置默认UTF-8编码*/
	setlocale(LC_ALL,"");
	setenv("LANG","zh_CN.UTF-8");
	/*先读取出配置*/
	duoyi_read_config(&data.data);
	/*默认状态为未更改*/
	data.changed=FALSE;
	gtk_init(&argc,&argv);

	win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),"配置");
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file(GTK_WINDOW(win),
			"img/64x64/preferences.png",NULL);
	gtk_window_set_resizable(GTK_WINDOW(win),FALSE);
	g_signal_connect(G_OBJECT(win),"delete-event",
			G_CALLBACK(pre_quit),&data);

	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	notebook=gtk_notebook_new();
	gtk_container_add(GTK_CONTAINER(win),vbox);

	gtk_box_pack_start(GTK_BOX(vbox),notebook,FALSE,FALSE,5);
	/*添加常规页*/
	page=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	label=gtk_label_new("常规");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),page,label);
	/*添加选择字体按钮*/
	label=gtk_label_new("选择字体:");
	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(page),hbox,FALSE,FALSE,20);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,20);

	if(data.data.font)
		button=gtk_font_button_new_with_font(data.data.font);
	else
		button=gtk_font_button_new();
	/*按钮使用字体显示文字*/
	gtk_font_button_set_use_font(GTK_FONT_BUTTON(button),TRUE);
	gtk_font_button_set_title(GTK_FONT_BUTTON(button),"字体选择");
	gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,100);
	g_signal_connect(G_OBJECT(button),"font-set",
			G_CALLBACK(pre_set_font),&data);

	/*添加默认词典选择*/
	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(page),hbox,FALSE,FALSE,20);
	label=gtk_label_new("默认词典:");
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,20);
	box=gtk_combo_box_text_new();
	for(i=0;i != 4;++i)
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(box),dic[i]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(box),data.data.dic);
	g_signal_connect(G_OBJECT(box),"changed",
				G_CALLBACK(pre_set_default_dic),&data);
	//gtk_combo_box_set_active(GTK_COMBO_BOX(box),data.data.dic);
	gtk_box_pack_start(GTK_BOX(hbox),box,TRUE,TRUE,100);


	/*添加API页*/
	page=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	label=gtk_label_new("API");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),page,label);
	for(i=0;i != 4;++i)
	{
		/*名称*/
		label=gtk_label_new(name[i]);
		/*api输入框*/
		api[i]=gtk_entry_new();
		/*api*/
		data.api[i]=api[i];
		/*如果当前API已设置，则显示出来*/
		if(data.data.api[i])
			gtk_entry_set_text(GTK_ENTRY(api[i]),data.data.api[i]);
		hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
		gtk_box_pack_start(GTK_BOX(page),hbox,TRUE,TRUE,10);
		gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,10);
		gtk_box_pack_start(GTK_BOX(hbox),api[i],TRUE,TRUE,10);
		/*如有改动更改操作状态*/
		g_signal_connect(G_OBJECT(api[i]),"changed",
				G_CALLBACK(pre_save_flag),&data);
	}

	hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_end(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(vbox),
			gtk_separator_new(GTK_ORIENTATION_HORIZONTAL),
			FALSE,FALSE,5);
	label=gtk_label_new("所有更改均在重启后生效!");
	gtk_box_pack_end(GTK_BOX(vbox),label,FALSE,FALSE,5);

	/*取消按钮*/
	button=gtk_button_new_with_label("取消");
	gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,20);
	g_signal_connect(G_OBJECT(button),"clicked",
			G_CALLBACK(pre_quit),&data);

	/*保存按钮*/
	button=gtk_button_new_with_label("保存");
	gtk_box_pack_end(GTK_BOX(hbox),button,FALSE,FALSE,20);
	g_signal_connect(G_OBJECT(button),"clicked",
			G_CALLBACK(pre_quit_with_save),&data);

	gtk_widget_show_all(win);
	gtk_main();

	return 0;
}
