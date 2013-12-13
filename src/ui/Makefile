SRCS = main.c
OBJS = $(SRCS:%.c=%.o)

GTK3_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK3_LIBS = `pkg-config --libs gtk+-3.0`
CFLAGS = -g -Wall $(GTK3_FLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

carpi_ui : $(OBJS)
	$(CC) -o $@ $(OBJS) $(GTK3_LIBS)

depend :
	$(CC) $(CFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

include depend.mak
