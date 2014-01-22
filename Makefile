CFLAGS=`pkg-config --cflags --libs gtk+-3.0`
JSON=`pkg-config --cflags --libs json-glib-1.0`
all:duoyi preferences

duoyi:main.o tray.o duoyi.o config.o
	gcc -o duoyi main.o tray.o duoyi.o config.o $(CFLAGS) $(JSON)

main.o:main.c tray.h
	gcc -c main.c $(CFLAGS) $(JSON)

tray.o:tray.c tray.h
	gcc -c tray.c $(CFLAGS) $(JSON)

duoyi.o:duoyi.c duoyi.h
	gcc -c duoyi.c $(CFLAGS) $(JSON)

config.o:config.c config.h
	gcc -c config.c $(JSON)

preferences:pre_ui.o preferences.o config.o
	gcc -o preferences pre_ui.o preferences.o config.o $(CFLAGS) $(JSON)

pre_ui.o:pre_ui.c preferences.h
	gcc -c pre_ui.c $(CFLAGS) $(JSON)

preferences.o:preferences.c preferences.h
	gcc -c preferences.c $(CFLAGS) $(JSON)

clean:
	rm *.o duoyi preferences
