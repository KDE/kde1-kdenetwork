#
# $Source$
#

#
# CFLAGS, etc export from ../Makefile
#

####### Files

HEADERS = ksticker.h
SOURCES = \
		ksticker.cpp \
		mksticker.cpp \
		main.cpp \
		mmain.h \


OBJECTS = \
	ksticker.o \
	mksticker.o \
	main.o \
	mmain.o

TARGET = ksticker.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(TARGET) main

$(TARGET): $(OBJECTS)
	ar cr $(TARGET) $(OBJECTS)

main: $(OBJECTS)
	$(CC) $(OBJECTS) -o main $(LFLAGS) -lm

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(HEADERS) $(SOURCES) $(OBJECTS) Makefile

clean:
	rm -f *.o *.bak *~ *% core *.orig log.flood
	rm -f $(TARGET) ksirc-static

####### Meta classes

#$(OBJMETA): $(SRCMETA)
#	$(MOC) -o $@ $<
m%.cpp: %.h
	moc $< -o $@

# DO NOT DELETE

mksticker.o: ksticker.h
main.o: ksticker.h
