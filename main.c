#include "tray.h"

int main(int argc,char **argv)
{
	GtkWidget *win;

	gtk_init(&argc,&argv);

	win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),TO_UTF8("多译"));
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file(GTK_WINDOW(win),"img/64x64/yi.png",NULL);
	g_signal_connect(G_OBJECT(win),"delete-event",
			G_CALLBACK(duoyi_quit),NULL);

	tray_create(win);

	gtk_widget_show_all(win);
	gtk_main();

	return 0;
}
