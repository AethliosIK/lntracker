CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS =
objects = main.o helpstruct.o str_buf.o hashtbl.o slist.o line.o
executable = lntracker

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

main.o: main.c helpstruct.h hashtbl.h str_buf.h slist.h line.h
helpstruct.o: helpstruct.c helpstruct.h
hashtbl.o: hashtbl.c hashtbl.h
str_buf.o: str_buf.c str_buf.h
slist.o: slist.c slist.h
line.o: line.c line.h
