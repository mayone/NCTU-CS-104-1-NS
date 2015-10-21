CC = gcc
CFLAGS =  -I. -g
LDFLAGS = -L. -lpthread
SOURCES = project1.c
OBJECTS = $(notdir $(SOURCES:.c=.o))
EXECUTE = magictype

all: $(OBJECTS) $(EXECUTE)

$(EXECUTE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -c

run:
	./$(EXECUTE)
clean:
	rm -rf *~ *.o $(EXECUTE) tmp
