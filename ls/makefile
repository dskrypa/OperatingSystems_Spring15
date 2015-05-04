CFLAGS=-Wall -g
LIBS=
OBJECTS=stringUtils.o myls.o
TARGET=myls
%.o: %.c
	gcc -c $(CFLAGS) -o $@ $< 
all: $(TARGET)
$(TARGET): $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS) $(LIBS)
myls.o: myls.h myls.c
stringUtils.o: stringUtils.h stringUtils.c

clean:
	rm *.o $(TARGET)
