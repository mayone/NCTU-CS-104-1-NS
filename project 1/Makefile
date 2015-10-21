CC = gcc
CFLAGS =  -I. -g
LDFLAGS = -L.
SOURCES = magicType.c
OBJECTS = $(notdir $(SOURCES:.c=.o))
EXECUTE = magicType

all: $(OBJECTS) $(EXECUTE)

$(EXECUTE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -c

run:
	./$(EXECUTE)
clean:
	rm -rf *~ *.o $(EXECUTE)
