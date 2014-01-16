CFLAGS=`pkg-config --cflags --libs gtk+-3.0`
all:duoyi

duoyi:main.o tray.o duoyi.o
	gcc -o duoyi main.o tray.o duoyi.o $(CFLAGS)

main.o:main.c tray.h
	gcc -c main.c $(CFLAGS)

tray.o:tray.c tray.h
	gcc -c tray.c $(CFLAGS)

duoyi.o:duoyi.c duoyi.h
	gcc -c duoyi.c $(CFLAGS)

clean:
	rm *.o duoyi
