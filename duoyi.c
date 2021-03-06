#include "duoyi.h"

/*词典被选择时的第一个操作
 * 判断当前选择的词典API Key是否已经设置
 */
#define duoyi_if_select(msg) \
{\
	if(select->api[select->select] == NULL)\
	{\
		duoyi_error_msgbox(msg);\
		return;\
	}\
}

/*翻译所支持的语言列表
 * 这些变量在相应词典函数文件中定义
 */
extern char *baidu_tran[];
extern char *baidu_code[];
extern char *bing_tran[];
extern char *bing_code[];

/*加载显示文件*/
void _load_file(char *filename,gpointer data);

void duoyi_quit(GtkWidget *widget,gpointer data)
{
	GtkWidget *dialog;
	int status;

	dialog=gtk_message_dialog_new(NULL,
			GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_OK_CANCEL,
			"您确定要退出本程序吗？\n按下取消键继续本程序",NULL);
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog),"img/64x64/quit.png",NULL);

	status=gtk_dialog_run(GTK_DIALOG(dialog));

	/*OK退出
	 * NO继续本程序
	 */
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
	/*根据主界面状态判断是否为点击状态栏图标，是则隐藏主界面*/
	if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && event->new_window_state == GDK_WINDOW_STATE_ICONIFIED)
		gtk_widget_hide(widget);
}

void duoyi_hide_window(GtkWidget *widget,TrayData *data)
{
	/*隐藏主界面并设置状态栏菜单中按钮的可用与否*/
	gtk_widget_hide(data->widget);

	gtk_widget_set_sensitive(widget,FALSE);
	gtk_widget_set_sensitive(data->item,TRUE);
}

void duoyi_read_from_file(GtkWidget *widget,gpointer data)
{
	GtkWidget *file;
	GtkFileFilter *filter;
	char *filename;
	int status;

	file=gtk_file_chooser_dialog_new("打开翻译文件",
			NULL,GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
			GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);

	gtk_window_set_icon_from_file(GTK_WINDOW(file),
			"img/64x64/file_load.png",NULL);
	filter=gtk_file_filter_new();
	/*添加文件类型，只支持文本文件*/
	gtk_file_filter_add_mime_type(filter,"text/plain");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file),filter);

	status=gtk_dialog_run(GTK_DIALOG(file));

	/*OK 加载文件
	 * CANCEL 取消选择
	 */
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
{
	/*首选项，该界面为新的程序*/
#ifdef _WIN32
	if(spawnl(P_NOWAIT,"preferences.exe","preferences",NULL) == -1)
		duoyi_error_msgbox(strerror(errno));
#else
	if(fork() == 0)
	{
		if(execl("./preferences","preferences",NULL) == -1)
			duoyi_error_msgbox(strerror(errno));
	}
#endif
}

void duoyi_about_dialog(GtkWidget *widget,gpointer data)
{
	GtkWidget *dialog;
	GdkPixbuf *pixbuf;
	char *license;
	const char *authors[]={"Brisk <briskgreen@163.com>",NULL};
	FILE *fp;
	long len;

	dialog=gtk_about_dialog_new();
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog),
			"img/64x64/about.png",NULL);
	pixbuf=gdk_pixbuf_new_from_file("img/64x64/yi.png",NULL);
	/*加载许可协议，本程序使用LGPL协议*/
	if((fp=fopen("LICENSE","rb")) == NULL)
		license=NULL;
	else
	{
		fseek(fp,0L,SEEK_END);
		len=ftell(fp);
		rewind(fp);

		license=malloc(sizeof(char)*len+1);
		fread(license,len,1,fp);
		fclose(fp);
		license[len]='\0';
	}

	/*设置logo*/
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog),pixbuf);
	/*设置名称*/
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),
			"多译");
	/*设置版本号*/
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),"1.0");
	/*设置描述*/
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
			"多语言网络翻译词典");
	/*设置协议*/
	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog),license);
	/*设置Copyright*/
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
			"Copyright ©  2014 By 炕头哥");
	/*设置联系方式*/
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog),
			"E-mail: briskgreen@163.com");
	/*设置作者,只有我一人*/
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog),authors);

	gtk_dialog_run(GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);
	if(license)
		free(license);
}

void duoyi_help_dialog(GtkWidget *widget,gpointer data)
{
	GtkWidget *dialog;
	GtkWidget *help;
	GtkWidget *area;
	GtkTextBuffer *buffer;
	GtkWidget *scrolled;
	long len;
	char *buf;
	FILE *fp;

	if((fp=fopen("readme","rb")) == NULL)
		return;

	/*加载帮助文件*/
	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	rewind(fp);
	buf=malloc(sizeof(char)*len+1);
	fread(buf,len,1,fp);
	buf[len]='\0';
	fclose(fp);

	dialog=gtk_dialog_new_with_buttons("帮助",NULL,GTK_DIALOG_MODAL,
			GTK_STOCK_OK,GTK_RESPONSE_ACCEPT,NULL);

	gtk_window_set_icon_from_file(GTK_WINDOW(dialog),
			"img/64x64/help.png",NULL);
	/*得到GtkDialog的一个GtkBox构件*/
	area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	help=gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(help),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(help),GTK_WRAP_CHAR);

	/*添加滚动条并设置自动滚动*/
	scrolled=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
			GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(scrolled,0x200,0x150);

	gtk_container_add(GTK_CONTAINER(scrolled),help);
	gtk_container_add(GTK_CONTAINER(area),scrolled);
	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(help));
	
	gtk_text_buffer_set_text(buffer,buf,-1);

	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	free(buf);
}

void duoyi_baidu_select(GtkWidget *widget,gpointer data)
{
	SelectionData *select=(SelectionData *)data;
	int i;

	/*设置当前词典*/
	select->select=0;
	duoyi_if_select("您没有设置百度API\n请于配置中设置相应API");
	//gdk_threads_enter();
	/*设置语言选择可用*/
	gtk_widget_set_sensitive(select->from,TRUE);
	gtk_widget_set_sensitive(select->to,TRUE);
	//gdk_threads_leave();

	//gdk_threads_enter();
	/*清空语言列表*/
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->from));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->to));
	/*添加语言支持列表*/
	for(i=0;baidu_tran[i] != NULL;++i)
	{
		gtk_combo_box_text_append_text(
				GTK_COMBO_BOX_TEXT(select->from),baidu_tran[i]);
		gtk_combo_box_text_append_text(
				GTK_COMBO_BOX_TEXT(select->to),baidu_tran[i]);
	}
	/*设置默认语言*/
	gtk_combo_box_set_active(GTK_COMBO_BOX(select->from),0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(select->to),0);
	//gdk_threads_leave();
}

void duoyi_bing_select(GtkWidget *widget,gpointer data)
{
	SelectionData *select=(SelectionData *)data;
	int i;

	/*与上同*/
	select->select=1;
	duoyi_if_select("您没有设置必应API\n请于配置中设置相应API");
	gtk_widget_set_sensitive(select->from,TRUE);
	gtk_widget_set_sensitive(select->to,TRUE);
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->from));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->to));
	for(i=0;bing_tran[i] != NULL;++i)
	{
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(select->from),bing_tran[i]);
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(select->to),bing_tran[i]);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(select->from),8);
	gtk_combo_box_set_active(GTK_COMBO_BOX(select->to),3);
}

void duoyi_king_select(GtkWidget *widget,gpointer data)
{
	SelectionData *select=(SelectionData *)data;

	/*与上同
	 *由于不支持多国语言翻译
	 所以语言选择设置不可用
	 */
	select->select=2;
	duoyi_if_select("您没有设置金山词霸API\n请于配置中设置相应API");
	gtk_widget_set_sensitive(select->from,FALSE);
	gtk_widget_set_sensitive(select->to,FALSE);
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->from));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->to));
}

void duoyi_youdao_select(GtkWidget *widget,gpointer data)
{
	SelectionData *select=(SelectionData *)data;

	/*与上同与上同*/
	select->select=3;
	duoyi_if_select("您没有设置有道API\n请于配置中设置相应API");
	gtk_widget_set_sensitive(select->from,FALSE);
	gtk_widget_set_sensitive(select->to,FALSE);
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->from));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(select->to));
}

void duoyi_translate(GtkWidget *widget,gpointer data)
{
	TranData *tran=(TranData *)data;
	GtkTextBuffer *reader;
	GtkTextBuffer *displayer;
	GtkTextIter start;
	GtkTextIter end;
	char *res;
	int from;
	int to;

	reader=gtk_text_view_get_buffer(GTK_TEXT_VIEW(tran->reader));
	/*得到输入内容构件的起始与结尾迭代器*/
	gtk_text_buffer_get_bounds(reader,&start,&end);
	displayer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(tran->displayer));
	/*清空显示*/
	duoyi_reader_cleanup(NULL,tran->displayer);

	switch(tran->data->select)
	{
		case 0: //选择了百度词典
			res=baidu_translate(
			baidu_code[gtk_combo_box_get_active(GTK_COMBO_BOX(tran->data->from))],
			baidu_code[gtk_combo_box_get_active(GTK_COMBO_BOX(tran->data->to))],
			tran->data->api[0],
			gtk_text_buffer_get_text(reader,&start,&end,FALSE));

			/*将返回的结果显示出来*/
			gtk_text_buffer_set_text(displayer,res,-1);
			free(res);
			break;
		case 1: //选择上必应词典
			res=bing_translate(
			bing_code[gtk_combo_box_get_active(GTK_COMBO_BOX(tran->data->from))],
			bing_code[gtk_combo_box_get_active(GTK_COMBO_BOX(tran->data->to))],
			tran->data->api[1],
			gtk_text_buffer_get_text(reader,&start,&end,FALSE));

			gtk_text_buffer_set_text(displayer,res,-1);
			free(res);
			break;
		case 2: //选择了金山词霸
			res=king_translate(tran->data->api[2],
			gtk_text_buffer_get_text(reader,&start,&end,FALSE));
			gtk_text_buffer_set_text(displayer,res,-1);
			free(res);
			break;
		case 3: //选择了有道词典
			res=youdao_translate(tran->data->api[3],
			gtk_text_buffer_get_text(reader,&start,&end,FALSE));
			gtk_text_buffer_set_text(displayer,res,-1);
			free(res);
			break;
	}
}

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
			msg,NULL);
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog),
			"img/64x64/message.png",NULL);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void _load_file(char *filename,gpointer data)
{
	GtkTextBuffer *buffer;
	FILE *fp;
	long len;
	char *buf;

	if((fp=fopen(filename,"rb")) == NULL)
	{
		duoyi_error_msgbox(strerror(errno));
		return;
	}

	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	rewind(fp);
	buf=malloc(len+1);
	fread(buf,len,1,fp);
	buf[len]='\0';
	fclose(fp);

	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
	gtk_text_buffer_set_text(buffer,buf,-1);

	free(buf);
}
