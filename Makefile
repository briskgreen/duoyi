CFLAGS=`pkg-config --cflags --libs gtk+-3.0`
JSON=`pkg-config --cflags --libs json-glib-1.0`
all:duoyi preferences

duoyi:main.o tray.o duoyi.o config.o baidu.o
	gcc -o duoyi main.o tray.o duoyi.o config.o baidu.o $(CFLAGS) $(JSON) -lcurl

main.o:main.c tray.h
	gcc -c main.c $(CFLAGS) $(JSON) -lcurl

tray.o:tray.c tray.h
	gcc -c tray.c $(CFLAGS) $(JSON) -lcurl

duoyi.o:duoyi.c duoyi.h baidu.o
	gcc -c duoyi.c baidu.o $(CFLAGS) $(JSON) -lcurl

config.o:config.c config.h
	gcc -c config.c $(JSON)

baidu.o:baidu.h baidu.c
	gcc -c baidu.c $(JSON) -lcurl

preferences:pre_ui.o preferences.o config.o
	gcc -o preferences pre_ui.o preferences.o config.o $(CFLAGS) $(JSON)

pre_ui.o:pre_ui.c preferences.h
	gcc -c pre_ui.c $(CFLAGS) $(JSON)

preferences.o:preferences.c preferences.h
	gcc -c preferences.c $(CFLAGS) $(JSON)

clean:
	rm *.o duoyi preferences
