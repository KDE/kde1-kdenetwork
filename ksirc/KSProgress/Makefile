#
# $Source$
#

#
# CFLAGS, etc export from ../Makefile
#

####### Files

HEADERS = ksprogress.h ksprogressData.h
SOURCES = \
		ksprogress.cpp \
		ksprogressdata.cpp \
		mksprogress.cpp \
		mksprogressdata.cpp


OBJECTS = \
		ksprogress.o \
		ksprogressdata.o \
		mksprogress.o \
		mksprogressdata.o

TARGET = ksprogress.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar cr $(TARGET) $(OBJECTS)

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

ksprogressdata.o: ksprogressdata.h
mksprogressdata.o: ksprogressdata.h
