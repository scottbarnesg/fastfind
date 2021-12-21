CC = g++
CFLAGS  = -g -Wall -std=c++17 -lmarisa
SRCDIR = src/
BUILDDIR = bin/

all:
	$(CC) $(CFLAGS) $(SRCDIR)main.cpp $(SRCDIR)parser.cpp $(SRCDIR)index.cpp $(SRCDIR)files.cpp -o $(BUILDDIR)fastfind
	$(CC) $(CFLAGS) $(SRCDIR)service.cpp $(SRCDIR)watcher.cpp -o $(BUILDDIR)fastfind-service
clean:
	$(RM) $(BUILDDIR)*
install:
	./setup.sh

