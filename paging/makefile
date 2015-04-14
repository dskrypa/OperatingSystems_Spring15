CFLAGS=-Wall -g
LIBS=
OBJECTS=LinkedList.o stringUtils.o paging.o
TARGET=paging
%.o: %.c
	gcc -c $(CFLAGS) -o $@ $< 
all: $(TARGET)
$(TARGET): $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS) $(LIBS)
paging.o: paging.h paging.c
LinkedList.o: LinkedList.h LinkedList.c
stringUtils.o: stringUtils.h stringUtils.c

clean:
	rm *.o $(TARGET)
