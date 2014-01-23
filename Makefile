CFLAGS=`pkg-config --cflags --libs gtk+-3.0`
JSON=`pkg-config --cflags --libs json-glib-1.0`
XML=`pkg-config --cflags --libs libxml-2.0`
all:duoyi preferences

duoyi:main.o tray.o duoyi.o config.o baidu.o bing.o king.o youdao.o tool.o
	gcc -o duoyi main.o tray.o duoyi.o config.o baidu.o bing.o king.o youdao.o tool.o $(CFLAGS) $(JSON) $(XML) -lcurl

main.o:main.c tray.h
	gcc -c main.c $(CFLAGS) $(JSON) $(XML) -lcurl

tray.o:tray.c tray.h
	gcc -c tray.c $(CFLAGS) $(JSON) $(XML) -lcurl

duoyi.o:duoyi.c duoyi.h
	gcc -c duoyi.c $(CFLAGS) $(JSON) $(XML) -lcurl

config.o:config.c config.h
	gcc -c config.c $(JSON)

baidu.o:baidu.h baidu.c tool.o
	gcc -c baidu.c tool.o $(JSON) -lcurl

bing.o:bing.h bing.c tool.o
	gcc -c bing.c tool.o $(JSON) -lcurl

king.o:king.h king.c
	gcc -c king.c $(XML) -lcurl

youdao.o:youdao.h youdao.c
	gcc -c youdao.c $(JSON) -lcurl

tool.o:tool.h tool.c
	gcc -c tool.c

preferences:pre_ui.o preferences.o config.o
	gcc -o preferences pre_ui.o preferences.o config.o $(CFLAGS) $(JSON)

pre_ui.o:pre_ui.c preferences.h
	gcc -c pre_ui.c $(CFLAGS) $(JSON)

preferences.o:preferences.c preferences.h
	gcc -c preferences.c $(CFLAGS) $(JSON)

clean:
	rm *.o duoyi preferences

install:
	./install.sh

uninstall:
	./uninstall.sh
