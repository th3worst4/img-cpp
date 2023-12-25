CC = g++
CFLAGS = -m64

CFILES = main.cpp
OBJECTS = main.o
DOBJECTS = main-debug.o

all: final debug

final: $(OBJECTS)
	$(CC) $(CFLAGS) $(addprefix bin/, $(OBJECTS)) -o bin/main
	@echo "final application built successfully"
	@echo "you can find it at 'bin/main'"
	@echo " "

$(OBJECTS): %.o: src/%.cpp
	$(CC) $(CFLAGS) -o bin/$@ -c $^

debug: $(DOBJECTS)
	$(CC) $(CFLAGS) -g $(addprefix debug/, $(DOBJECTS)) -o debug/main-debug
	@echo "debug application built successfully"
	@echo "you can run gdb on 'debug/main-debug'"
	@echo " "

$(DOBJECTS): %-debug.o: src/%.cpp
	$(CC) $(CFLAGS) -g -o debug/$@ -c $^


delete:
	@echo "removing all not source files"
	@rm bin/main bin/main.o
	@rm debug/main-debug debug/main-debug.o