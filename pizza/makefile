CFLAGS=-Wall -g
LIBS=
OBJECTS=pizza.o
TARGET=pizza
%.o: %.c
	gcc -c $(CFLAGS) -o $@ $< 
all: $(TARGET)
$(TARGET): $(OBJECTS)
	gcc -pthread -o $(TARGET) $(OBJECTS) $(LIBS)
pizza.o: pizza.h pizza.c

clean:
	rm *.o $(TARGET)
