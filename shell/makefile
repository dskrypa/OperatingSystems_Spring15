CFLAGS=-Wall -g
LIBS=
OBJECTS=LinkedList.o shell.o
TARGET=shell.x
%.o: %.c
	gcc -c $(CFLAGS) -o $@ $< 
all: $(TARGET)
$(TARGET): $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS) $(LIBS)
LinkedList.o: LinkedList.h LinkedList.c
shell.o: shell.h shell.c

clean:
	rm *.o $(TARGET)
