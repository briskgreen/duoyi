#include "duoyi.h"

void _load_file(char *filename,gpointer data);

void duoyi_quit(GtkWidget *widget,gpointer data)
{
	GtkWidget *dialog;
	int status;

	dialog=gtk_message_dialog_new(NULL,
			GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_OK_CANCEL,
			TO_UTF8("您确定要退出本程序吗？\n按下取消键继续本程序"),NULL);

	status=gtk_dialog_run(GTK_DIALOG(dialog));

	switch(status)
	{
		case GTK_RESPONSE_OK:
			gtk_main_quit();
			break;
		case GTK_RESPONSE_CANCEL:
		default:
			break;
	}

	gtk_widget_destroy(dialog);
}

void duoyi_hide_to_tray(GtkWidget *widget,GdkEventWindowState *event,
		gpointer data)
{
	if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && event->new_window_state == GDK_WINDOW_STATE_ICONIFIED)
		gtk_widget_hide(widget);
}

void duoyi_hide_window(GtkWidget *widget,TrayData *data)
{
	gtk_widget_hide(data->widget);

	gtk_widget_set_sensitive(widget,FALSE);
	gtk_widget_set_sensitive(data->item,TRUE);
}

void duoyi_combox_select(GtkWidget *widget,gpointer data)
{}

void duoyi_read_from_file(GtkWidget *widget,gpointer data)
{
	GtkWidget *file;
	GtkFileFilter *filter;
	char *filename;
	int status;

	file=gtk_file_chooser_dialog_new(TO_UTF8("打开翻译文件"),
			NULL,GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
			GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	filter=gtk_file_filter_new();
	gtk_file_filter_add_mime_type(filter,"text/plain");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file),filter);

	status=gtk_dialog_run(GTK_DIALOG(file));

	switch(status)
	{
		case GTK_RESPONSE_OK:
			filename=gtk_file_chooser_get_filename(
					GTK_FILE_CHOOSER(file));

			_load_file(filename,data);
			g_free(filename);
			break;

		case GTK_RESPONSE_CANCEL:
		default:
			break;
	}

	gtk_widget_destroy(file);
}

void duoyi_preferences(GtkWidget *widget,gpointer data)
{}

void duoyi_about_dialog(GtkWidget *widget,gpointer data)
{}

void duoyi_help_dialog(GtkWidget *widget,gpointer data)
{}

void duoyi_baidu_select(GtkWidget *widget,gpointer data)
{
}

void duoyi_bing_select(GtkWidget *widget,gpointer data)
{
}

void duoyi_king_select(GtkWidget *widget,gpointer data)
{
}

void duoyi_youdao_select(GtkWidget *widget,gpointer data)
{
}

void duoyi_translate(GtkWidget *widget,gpointer data)
{}

void duoyi_reader_cleanup(GtkWidget *widget,gpointer data)
{
	GtkTextBuffer *buffer;
	GtkTextIter start,end;

	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
	gtk_text_buffer_get_bounds(buffer,&start,&end);
	gtk_text_buffer_delete(buffer,&start,&end);
}

void duoyi_error_msgbox(char *msg)
{
	GtkWidget *dialog;

	dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,
			GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
			TO_UTF8(msg),NULL);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void _load_file(char *filename,gpointer data)
{
	GtkTextBuffer *buffer;
	FILE *fp;
	char buf[64]={0};

	if((fp=fopen(filename,"rb")) == NULL)
	{
		duoyi_error_msgbox(strerror(errno));
		return;
	}

	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
	while(!feof(fp))
	{
		bzero(buf,sizeof(buf));
		fread(buf,sizeof(buf)-1,1,fp);

		gtk_text_buffer_insert_at_cursor(buffer,buf,-1);
	}

	fclose(fp);
}
