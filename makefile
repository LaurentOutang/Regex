COMPILER= g++
CFLAGS= -Wall
EXEC= prog

SRC=$(wildcard *.cpp)

OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(COMPILER) -o $@ $^

main.o: main.cpp Regex.h

Regex.o: Regex.cpp NFA.h

%.o: %.c
	$(COMPILER) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)
   