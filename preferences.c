/*首选项程序回调函数文件*/
#include "preferences.h"

void pre_quit_with_save(GtkWidget *widget,gpointer data)
{
	PREData *pre=(PREData *)data;
	int i;

	/*设置API*/
	for(i=0;i != 4;++i)
	{
		if(gtk_entry_get_text_length(GTK_ENTRY(pre->api[i])) == 0)
			pre->data.api[i]=NULL;
		else
			pre->data.api[i]=gtk_entry_get_text(GTK_ENTRY(pre->api[i]));
	}

	/*写稿配置文件*/
	duoyi_write_config(&pre->data);
	gtk_main_quit();
}

void pre_set_font(GtkWidget *widget,gpointer data)
{
	PREData *pre=(PREData *)data;

	/*设置状态为已改动*/
	pre->changed=TRUE;
	/*得到选择的字体名称*/
	pre->data.font=gtk_font_button_get_font_name(GTK_FONT_BUTTON(widget));
}

void pre_set_default_dic(GtkWidget *widget,gpointer data)
{
	PREData *pre=(PREData *)data;

	/*同上*/
	pre->changed=TRUE;
	/*设置默认词典*/
	pre->data.dic=gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
}

void pre_save_flag(GtkWidget *widget,gpointer data)
{
	PREData *pre=(PREData *)data;

	/*更改操作状态
	 * API输入框有过变动则设置状态为已更改
	 */
	pre->changed=TRUE;
}

void pre_quit(GtkWidget *widget,gpointer data)
{
	GtkWidget *dialog;
	PREData *pre=(PREData *)data;
	int status;

	/*如果没有改动过则直接退出,否则弹出对话框*/
	if(!pre->changed)
	{
		gtk_main_quit();
		return;
	}
	
	dialog=gtk_message_dialog_new(NULL,
			GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_YES_NO,
			"发现您修改了配置文件\n您真的要取消所有设置吗?");
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog),
			"img/64x64/message.png",NULL);
	status=gtk_dialog_run(GTK_DIALOG(dialog));

	/*选择是否保存*/
	switch(status)
	{
		case GTK_RESPONSE_NO:
			pre_quit_with_save(NULL,data);
			return;
		case GTK_RESPONSE_YES:
			gtk_main_quit();
			return;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
}
