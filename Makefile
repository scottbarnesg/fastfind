CC = g++
CFLAGS  = -g -Wall -std=c++17 -lmarisa
SRCDIR = src/
BUILDDIR = bin/
DEBUGGER = gdb

all:
	$(CC) $(CFLAGS) $(SRCDIR)main.cpp $(SRCDIR)parser.cpp $(SRCDIR)index.cpp $(SRCDIR)files.cpp -o $(BUILDDIR)fastfind
	$(CC) $(CFLAGS) $(SRCDIR)service.cpp $(SRCDIR)watcher.cpp $(SRCDIR)parser.cpp $(SRCDIR)index.cpp $(SRCDIR)files.cpp -o $(BUILDDIR)fastfind-service
clean:
	$(RM) $(BUILDDIR)*
install:
	./setup.sh
debug-cli:  # type r to run in debugger
	$(DEBUGGER) $(BUILDDIR)fastfind
debug-service:
	$(DEBUGGER) $(BUILDDIR)fastfind-service