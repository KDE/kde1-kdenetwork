####### This section was automatically generated from
#######    /usr/local/lib/qt-1.31/Makefile for building on
#######    linux-gcc-shared on Sun Oct 12 09:16:39 MDT 1997

INCDIR = /usr/local/lib/qt/include
CFLAGS = -g -Wall -I/usr/X11R6/include -I$(HOME)/include -I/opt/kde/include
LIBCFLAGS = -fPIC
YACCCFLAGS = -Wno-unused -Wno-parentheses
LFLAGS = -g -L/usr/local/lib/qt/lib -lqt -L/opt/kde/lib -lkdecore -lkdeui -L/usr/X11/lib -lXext
CC = g++
MOC = /usr/local/lib/qt/bin/moc
SHELL =	/bin/sh

####### End of automatically generated section
#
# $Source$
#

####### Files

HEADERS = toplevel.h irclistitem.h ahistlineedit.h #ahtmlview.h
SOURCES = toplevel.cpp \
ksirc.cpp \
irclistbox.cpp \
mirclistbox.cpp \
irclistitem.cpp \
mirclistitem.cpp \
alistbox.cpp \
mtoplevel.cpp \
ahistlineedit.cpp mahistlineedit.cpp \
malistbox.cpp \
UserDataRef.cpp UserMenuRefData.cpp mUserDataRef.cpp mUserMenuRefData.cpp \
iocontroller.cpp miocontroller.cpp \
open_top.cpp open_topData.cpp mopen_top.cpp mopen_topData.cpp \
messageReceiver.cpp \
ioBroadcast.cpp \
ksircprocess.cpp mksircprocess.cpp \
open_ksirc.cpp open_ksircData.cpp mopen_ksirc.cpp mopen_ksircData.cpp \
servercontroller.cpp servercontrollerData.cpp mservercontroller.cpp mservercontrollerData.cpp \
control.cpp mcontrol.cpp \
KSircColour.cpp KSircColourData.cpp mKSircColour.cpp mKSircColourData.cpp \
FilterRuleEditor.cpp FilterRuleEditorData.cpp mFilterRuleEditor.cpp mFilterRuleEditorData.cpp

OBJECTS = \
toplevel.o \
ksirc.o \
irclistbox.o \
mirclistbox.o \
irclistitem.o \
mirclistitem.o \
ahistlineedit.o \
alistbox.o \
mtoplevel.o \
malistbox.o \
UserMenuRef.o UserMenuRefData.o mUserMenuRef.o mUserMenuRefData.o \
iocontroller.o miocontroller.o \
open_top.o open_topData.o mopen_top.o mopen_topData.o \
messageReceiver.o \
ioBroadcast.o \
ksircprocess.o mksircprocess.o \
open_ksirc.o open_ksircData.o mopen_ksirc.o mopen_ksircData.o \
servercontroller.o servercontrollerData.o \
mservercontroller.o mservercontrollerData.o \
control.o mcontrol.o \
mahistlineedit.o \
KSircColour.o KSircColourData.o mKSircColour.o mKSircColourData.o \
FilterRuleEditor.o FilterRuleEditorData.o mFilterRuleEditor.o mFilterRuleEditorData.o

TARGET = ksirc

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(TARGET)
static: $(OBJECTS)
	$(CC) -static $(OBJECTS) -o ksirc-static -g -L/usr/local/lib/qt/lib -L/opt/kde/lib -lkdeui -lkdecore -lm  -L/usr/X11R6/lib -lqt -lX11 -lXext

hstatic: $(OBJECTS)
	$(CC) $(OBJECTS) -o ksirc-hstatic -g -L/usr/local/lib/qt/lib \
-L/opt/kde/lib /opt/kde/lib/libkdeui.a /opt/kde/lib/libkdecore.a \
-lm  -L/usr/X11R6/lib /usr/local/lib/qt/lib/libqt.a -lX11 -lXext

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS) -lm

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

toplevel.o: toplevel.h /usr/include/unistd.h /usr/include/features.h
toplevel.o: /usr/include/sys/cdefs.h /usr/include/posix_opt.h
toplevel.o: /usr/include/gnu/types.h /usr/include/confname.h
toplevel.o: /usr/include/sys/types.h /usr/include/linux/types.h
toplevel.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
toplevel.o: /usr/include/asm/types.h /usr/include/sys/bitypes.h
toplevel.o: /usr/include/signal.h /usr/include/linux/signal.h
toplevel.o: /usr/include/asm/signal.h /usr/local/lib/qt/include/qwidget.h
toplevel.o: /usr/local/lib/qt/include/qwindefs.h
toplevel.o: /usr/local/lib/qt/include/qobjdefs.h
toplevel.o: /usr/local/lib/qt/include/qglobal.h
toplevel.o: /usr/local/lib/qt/include/qobject.h
toplevel.o: /usr/local/lib/qt/include/qstring.h
toplevel.o: /usr/local/lib/qt/include/qarray.h
toplevel.o: /usr/local/lib/qt/include/qgarray.h
toplevel.o: /usr/local/lib/qt/include/qshared.h
toplevel.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
toplevel.o: /usr/local/lib/qt/include/qevent.h
toplevel.o: /usr/local/lib/qt/include/qrect.h
toplevel.o: /usr/local/lib/qt/include/qsize.h
toplevel.o: /usr/local/lib/qt/include/qpoint.h
toplevel.o: /usr/local/lib/qt/include/qpaintd.h
toplevel.o: /usr/local/lib/qt/include/qpalette.h
toplevel.o: /usr/local/lib/qt/include/qcolor.h
toplevel.o: /usr/local/lib/qt/include/qcursor.h
toplevel.o: /usr/local/lib/qt/include/qfont.h
toplevel.o: /usr/local/lib/qt/include/qfontmet.h
toplevel.o: /usr/local/lib/qt/include/qfontinf.h
toplevel.o: /usr/local/lib/qt/include/qsocknot.h
toplevel.o: /usr/local/lib/qt/include/qlined.h
toplevel.o: /usr/local/lib/qt/include/qmlined.h
toplevel.o: /usr/local/lib/qt/include/qtablevw.h
toplevel.o: /usr/local/lib/qt/include/qframe.h
toplevel.o: /usr/local/lib/qt/include/qlist.h
toplevel.o: /usr/local/lib/qt/include/qglist.h
toplevel.o: /usr/local/lib/qt/include/qcollect.h
toplevel.o: /usr/local/lib/qt/include/qlayout.h
toplevel.o: /usr/local/lib/qt/include/qgmanagr.h
toplevel.o: /usr/local/lib/qt/include/qintdict.h
toplevel.o: /usr/local/lib/qt/include/qgdict.h
toplevel.o: /usr/local/lib/qt/include/qlistbox.h
toplevel.o: /usr/local/lib/qt/include/qpixmap.h
toplevel.o: /usr/local/lib/qt/include/qscrbar.h
toplevel.o: /usr/local/lib/qt/include/qrangect.h
toplevel.o: /usr/local/lib/qt/include/qdrawutl.h
toplevel.o: /usr/local/lib/qt/include/qpainter.h
toplevel.o: /usr/local/lib/qt/include/qregion.h
toplevel.o: /usr/local/lib/qt/include/qpen.h
toplevel.o: /usr/local/lib/qt/include/qbrush.h
toplevel.o: /usr/local/lib/qt/include/qpntarry.h
toplevel.o: /usr/local/lib/qt/include/qwmatrix.h
toplevel.o: /usr/local/lib/qt/include/qregexp.h
toplevel.o: /usr/local/lib/qt/include/qaccel.h
toplevel.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
toplevel.o: /usr/local/lib/qt/include/qstrlist.h
toplevel.o: /usr/local/lib/qt/include/qdstream.h
toplevel.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h alistbox.h
toplevel.o: messageReceiver.h ksircprocess.h iocontroller.h
toplevel.o: /usr/local/lib/qt/include/qdict.h UserMenuRef.h UserMenuRefData.h
toplevel.o: /usr/local/lib/qt/include/qchkbox.h
toplevel.o: /usr/local/lib/qt/include/qbutton.h
toplevel.o: /usr/local/lib/qt/include/qpushbt.h
toplevel.o: /usr/local/lib/qt/include/qcombo.h irclistbox.h open_top.h
toplevel.o: open_topData.h /usr/local/lib/qt/include/qdialog.h
toplevel.o: control_message.h config.h /usr/include/termios.h
toplevel.o: /usr/include/linux/termios.h /usr/include/asm/termios.h
toplevel.o: /usr/include/asm/termbits.h /usr/include/asm/ioctls.h
toplevel.o: /usr/include/asm/ioctl.h
ksirc.o: servercontroller.h /usr/local/lib/qt/include/qdict.h
ksirc.o: /usr/local/lib/qt/include/qgdict.h
ksirc.o: /usr/local/lib/qt/include/qcollect.h
ksirc.o: /usr/local/lib/qt/include/qglobal.h
ksirc.o: /usr/local/lib/qt/include/qgeneric.h servercontrollerData.h
ksirc.o: control.h /usr/local/lib/qt/include/qframe.h
ksirc.o: /usr/local/lib/qt/include/qwidget.h
ksirc.o: /usr/local/lib/qt/include/qwindefs.h
ksirc.o: /usr/local/lib/qt/include/qobjdefs.h
ksirc.o: /usr/local/lib/qt/include/qobject.h
ksirc.o: /usr/local/lib/qt/include/qstring.h
ksirc.o: /usr/local/lib/qt/include/qarray.h
ksirc.o: /usr/local/lib/qt/include/qgarray.h
ksirc.o: /usr/local/lib/qt/include/qshared.h /usr/include/string.h
ksirc.o: /usr/include/features.h /usr/include/sys/cdefs.h
ksirc.o: /usr/local/lib/qt/include/qevent.h /usr/local/lib/qt/include/qrect.h
ksirc.o: /usr/local/lib/qt/include/qsize.h /usr/local/lib/qt/include/qpoint.h
ksirc.o: /usr/local/lib/qt/include/qpaintd.h
ksirc.o: /usr/local/lib/qt/include/qpalette.h
ksirc.o: /usr/local/lib/qt/include/qcolor.h
ksirc.o: /usr/local/lib/qt/include/qcursor.h
ksirc.o: /usr/local/lib/qt/include/qfont.h
ksirc.o: /usr/local/lib/qt/include/qfontmet.h
ksirc.o: /usr/local/lib/qt/include/qfontinf.h ksircprocess.h iocontroller.h
ksirc.o: /usr/local/lib/qt/include/qsocknot.h messageReceiver.h
ksirc.o: /usr/include/termios.h /usr/include/sys/types.h
ksirc.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
ksirc.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
ksirc.o: /usr/include/sys/bitypes.h /usr/include/linux/termios.h
ksirc.o: /usr/include/asm/termios.h /usr/include/asm/termbits.h
ksirc.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
ksirc.o: /usr/include/unistd.h /usr/include/posix_opt.h
ksirc.o: /usr/include/gnu/types.h /usr/include/confname.h
ksirc.o: /usr/include/fcntl.h /usr/include/linux/fcntl.h
ksirc.o: /usr/include/asm/fcntl.h config.h
irclistbox.o: irclistbox.h /usr/local/lib/qt/include/qobject.h
irclistbox.o: /usr/local/lib/qt/include/qobjdefs.h
irclistbox.o: /usr/local/lib/qt/include/qglobal.h
irclistbox.o: /usr/local/lib/qt/include/qstring.h
irclistbox.o: /usr/local/lib/qt/include/qarray.h
irclistbox.o: /usr/local/lib/qt/include/qgarray.h
irclistbox.o: /usr/local/lib/qt/include/qshared.h
irclistbox.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
irclistbox.o: /usr/include/features.h /usr/include/sys/cdefs.h
irclistbox.o: /usr/local/lib/qt/include/qevent.h
irclistbox.o: /usr/local/lib/qt/include/qwindefs.h
irclistbox.o: /usr/local/lib/qt/include/qrect.h
irclistbox.o: /usr/local/lib/qt/include/qsize.h
irclistbox.o: /usr/local/lib/qt/include/qpoint.h
irclistbox.o: /usr/local/lib/qt/include/qlistbox.h
irclistbox.o: /usr/local/lib/qt/include/qtablevw.h
irclistbox.o: /usr/local/lib/qt/include/qframe.h
irclistbox.o: /usr/local/lib/qt/include/qwidget.h
irclistbox.o: /usr/local/lib/qt/include/qpaintd.h
irclistbox.o: /usr/local/lib/qt/include/qpalette.h
irclistbox.o: /usr/local/lib/qt/include/qcolor.h
irclistbox.o: /usr/local/lib/qt/include/qcursor.h
irclistbox.o: /usr/local/lib/qt/include/qfont.h
irclistbox.o: /usr/local/lib/qt/include/qfontmet.h
irclistbox.o: /usr/local/lib/qt/include/qfontinf.h
irclistbox.o: /usr/local/lib/qt/include/qpixmap.h
irclistbox.o: /usr/local/lib/qt/include/qscrbar.h
irclistbox.o: /usr/local/lib/qt/include/qrangect.h
irclistbox.o: /usr/local/lib/qt/include/qdrawutl.h
irclistbox.o: /usr/local/lib/qt/include/qpainter.h
irclistbox.o: /usr/local/lib/qt/include/qregion.h
irclistbox.o: /usr/local/lib/qt/include/qpen.h
irclistbox.o: /usr/local/lib/qt/include/qbrush.h
irclistbox.o: /usr/local/lib/qt/include/qpntarry.h
irclistbox.o: /usr/local/lib/qt/include/qwmatrix.h
irclistitem.o: irclistitem.h /usr/local/lib/qt/include/qobject.h
irclistitem.o: /usr/local/lib/qt/include/qobjdefs.h
irclistitem.o: /usr/local/lib/qt/include/qglobal.h
irclistitem.o: /usr/local/lib/qt/include/qstring.h
irclistitem.o: /usr/local/lib/qt/include/qarray.h
irclistitem.o: /usr/local/lib/qt/include/qgarray.h
irclistitem.o: /usr/local/lib/qt/include/qshared.h
irclistitem.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
irclistitem.o: /usr/include/features.h /usr/include/sys/cdefs.h
irclistitem.o: /usr/local/lib/qt/include/qevent.h
irclistitem.o: /usr/local/lib/qt/include/qwindefs.h
irclistitem.o: /usr/local/lib/qt/include/qrect.h
irclistitem.o: /usr/local/lib/qt/include/qsize.h
irclistitem.o: /usr/local/lib/qt/include/qpoint.h
irclistitem.o: /usr/local/lib/qt/include/qcolor.h
irclistitem.o: /usr/local/lib/qt/include/qlistbox.h
irclistitem.o: /usr/local/lib/qt/include/qtablevw.h
irclistitem.o: /usr/local/lib/qt/include/qframe.h
irclistitem.o: /usr/local/lib/qt/include/qwidget.h
irclistitem.o: /usr/local/lib/qt/include/qpaintd.h
irclistitem.o: /usr/local/lib/qt/include/qpalette.h
irclistitem.o: /usr/local/lib/qt/include/qcursor.h
irclistitem.o: /usr/local/lib/qt/include/qfont.h
irclistitem.o: /usr/local/lib/qt/include/qfontmet.h
irclistitem.o: /usr/local/lib/qt/include/qfontinf.h
irclistitem.o: /usr/local/lib/qt/include/qpixmap.h
irclistitem.o: /usr/local/lib/qt/include/qpainter.h
irclistitem.o: /usr/local/lib/qt/include/qregion.h
irclistitem.o: /usr/local/lib/qt/include/qpen.h
irclistitem.o: /usr/local/lib/qt/include/qbrush.h
irclistitem.o: /usr/local/lib/qt/include/qpntarry.h
irclistitem.o: /usr/local/lib/qt/include/qwmatrix.h
irclistitem.o: /usr/local/lib/qt/include/qstrlist.h
irclistitem.o: /usr/local/lib/qt/include/qlist.h
irclistitem.o: /usr/local/lib/qt/include/qglist.h
irclistitem.o: /usr/local/lib/qt/include/qcollect.h
irclistitem.o: /usr/local/lib/qt/include/qdstream.h
irclistitem.o: /usr/local/lib/qt/include/qiodev.h
alistbox.o: alistbox.h /usr/local/lib/qt/include/qobject.h
alistbox.o: /usr/local/lib/qt/include/qobjdefs.h
alistbox.o: /usr/local/lib/qt/include/qglobal.h
alistbox.o: /usr/local/lib/qt/include/qstring.h
alistbox.o: /usr/local/lib/qt/include/qarray.h
alistbox.o: /usr/local/lib/qt/include/qgarray.h
alistbox.o: /usr/local/lib/qt/include/qshared.h
alistbox.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
alistbox.o: /usr/include/features.h /usr/include/sys/cdefs.h
alistbox.o: /usr/local/lib/qt/include/qevent.h
alistbox.o: /usr/local/lib/qt/include/qwindefs.h
alistbox.o: /usr/local/lib/qt/include/qrect.h
alistbox.o: /usr/local/lib/qt/include/qsize.h
alistbox.o: /usr/local/lib/qt/include/qpoint.h
alistbox.o: /usr/local/lib/qt/include/qwidget.h
alistbox.o: /usr/local/lib/qt/include/qpaintd.h
alistbox.o: /usr/local/lib/qt/include/qpalette.h
alistbox.o: /usr/local/lib/qt/include/qcolor.h
alistbox.o: /usr/local/lib/qt/include/qcursor.h
alistbox.o: /usr/local/lib/qt/include/qfont.h
alistbox.o: /usr/local/lib/qt/include/qfontmet.h
alistbox.o: /usr/local/lib/qt/include/qfontinf.h
alistbox.o: /usr/local/lib/qt/include/qlistbox.h
alistbox.o: /usr/local/lib/qt/include/qtablevw.h
alistbox.o: /usr/local/lib/qt/include/qframe.h
alistbox.o: /usr/local/lib/qt/include/qpixmap.h
ahistlineedit.o: ahistlineedit.h /usr/local/lib/qt/include/qlined.h
ahistlineedit.o: /usr/local/lib/qt/include/qwidget.h
ahistlineedit.o: /usr/local/lib/qt/include/qwindefs.h
ahistlineedit.o: /usr/local/lib/qt/include/qobjdefs.h
ahistlineedit.o: /usr/local/lib/qt/include/qglobal.h
ahistlineedit.o: /usr/local/lib/qt/include/qobject.h
ahistlineedit.o: /usr/local/lib/qt/include/qstring.h
ahistlineedit.o: /usr/local/lib/qt/include/qarray.h
ahistlineedit.o: /usr/local/lib/qt/include/qgarray.h
ahistlineedit.o: /usr/local/lib/qt/include/qshared.h
ahistlineedit.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
ahistlineedit.o: /usr/include/features.h /usr/include/sys/cdefs.h
ahistlineedit.o: /usr/local/lib/qt/include/qevent.h
ahistlineedit.o: /usr/local/lib/qt/include/qrect.h
ahistlineedit.o: /usr/local/lib/qt/include/qsize.h
ahistlineedit.o: /usr/local/lib/qt/include/qpoint.h
ahistlineedit.o: /usr/local/lib/qt/include/qpaintd.h
ahistlineedit.o: /usr/local/lib/qt/include/qpalette.h
ahistlineedit.o: /usr/local/lib/qt/include/qcolor.h
ahistlineedit.o: /usr/local/lib/qt/include/qcursor.h
ahistlineedit.o: /usr/local/lib/qt/include/qfont.h
ahistlineedit.o: /usr/local/lib/qt/include/qfontmet.h
ahistlineedit.o: /usr/local/lib/qt/include/qfontinf.h
ahistlineedit.o: /usr/local/lib/qt/include/qstrlist.h
ahistlineedit.o: /usr/local/lib/qt/include/qlist.h
ahistlineedit.o: /usr/local/lib/qt/include/qglist.h
ahistlineedit.o: /usr/local/lib/qt/include/qcollect.h
ahistlineedit.o: /usr/local/lib/qt/include/qdstream.h
ahistlineedit.o: /usr/local/lib/qt/include/qiodev.h
ahistlineedit.o: /usr/local/lib/qt/include/qkeycode.h
UserMenuRefData.o: UserMenuRefData.h /usr/local/lib/qt/include/qframe.h
UserMenuRefData.o: /usr/local/lib/qt/include/qwidget.h
UserMenuRefData.o: /usr/local/lib/qt/include/qwindefs.h
UserMenuRefData.o: /usr/local/lib/qt/include/qobjdefs.h
UserMenuRefData.o: /usr/local/lib/qt/include/qglobal.h
UserMenuRefData.o: /usr/local/lib/qt/include/qobject.h
UserMenuRefData.o: /usr/local/lib/qt/include/qstring.h
UserMenuRefData.o: /usr/local/lib/qt/include/qarray.h
UserMenuRefData.o: /usr/local/lib/qt/include/qgarray.h
UserMenuRefData.o: /usr/local/lib/qt/include/qshared.h
UserMenuRefData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
UserMenuRefData.o: /usr/include/features.h /usr/include/sys/cdefs.h
UserMenuRefData.o: /usr/local/lib/qt/include/qevent.h
UserMenuRefData.o: /usr/local/lib/qt/include/qrect.h
UserMenuRefData.o: /usr/local/lib/qt/include/qsize.h
UserMenuRefData.o: /usr/local/lib/qt/include/qpoint.h
UserMenuRefData.o: /usr/local/lib/qt/include/qpaintd.h
UserMenuRefData.o: /usr/local/lib/qt/include/qpalette.h
UserMenuRefData.o: /usr/local/lib/qt/include/qcolor.h
UserMenuRefData.o: /usr/local/lib/qt/include/qcursor.h
UserMenuRefData.o: /usr/local/lib/qt/include/qfont.h
UserMenuRefData.o: /usr/local/lib/qt/include/qfontmet.h
UserMenuRefData.o: /usr/local/lib/qt/include/qfontinf.h
UserMenuRefData.o: /usr/local/lib/qt/include/qchkbox.h
UserMenuRefData.o: /usr/local/lib/qt/include/qbutton.h
UserMenuRefData.o: /usr/local/lib/qt/include/qpushbt.h
UserMenuRefData.o: /usr/local/lib/qt/include/qlistbox.h
UserMenuRefData.o: /usr/local/lib/qt/include/qtablevw.h
UserMenuRefData.o: /usr/local/lib/qt/include/qpixmap.h
UserMenuRefData.o: /usr/local/lib/qt/include/qlined.h
UserMenuRefData.o: /usr/local/lib/qt/include/qcombo.h
UserMenuRefData.o: /usr/local/lib/qt/include/qlabel.h
iocontroller.o: iocontroller.h /usr/local/lib/qt/include/qobject.h
iocontroller.o: /usr/local/lib/qt/include/qobjdefs.h
iocontroller.o: /usr/local/lib/qt/include/qglobal.h
iocontroller.o: /usr/local/lib/qt/include/qstring.h
iocontroller.o: /usr/local/lib/qt/include/qarray.h
iocontroller.o: /usr/local/lib/qt/include/qgarray.h
iocontroller.o: /usr/local/lib/qt/include/qshared.h
iocontroller.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
iocontroller.o: /usr/include/features.h /usr/include/sys/cdefs.h
iocontroller.o: /usr/local/lib/qt/include/qevent.h
iocontroller.o: /usr/local/lib/qt/include/qwindefs.h
iocontroller.o: /usr/local/lib/qt/include/qrect.h
iocontroller.o: /usr/local/lib/qt/include/qsize.h
iocontroller.o: /usr/local/lib/qt/include/qpoint.h
iocontroller.o: /usr/local/lib/qt/include/qwidget.h
iocontroller.o: /usr/local/lib/qt/include/qpaintd.h
iocontroller.o: /usr/local/lib/qt/include/qpalette.h
iocontroller.o: /usr/local/lib/qt/include/qcolor.h
iocontroller.o: /usr/local/lib/qt/include/qcursor.h
iocontroller.o: /usr/local/lib/qt/include/qfont.h
iocontroller.o: /usr/local/lib/qt/include/qfontmet.h
iocontroller.o: /usr/local/lib/qt/include/qfontinf.h
iocontroller.o: /usr/local/lib/qt/include/qsocknot.h
iocontroller.o: /usr/local/lib/qt/include/qdict.h
iocontroller.o: /usr/local/lib/qt/include/qgdict.h
iocontroller.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
iocontroller.o: ksircprocess.h toplevel.h /usr/include/unistd.h
iocontroller.o: /usr/include/posix_opt.h /usr/include/gnu/types.h
iocontroller.o: /usr/include/confname.h /usr/include/sys/types.h
iocontroller.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
iocontroller.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
iocontroller.o: /usr/include/sys/bitypes.h /usr/include/signal.h
iocontroller.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
iocontroller.o: /usr/local/lib/qt/include/qlined.h
iocontroller.o: /usr/local/lib/qt/include/qmlined.h
iocontroller.o: /usr/local/lib/qt/include/qtablevw.h
iocontroller.o: /usr/local/lib/qt/include/qframe.h
iocontroller.o: /usr/local/lib/qt/include/qlist.h
iocontroller.o: /usr/local/lib/qt/include/qglist.h
iocontroller.o: /usr/local/lib/qt/include/qlayout.h
iocontroller.o: /usr/local/lib/qt/include/qgmanagr.h
iocontroller.o: /usr/local/lib/qt/include/qintdict.h
iocontroller.o: /usr/local/lib/qt/include/qlistbox.h
iocontroller.o: /usr/local/lib/qt/include/qpixmap.h
iocontroller.o: /usr/local/lib/qt/include/qscrbar.h
iocontroller.o: /usr/local/lib/qt/include/qrangect.h
iocontroller.o: /usr/local/lib/qt/include/qdrawutl.h
iocontroller.o: /usr/local/lib/qt/include/qpainter.h
iocontroller.o: /usr/local/lib/qt/include/qregion.h
iocontroller.o: /usr/local/lib/qt/include/qpen.h
iocontroller.o: /usr/local/lib/qt/include/qbrush.h
iocontroller.o: /usr/local/lib/qt/include/qpntarry.h
iocontroller.o: /usr/local/lib/qt/include/qwmatrix.h
iocontroller.o: /usr/local/lib/qt/include/qregexp.h
iocontroller.o: /usr/local/lib/qt/include/qaccel.h
iocontroller.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
iocontroller.o: /usr/local/lib/qt/include/qstrlist.h
iocontroller.o: /usr/local/lib/qt/include/qdstream.h
iocontroller.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h alistbox.h
iocontroller.o: UserMenuRef.h UserMenuRefData.h
iocontroller.o: /usr/local/lib/qt/include/qchkbox.h
iocontroller.o: /usr/local/lib/qt/include/qbutton.h
iocontroller.o: /usr/local/lib/qt/include/qpushbt.h
iocontroller.o: /usr/local/lib/qt/include/qcombo.h irclistbox.h
iocontroller.o: control_message.h
open_top.o: open_top.h open_topData.h /usr/local/lib/qt/include/qdialog.h
open_top.o: /usr/local/lib/qt/include/qwidget.h
open_top.o: /usr/local/lib/qt/include/qwindefs.h
open_top.o: /usr/local/lib/qt/include/qobjdefs.h
open_top.o: /usr/local/lib/qt/include/qglobal.h
open_top.o: /usr/local/lib/qt/include/qobject.h
open_top.o: /usr/local/lib/qt/include/qstring.h
open_top.o: /usr/local/lib/qt/include/qarray.h
open_top.o: /usr/local/lib/qt/include/qgarray.h
open_top.o: /usr/local/lib/qt/include/qshared.h
open_top.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
open_top.o: /usr/include/features.h /usr/include/sys/cdefs.h
open_top.o: /usr/local/lib/qt/include/qevent.h
open_top.o: /usr/local/lib/qt/include/qrect.h
open_top.o: /usr/local/lib/qt/include/qsize.h
open_top.o: /usr/local/lib/qt/include/qpoint.h
open_top.o: /usr/local/lib/qt/include/qpaintd.h
open_top.o: /usr/local/lib/qt/include/qpalette.h
open_top.o: /usr/local/lib/qt/include/qcolor.h
open_top.o: /usr/local/lib/qt/include/qcursor.h
open_top.o: /usr/local/lib/qt/include/qfont.h
open_top.o: /usr/local/lib/qt/include/qfontmet.h
open_top.o: /usr/local/lib/qt/include/qfontinf.h
open_top.o: /usr/local/lib/qt/include/qpushbt.h
open_top.o: /usr/local/lib/qt/include/qbutton.h
open_top.o: /usr/local/lib/qt/include/qcombo.h messageReceiver.h toplevel.h
open_top.o: /usr/include/unistd.h /usr/include/posix_opt.h
open_top.o: /usr/include/gnu/types.h /usr/include/confname.h
open_top.o: /usr/include/sys/types.h /usr/include/linux/types.h
open_top.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
open_top.o: /usr/include/asm/types.h /usr/include/sys/bitypes.h
open_top.o: /usr/include/signal.h /usr/include/linux/signal.h
open_top.o: /usr/include/asm/signal.h /usr/local/lib/qt/include/qsocknot.h
open_top.o: /usr/local/lib/qt/include/qlined.h
open_top.o: /usr/local/lib/qt/include/qmlined.h
open_top.o: /usr/local/lib/qt/include/qtablevw.h
open_top.o: /usr/local/lib/qt/include/qframe.h
open_top.o: /usr/local/lib/qt/include/qlist.h
open_top.o: /usr/local/lib/qt/include/qglist.h
open_top.o: /usr/local/lib/qt/include/qcollect.h
open_top.o: /usr/local/lib/qt/include/qlayout.h
open_top.o: /usr/local/lib/qt/include/qgmanagr.h
open_top.o: /usr/local/lib/qt/include/qintdict.h
open_top.o: /usr/local/lib/qt/include/qgdict.h
open_top.o: /usr/local/lib/qt/include/qlistbox.h
open_top.o: /usr/local/lib/qt/include/qpixmap.h
open_top.o: /usr/local/lib/qt/include/qscrbar.h
open_top.o: /usr/local/lib/qt/include/qrangect.h
open_top.o: /usr/local/lib/qt/include/qdrawutl.h
open_top.o: /usr/local/lib/qt/include/qpainter.h
open_top.o: /usr/local/lib/qt/include/qregion.h
open_top.o: /usr/local/lib/qt/include/qpen.h
open_top.o: /usr/local/lib/qt/include/qbrush.h
open_top.o: /usr/local/lib/qt/include/qpntarry.h
open_top.o: /usr/local/lib/qt/include/qwmatrix.h
open_top.o: /usr/local/lib/qt/include/qregexp.h
open_top.o: /usr/local/lib/qt/include/qaccel.h
open_top.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
open_top.o: /usr/local/lib/qt/include/qstrlist.h
open_top.o: /usr/local/lib/qt/include/qdstream.h
open_top.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h alistbox.h
open_top.o: ksircprocess.h iocontroller.h /usr/local/lib/qt/include/qdict.h
open_top.o: UserMenuRef.h UserMenuRefData.h
open_top.o: /usr/local/lib/qt/include/qchkbox.h irclistbox.h
open_topData.o: open_topData.h /usr/local/lib/qt/include/qdialog.h
open_topData.o: /usr/local/lib/qt/include/qwidget.h
open_topData.o: /usr/local/lib/qt/include/qwindefs.h
open_topData.o: /usr/local/lib/qt/include/qobjdefs.h
open_topData.o: /usr/local/lib/qt/include/qglobal.h
open_topData.o: /usr/local/lib/qt/include/qobject.h
open_topData.o: /usr/local/lib/qt/include/qstring.h
open_topData.o: /usr/local/lib/qt/include/qarray.h
open_topData.o: /usr/local/lib/qt/include/qgarray.h
open_topData.o: /usr/local/lib/qt/include/qshared.h
open_topData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
open_topData.o: /usr/include/features.h /usr/include/sys/cdefs.h
open_topData.o: /usr/local/lib/qt/include/qevent.h
open_topData.o: /usr/local/lib/qt/include/qrect.h
open_topData.o: /usr/local/lib/qt/include/qsize.h
open_topData.o: /usr/local/lib/qt/include/qpoint.h
open_topData.o: /usr/local/lib/qt/include/qpaintd.h
open_topData.o: /usr/local/lib/qt/include/qpalette.h
open_topData.o: /usr/local/lib/qt/include/qcolor.h
open_topData.o: /usr/local/lib/qt/include/qcursor.h
open_topData.o: /usr/local/lib/qt/include/qfont.h
open_topData.o: /usr/local/lib/qt/include/qfontmet.h
open_topData.o: /usr/local/lib/qt/include/qfontinf.h
open_topData.o: /usr/local/lib/qt/include/qpushbt.h
open_topData.o: /usr/local/lib/qt/include/qbutton.h
open_topData.o: /usr/local/lib/qt/include/qcombo.h
open_topData.o: /usr/local/lib/qt/include/qlabel.h
open_topData.o: /usr/local/lib/qt/include/qframe.h
messageReceiver.o: messageReceiver.h /usr/local/lib/qt/include/qstring.h
messageReceiver.o: /usr/local/lib/qt/include/qarray.h
messageReceiver.o: /usr/local/lib/qt/include/qgarray.h
messageReceiver.o: /usr/local/lib/qt/include/qshared.h
messageReceiver.o: /usr/local/lib/qt/include/qglobal.h
messageReceiver.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
messageReceiver.o: /usr/include/features.h /usr/include/sys/cdefs.h
messageReceiver.o: ksircprocess.h /usr/local/lib/qt/include/qobject.h
messageReceiver.o: /usr/local/lib/qt/include/qobjdefs.h
messageReceiver.o: /usr/local/lib/qt/include/qevent.h
messageReceiver.o: /usr/local/lib/qt/include/qwindefs.h
messageReceiver.o: /usr/local/lib/qt/include/qrect.h
messageReceiver.o: /usr/local/lib/qt/include/qsize.h
messageReceiver.o: /usr/local/lib/qt/include/qpoint.h iocontroller.h
messageReceiver.o: /usr/local/lib/qt/include/qwidget.h
messageReceiver.o: /usr/local/lib/qt/include/qpaintd.h
messageReceiver.o: /usr/local/lib/qt/include/qpalette.h
messageReceiver.o: /usr/local/lib/qt/include/qcolor.h
messageReceiver.o: /usr/local/lib/qt/include/qcursor.h
messageReceiver.o: /usr/local/lib/qt/include/qfont.h
messageReceiver.o: /usr/local/lib/qt/include/qfontmet.h
messageReceiver.o: /usr/local/lib/qt/include/qfontinf.h
messageReceiver.o: /usr/local/lib/qt/include/qsocknot.h
messageReceiver.o: /usr/local/lib/qt/include/qdict.h
messageReceiver.o: /usr/local/lib/qt/include/qgdict.h
messageReceiver.o: /usr/local/lib/qt/include/qcollect.h
ioBroadcast.o: ioBroadcast.h /usr/local/lib/qt/include/qobject.h
ioBroadcast.o: /usr/local/lib/qt/include/qobjdefs.h
ioBroadcast.o: /usr/local/lib/qt/include/qglobal.h
ioBroadcast.o: /usr/local/lib/qt/include/qstring.h
ioBroadcast.o: /usr/local/lib/qt/include/qarray.h
ioBroadcast.o: /usr/local/lib/qt/include/qgarray.h
ioBroadcast.o: /usr/local/lib/qt/include/qshared.h
ioBroadcast.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
ioBroadcast.o: /usr/include/features.h /usr/include/sys/cdefs.h
ioBroadcast.o: /usr/local/lib/qt/include/qevent.h
ioBroadcast.o: /usr/local/lib/qt/include/qwindefs.h
ioBroadcast.o: /usr/local/lib/qt/include/qrect.h
ioBroadcast.o: /usr/local/lib/qt/include/qsize.h
ioBroadcast.o: /usr/local/lib/qt/include/qpoint.h
ioBroadcast.o: /usr/local/lib/qt/include/qdict.h
ioBroadcast.o: /usr/local/lib/qt/include/qgdict.h
ioBroadcast.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
ioBroadcast.o: ksircprocess.h iocontroller.h
ioBroadcast.o: /usr/local/lib/qt/include/qwidget.h
ioBroadcast.o: /usr/local/lib/qt/include/qpaintd.h
ioBroadcast.o: /usr/local/lib/qt/include/qpalette.h
ioBroadcast.o: /usr/local/lib/qt/include/qcolor.h
ioBroadcast.o: /usr/local/lib/qt/include/qcursor.h
ioBroadcast.o: /usr/local/lib/qt/include/qfont.h
ioBroadcast.o: /usr/local/lib/qt/include/qfontmet.h
ioBroadcast.o: /usr/local/lib/qt/include/qfontinf.h
ioBroadcast.o: /usr/local/lib/qt/include/qsocknot.h
ksircprocess.o: ksircprocess.h /usr/local/lib/qt/include/qobject.h
ksircprocess.o: /usr/local/lib/qt/include/qobjdefs.h
ksircprocess.o: /usr/local/lib/qt/include/qglobal.h
ksircprocess.o: /usr/local/lib/qt/include/qstring.h
ksircprocess.o: /usr/local/lib/qt/include/qarray.h
ksircprocess.o: /usr/local/lib/qt/include/qgarray.h
ksircprocess.o: /usr/local/lib/qt/include/qshared.h
ksircprocess.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
ksircprocess.o: /usr/include/features.h /usr/include/sys/cdefs.h
ksircprocess.o: /usr/local/lib/qt/include/qevent.h
ksircprocess.o: /usr/local/lib/qt/include/qwindefs.h
ksircprocess.o: /usr/local/lib/qt/include/qrect.h
ksircprocess.o: /usr/local/lib/qt/include/qsize.h
ksircprocess.o: /usr/local/lib/qt/include/qpoint.h iocontroller.h
ksircprocess.o: /usr/local/lib/qt/include/qwidget.h
ksircprocess.o: /usr/local/lib/qt/include/qpaintd.h
ksircprocess.o: /usr/local/lib/qt/include/qpalette.h
ksircprocess.o: /usr/local/lib/qt/include/qcolor.h
ksircprocess.o: /usr/local/lib/qt/include/qcursor.h
ksircprocess.o: /usr/local/lib/qt/include/qfont.h
ksircprocess.o: /usr/local/lib/qt/include/qfontmet.h
ksircprocess.o: /usr/local/lib/qt/include/qfontinf.h
ksircprocess.o: /usr/local/lib/qt/include/qsocknot.h
ksircprocess.o: /usr/local/lib/qt/include/qdict.h
ksircprocess.o: /usr/local/lib/qt/include/qgdict.h
ksircprocess.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
ksircprocess.o: toplevel.h /usr/include/unistd.h /usr/include/posix_opt.h
ksircprocess.o: /usr/include/gnu/types.h /usr/include/confname.h
ksircprocess.o: /usr/include/sys/types.h /usr/include/linux/types.h
ksircprocess.o: /usr/include/linux/posix_types.h
ksircprocess.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
ksircprocess.o: /usr/include/sys/bitypes.h /usr/include/signal.h
ksircprocess.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
ksircprocess.o: /usr/local/lib/qt/include/qlined.h
ksircprocess.o: /usr/local/lib/qt/include/qmlined.h
ksircprocess.o: /usr/local/lib/qt/include/qtablevw.h
ksircprocess.o: /usr/local/lib/qt/include/qframe.h
ksircprocess.o: /usr/local/lib/qt/include/qlist.h
ksircprocess.o: /usr/local/lib/qt/include/qglist.h
ksircprocess.o: /usr/local/lib/qt/include/qlayout.h
ksircprocess.o: /usr/local/lib/qt/include/qgmanagr.h
ksircprocess.o: /usr/local/lib/qt/include/qintdict.h
ksircprocess.o: /usr/local/lib/qt/include/qlistbox.h
ksircprocess.o: /usr/local/lib/qt/include/qpixmap.h
ksircprocess.o: /usr/local/lib/qt/include/qscrbar.h
ksircprocess.o: /usr/local/lib/qt/include/qrangect.h
ksircprocess.o: /usr/local/lib/qt/include/qdrawutl.h
ksircprocess.o: /usr/local/lib/qt/include/qpainter.h
ksircprocess.o: /usr/local/lib/qt/include/qregion.h
ksircprocess.o: /usr/local/lib/qt/include/qpen.h
ksircprocess.o: /usr/local/lib/qt/include/qbrush.h
ksircprocess.o: /usr/local/lib/qt/include/qpntarry.h
ksircprocess.o: /usr/local/lib/qt/include/qwmatrix.h
ksircprocess.o: /usr/local/lib/qt/include/qregexp.h
ksircprocess.o: /usr/local/lib/qt/include/qaccel.h
ksircprocess.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
ksircprocess.o: /usr/local/lib/qt/include/qstrlist.h
ksircprocess.o: /usr/local/lib/qt/include/qdstream.h
ksircprocess.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h alistbox.h
ksircprocess.o: UserMenuRef.h UserMenuRefData.h
ksircprocess.o: /usr/local/lib/qt/include/qchkbox.h
ksircprocess.o: /usr/local/lib/qt/include/qbutton.h
ksircprocess.o: /usr/local/lib/qt/include/qpushbt.h
ksircprocess.o: /usr/local/lib/qt/include/qcombo.h irclistbox.h ioBroadcast.h
open_ksirc.o: open_ksirc.h open_ksircData.h
open_ksirc.o: /usr/local/lib/qt/include/qdialog.h
open_ksirc.o: /usr/local/lib/qt/include/qwidget.h
open_ksirc.o: /usr/local/lib/qt/include/qwindefs.h
open_ksirc.o: /usr/local/lib/qt/include/qobjdefs.h
open_ksirc.o: /usr/local/lib/qt/include/qglobal.h
open_ksirc.o: /usr/local/lib/qt/include/qobject.h
open_ksirc.o: /usr/local/lib/qt/include/qstring.h
open_ksirc.o: /usr/local/lib/qt/include/qarray.h
open_ksirc.o: /usr/local/lib/qt/include/qgarray.h
open_ksirc.o: /usr/local/lib/qt/include/qshared.h
open_ksirc.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
open_ksirc.o: /usr/include/features.h /usr/include/sys/cdefs.h
open_ksirc.o: /usr/local/lib/qt/include/qevent.h
open_ksirc.o: /usr/local/lib/qt/include/qrect.h
open_ksirc.o: /usr/local/lib/qt/include/qsize.h
open_ksirc.o: /usr/local/lib/qt/include/qpoint.h
open_ksirc.o: /usr/local/lib/qt/include/qpaintd.h
open_ksirc.o: /usr/local/lib/qt/include/qpalette.h
open_ksirc.o: /usr/local/lib/qt/include/qcolor.h
open_ksirc.o: /usr/local/lib/qt/include/qcursor.h
open_ksirc.o: /usr/local/lib/qt/include/qfont.h
open_ksirc.o: /usr/local/lib/qt/include/qfontmet.h
open_ksirc.o: /usr/local/lib/qt/include/qfontinf.h
open_ksirc.o: /usr/local/lib/qt/include/qpushbt.h
open_ksirc.o: /usr/local/lib/qt/include/qbutton.h
open_ksirc.o: /usr/local/lib/qt/include/qcombo.h iocontroller.h
open_ksirc.o: /usr/local/lib/qt/include/qsocknot.h
open_ksirc.o: /usr/local/lib/qt/include/qdict.h
open_ksirc.o: /usr/local/lib/qt/include/qgdict.h
open_ksirc.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
open_ksirc.o: ksircprocess.h
open_ksircData.o: open_ksircData.h /usr/local/lib/qt/include/qdialog.h
open_ksircData.o: /usr/local/lib/qt/include/qwidget.h
open_ksircData.o: /usr/local/lib/qt/include/qwindefs.h
open_ksircData.o: /usr/local/lib/qt/include/qobjdefs.h
open_ksircData.o: /usr/local/lib/qt/include/qglobal.h
open_ksircData.o: /usr/local/lib/qt/include/qobject.h
open_ksircData.o: /usr/local/lib/qt/include/qstring.h
open_ksircData.o: /usr/local/lib/qt/include/qarray.h
open_ksircData.o: /usr/local/lib/qt/include/qgarray.h
open_ksircData.o: /usr/local/lib/qt/include/qshared.h
open_ksircData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
open_ksircData.o: /usr/include/features.h /usr/include/sys/cdefs.h
open_ksircData.o: /usr/local/lib/qt/include/qevent.h
open_ksircData.o: /usr/local/lib/qt/include/qrect.h
open_ksircData.o: /usr/local/lib/qt/include/qsize.h
open_ksircData.o: /usr/local/lib/qt/include/qpoint.h
open_ksircData.o: /usr/local/lib/qt/include/qpaintd.h
open_ksircData.o: /usr/local/lib/qt/include/qpalette.h
open_ksircData.o: /usr/local/lib/qt/include/qcolor.h
open_ksircData.o: /usr/local/lib/qt/include/qcursor.h
open_ksircData.o: /usr/local/lib/qt/include/qfont.h
open_ksircData.o: /usr/local/lib/qt/include/qfontmet.h
open_ksircData.o: /usr/local/lib/qt/include/qfontinf.h
open_ksircData.o: /usr/local/lib/qt/include/qpushbt.h
open_ksircData.o: /usr/local/lib/qt/include/qbutton.h
open_ksircData.o: /usr/local/lib/qt/include/qcombo.h
open_ksircData.o: /usr/local/lib/qt/include/qlabel.h
open_ksircData.o: /usr/local/lib/qt/include/qframe.h
servercontroller.o: /usr/local/lib/qt/include/qpopmenu.h
servercontroller.o: /usr/local/lib/qt/include/qtablevw.h
servercontroller.o: /usr/local/lib/qt/include/qframe.h
servercontroller.o: /usr/local/lib/qt/include/qwidget.h
servercontroller.o: /usr/local/lib/qt/include/qwindefs.h
servercontroller.o: /usr/local/lib/qt/include/qobjdefs.h
servercontroller.o: /usr/local/lib/qt/include/qglobal.h
servercontroller.o: /usr/local/lib/qt/include/qobject.h
servercontroller.o: /usr/local/lib/qt/include/qstring.h
servercontroller.o: /usr/local/lib/qt/include/qarray.h
servercontroller.o: /usr/local/lib/qt/include/qgarray.h
servercontroller.o: /usr/local/lib/qt/include/qshared.h
servercontroller.o: /usr/local/lib/qt/include/qgeneric.h
servercontroller.o: /usr/include/string.h /usr/include/features.h
servercontroller.o: /usr/include/sys/cdefs.h
servercontroller.o: /usr/local/lib/qt/include/qevent.h
servercontroller.o: /usr/local/lib/qt/include/qrect.h
servercontroller.o: /usr/local/lib/qt/include/qsize.h
servercontroller.o: /usr/local/lib/qt/include/qpoint.h
servercontroller.o: /usr/local/lib/qt/include/qpaintd.h
servercontroller.o: /usr/local/lib/qt/include/qpalette.h
servercontroller.o: /usr/local/lib/qt/include/qcolor.h
servercontroller.o: /usr/local/lib/qt/include/qcursor.h
servercontroller.o: /usr/local/lib/qt/include/qfont.h
servercontroller.o: /usr/local/lib/qt/include/qfontmet.h
servercontroller.o: /usr/local/lib/qt/include/qfontinf.h
servercontroller.o: /usr/local/lib/qt/include/qmenudta.h servercontroller.h
servercontroller.o: /usr/local/lib/qt/include/qdict.h
servercontroller.o: /usr/local/lib/qt/include/qgdict.h
servercontroller.o: /usr/local/lib/qt/include/qcollect.h
servercontroller.o: servercontrollerData.h control.h ksircprocess.h
servercontroller.o: iocontroller.h /usr/local/lib/qt/include/qsocknot.h
servercontroller.o: messageReceiver.h open_ksirc.h open_ksircData.h
servercontroller.o: /usr/local/lib/qt/include/qdialog.h
servercontroller.o: /usr/local/lib/qt/include/qpushbt.h
servercontroller.o: /usr/local/lib/qt/include/qbutton.h
servercontroller.o: /usr/local/lib/qt/include/qcombo.h open_top.h
servercontroller.o: open_topData.h toplevel.h /usr/include/unistd.h
servercontroller.o: /usr/include/posix_opt.h /usr/include/gnu/types.h
servercontroller.o: /usr/include/confname.h /usr/include/sys/types.h
servercontroller.o: /usr/include/linux/types.h
servercontroller.o: /usr/include/linux/posix_types.h
servercontroller.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
servercontroller.o: /usr/include/sys/bitypes.h /usr/include/signal.h
servercontroller.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
servercontroller.o: /usr/local/lib/qt/include/qlined.h
servercontroller.o: /usr/local/lib/qt/include/qmlined.h
servercontroller.o: /usr/local/lib/qt/include/qlist.h
servercontroller.o: /usr/local/lib/qt/include/qglist.h
servercontroller.o: /usr/local/lib/qt/include/qlayout.h
servercontroller.o: /usr/local/lib/qt/include/qgmanagr.h
servercontroller.o: /usr/local/lib/qt/include/qintdict.h
servercontroller.o: /usr/local/lib/qt/include/qlistbox.h
servercontroller.o: /usr/local/lib/qt/include/qpixmap.h
servercontroller.o: /usr/local/lib/qt/include/qscrbar.h
servercontroller.o: /usr/local/lib/qt/include/qrangect.h
servercontroller.o: /usr/local/lib/qt/include/qdrawutl.h
servercontroller.o: /usr/local/lib/qt/include/qpainter.h
servercontroller.o: /usr/local/lib/qt/include/qregion.h
servercontroller.o: /usr/local/lib/qt/include/qpen.h
servercontroller.o: /usr/local/lib/qt/include/qbrush.h
servercontroller.o: /usr/local/lib/qt/include/qpntarry.h
servercontroller.o: /usr/local/lib/qt/include/qwmatrix.h
servercontroller.o: /usr/local/lib/qt/include/qregexp.h
servercontroller.o: /usr/local/lib/qt/include/qaccel.h
servercontroller.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
servercontroller.o: /usr/local/lib/qt/include/qstrlist.h
servercontroller.o: /usr/local/lib/qt/include/qdstream.h
servercontroller.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h
servercontroller.o: alistbox.h UserMenuRef.h UserMenuRefData.h
servercontroller.o: /usr/local/lib/qt/include/qchkbox.h irclistbox.h
servercontroller.o: KSircColour.h KSircColourData.h
servercontrollerData.o: servercontrollerData.h control.h
servercontrollerData.o: /usr/local/lib/qt/include/qframe.h
servercontrollerData.o: /usr/local/lib/qt/include/qwidget.h
servercontrollerData.o: /usr/local/lib/qt/include/qwindefs.h
servercontrollerData.o: /usr/local/lib/qt/include/qobjdefs.h
servercontrollerData.o: /usr/local/lib/qt/include/qglobal.h
servercontrollerData.o: /usr/local/lib/qt/include/qobject.h
servercontrollerData.o: /usr/local/lib/qt/include/qstring.h
servercontrollerData.o: /usr/local/lib/qt/include/qarray.h
servercontrollerData.o: /usr/local/lib/qt/include/qgarray.h
servercontrollerData.o: /usr/local/lib/qt/include/qshared.h
servercontrollerData.o: /usr/local/lib/qt/include/qgeneric.h
servercontrollerData.o: /usr/include/string.h /usr/include/features.h
servercontrollerData.o: /usr/include/sys/cdefs.h
servercontrollerData.o: /usr/local/lib/qt/include/qevent.h
servercontrollerData.o: /usr/local/lib/qt/include/qrect.h
servercontrollerData.o: /usr/local/lib/qt/include/qsize.h
servercontrollerData.o: /usr/local/lib/qt/include/qpoint.h
servercontrollerData.o: /usr/local/lib/qt/include/qpaintd.h
servercontrollerData.o: /usr/local/lib/qt/include/qpalette.h
servercontrollerData.o: /usr/local/lib/qt/include/qcolor.h
servercontrollerData.o: /usr/local/lib/qt/include/qcursor.h
servercontrollerData.o: /usr/local/lib/qt/include/qfont.h
servercontrollerData.o: /usr/local/lib/qt/include/qfontmet.h
servercontrollerData.o: /usr/local/lib/qt/include/qfontinf.h
servercontrollerData.o: /usr/local/lib/qt/include/qlabel.h
servercontrollerData.o: /usr/local/lib/qt/include/qlayout.h
servercontrollerData.o: /usr/local/lib/qt/include/qgmanagr.h
servercontrollerData.o: /usr/local/lib/qt/include/qintdict.h
servercontrollerData.o: /usr/local/lib/qt/include/qgdict.h
servercontrollerData.o: /usr/local/lib/qt/include/qcollect.h
servercontrollerData.o: /usr/local/lib/qt/include/qlist.h
servercontrollerData.o: /usr/local/lib/qt/include/qglist.h
control.o: control.h
KSircColour.o: KSircColour.h KSircColourData.h
KSircColour.o: /usr/local/lib/qt/include/qframe.h
KSircColour.o: /usr/local/lib/qt/include/qwidget.h
KSircColour.o: /usr/local/lib/qt/include/qwindefs.h
KSircColour.o: /usr/local/lib/qt/include/qobjdefs.h
KSircColour.o: /usr/local/lib/qt/include/qglobal.h
KSircColour.o: /usr/local/lib/qt/include/qobject.h
KSircColour.o: /usr/local/lib/qt/include/qstring.h
KSircColour.o: /usr/local/lib/qt/include/qarray.h
KSircColour.o: /usr/local/lib/qt/include/qgarray.h
KSircColour.o: /usr/local/lib/qt/include/qshared.h
KSircColour.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
KSircColour.o: /usr/include/features.h /usr/include/sys/cdefs.h
KSircColour.o: /usr/local/lib/qt/include/qevent.h
KSircColour.o: /usr/local/lib/qt/include/qrect.h
KSircColour.o: /usr/local/lib/qt/include/qsize.h
KSircColour.o: /usr/local/lib/qt/include/qpoint.h
KSircColour.o: /usr/local/lib/qt/include/qpaintd.h
KSircColour.o: /usr/local/lib/qt/include/qpalette.h
KSircColour.o: /usr/local/lib/qt/include/qcolor.h
KSircColour.o: /usr/local/lib/qt/include/qcursor.h
KSircColour.o: /usr/local/lib/qt/include/qfont.h
KSircColour.o: /usr/local/lib/qt/include/qfontmet.h
KSircColour.o: /usr/local/lib/qt/include/qfontinf.h config.h
KSircColourData.o: KSircColourData.h /usr/local/lib/qt/include/qframe.h
KSircColourData.o: /usr/local/lib/qt/include/qwidget.h
KSircColourData.o: /usr/local/lib/qt/include/qwindefs.h
KSircColourData.o: /usr/local/lib/qt/include/qobjdefs.h
KSircColourData.o: /usr/local/lib/qt/include/qglobal.h
KSircColourData.o: /usr/local/lib/qt/include/qobject.h
KSircColourData.o: /usr/local/lib/qt/include/qstring.h
KSircColourData.o: /usr/local/lib/qt/include/qarray.h
KSircColourData.o: /usr/local/lib/qt/include/qgarray.h
KSircColourData.o: /usr/local/lib/qt/include/qshared.h
KSircColourData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
KSircColourData.o: /usr/include/features.h /usr/include/sys/cdefs.h
KSircColourData.o: /usr/local/lib/qt/include/qevent.h
KSircColourData.o: /usr/local/lib/qt/include/qrect.h
KSircColourData.o: /usr/local/lib/qt/include/qsize.h
KSircColourData.o: /usr/local/lib/qt/include/qpoint.h
KSircColourData.o: /usr/local/lib/qt/include/qpaintd.h
KSircColourData.o: /usr/local/lib/qt/include/qpalette.h
KSircColourData.o: /usr/local/lib/qt/include/qcolor.h
KSircColourData.o: /usr/local/lib/qt/include/qcursor.h
KSircColourData.o: /usr/local/lib/qt/include/qfont.h
KSircColourData.o: /usr/local/lib/qt/include/qfontmet.h
KSircColourData.o: /usr/local/lib/qt/include/qfontinf.h
KSircColourData.o: /usr/local/lib/qt/include/qlabel.h
KSircColourData.o: /usr/local/lib/qt/include/qpushbt.h
KSircColourData.o: /usr/local/lib/qt/include/qbutton.h
KSircColourData.o: /usr/local/lib/qt/include/qlayout.h
KSircColourData.o: /usr/local/lib/qt/include/qgmanagr.h
KSircColourData.o: /usr/local/lib/qt/include/qintdict.h
KSircColourData.o: /usr/local/lib/qt/include/qgdict.h
KSircColourData.o: /usr/local/lib/qt/include/qcollect.h
KSircColourData.o: /usr/local/lib/qt/include/qlist.h
KSircColourData.o: /usr/local/lib/qt/include/qglist.h
