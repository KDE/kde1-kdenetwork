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
		speeddialog.cpp \
		mspeeddialog.cpp \
		speeddialogData.cpp \
		mspeeddialogData.cpp \
		main.cpp \
		mmain.h \


OBJECTS = \
	ksticker.o \
	mksticker.o \
	speeddialog.o \
	mspeeddialog.o \
	speeddialogData.o \
	mspeeddialogData.o \
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
speeddialog.o: speeddialog.h speeddialogData.h
mspeeddialog.o: speeddialog.h speeddialogData.h
speeddialogData.o: speeddialogData.h
mspeeddialogData.o: speeddialogData.h
main.o: /usr/include/unistd.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/posix_opt.h
main.o: /usr/include/gnu/types.h /usr/include/confname.h
main.o: /usr/include/sys/types.h /usr/include/linux/types.h
main.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
main.o: /usr/include/asm/types.h /usr/include/sys/bitypes.h ksticker.h
