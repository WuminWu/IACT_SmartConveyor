#Makefile

DESTDIR?=/usr
PREFIX?=/local

DEBUG	= -O2
CC	= gcc
CFLAGS	= $(DEBUG) -Wall $(INCLUDE)
LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib
LIBS    = -lwiringPi -lwiringPiDev -lpthread

DIAG_SRC	=	diag_conveyer.c rbpIac.c
DIAG_OBJ	=	$(DIAG_SRC:.c=.o)

ELEV_SRC	=	diag_elevator.c rbpIac.c
ELEV_OBJ	=	$(ELEV_SRC:.c=.o)

all:	diag elev

diag:	$(DIAG_OBJ)
	$(CC) -o $@ $(DIAG_OBJ) $(LDFLAGS) $(LIBS)
	
elev:	$(ELEV_OBJ)
	$(CC) -o $@ $(ELEV_OBJ) $(LDFLAGS) $(LIBS)
	
clean:
	rm -f $(DIAG_OBJ) $(ELEV_OBJ) diag elev