# For Linux
# Configuration stuff
CC = gcc
RM = rm -rf
MV = mv

# Add version to binary
GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --always')

# Flags
CFLAGS = -W -Wall -O2 -D__GIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS = -lX11

# Default (dir) locations
SRCDIR = src

# Required objects
OBJS =  $(SRCDIR)/main.o \
		$(SRCDIR)/display.o \
		$(SRCDIR)/args.o \
		$(SRCDIR)/signal.o

$(SRCDIR)%.o: %.c
	  $(CC) $(CFLAGS) -c -o $@ $< 
	
	  
help:
	@echo "|-----------------------------------------------------|"
	@echo "|  iKeyLog - *A KeyLogger by Roshan Lamichhane        |"
	@echo "|-----------------------------------------------------|"
	@echo
	@echo "make ikeylog     - build ikeylog"
	@echo "make clean       - clean all"
	@echo


ikeylog: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


clean:
	@echo "Removing objects ..."
	$(RM) $(SRCDIR)/*.o
	@echo "Removing binary ..."
	$(RM) ikeylog
