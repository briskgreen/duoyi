/*创建状态栏显示的头文件*/
#ifndef _TRAY_H
#define _TRAY_H

#include "duoyi.h"

/*用于设置当前是否可显示的数据结构
 * show指向菜单显示
 * hide指向菜单隐藏
 */
typedef struct
{
	TrayData *show;
	TrayData *hide;
}Tray;

/*创建状态栏显示主函数*/
void tray_create(GtkWidget *widget);

/*点击状态栏图标的回调函数*/
void tray_on_clicked_with_tray(GtkWidget *widget,Tray *data);

/*点击状态栏图标的回调函数*/
void tray_on_clicked(GtkWidget *widget,TrayData *data);

/*状态栏图标右键菜单回调函数*/
void tray_on_menu(GtkStatusIcon *tray,guint button,
		guint activate_item,gpointer data);

#endif
