####### This section was automatically generated from
#######    /usr/local/lib/qt-1.31/Makefile for building on
#######    linux-gcc-shared on Sun Oct 12 09:16:39 MDT 1997

export INCDIR = /usr/local/lib/qt/include
#export CFLAGS = -O3 -m486 -Wall -I/usr/X11R6/include -I$(HOME)/include -I/opt/kde/include
export CFLAGS = -ggdb -g -Wall -I/usr/X11R6/include -I$(HOME)/include -I/opt/kde/include
export LIBCFLAGS = -fPIC
export YACCCFLAGS = -Wno-unused -Wno-parentheses
export LFLAGS = -L/usr/local/lib/qt/lib -lqt -L/opt/kde/lib -lkdecore -lkdeui -L/usr/X11/lib -lXext
export CC = g++
export MOC = /usr/local/lib/qt/bin/moc
export SHELL =	/bin/sh

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
FilterRuleEditor.cpp FilterRuleEditorData.cpp mFilterRuleEditor.cpp mFilterRuleEditorData.cpp \
ioDiscard.cpp \
ioDCC.cpp mioDCC.cpp dccDialog.cpp mdccDialog.cpp dccDialogData.cpp mdccDialogData.cpp \
linelistitem.cpp

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
FilterRuleEditor.o FilterRuleEditorData.o mFilterRuleEditor.o mFilterRuleEditorData.o \
ioDiscard.o \
ioDCC.o mioDCC.o dccDialog.o mdccDialog.o dccDialogData.o mdccDialogData.o \
linelistitem.o \
KSProgress/ksprogress.a KSTicker/ksticker.a

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

$(TARGET): $(OBJECTS) KSProgress KSTicker
	$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS) -lm

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(HEADERS) $(SOURCES) $(OBJECTS) Makefile

clean:
	rm -f *.o *.bak *~ *% core *.orig log.flood
	rm -f $(TARGET) ksirc-static
	$(MAKE) -C KSProgress clean
	$(MAKE) -C KSTicker clean

####### BUILD RULES FOR SUB DIRS

KSProgress/ksprogress.a: dummy
	$(MAKE) -C KSProgress

KSTicker/ksticker.a: dummy
	$(MAKE) -C KSTicker



dummy:

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
toplevel.o: /usr/local/lib/qt/include/qdict.h KSTicker/ksticker.h
toplevel.o: /usr/local/lib/qt/include/qpopmenu.h
toplevel.o: /usr/local/lib/qt/include/qmenudta.h UserMenuRef.h
toplevel.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
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
ksirc.o: /usr/local/lib/qt/include/qgeneric.h
ksirc.o: /usr/local/lib/qt/include/qpixmap.h
ksirc.o: /usr/local/lib/qt/include/qpaintd.h
ksirc.o: /usr/local/lib/qt/include/qwindefs.h
ksirc.o: /usr/local/lib/qt/include/qobjdefs.h
ksirc.o: /usr/local/lib/qt/include/qrect.h /usr/local/lib/qt/include/qsize.h
ksirc.o: /usr/local/lib/qt/include/qpoint.h
ksirc.o: /usr/local/lib/qt/include/qcolor.h
ksirc.o: /usr/local/lib/qt/include/qshared.h servercontrollerData.h control.h
ksirc.o: /usr/local/lib/qt/include/qframe.h
ksirc.o: /usr/local/lib/qt/include/qwidget.h
ksirc.o: /usr/local/lib/qt/include/qobject.h
ksirc.o: /usr/local/lib/qt/include/qstring.h
ksirc.o: /usr/local/lib/qt/include/qarray.h
ksirc.o: /usr/local/lib/qt/include/qgarray.h /usr/include/string.h
ksirc.o: /usr/include/features.h /usr/include/sys/cdefs.h
ksirc.o: /usr/local/lib/qt/include/qevent.h
ksirc.o: /usr/local/lib/qt/include/qpalette.h
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
mirclistbox.o: irclistbox.h /usr/local/lib/qt/include/qobject.h
mirclistbox.o: /usr/local/lib/qt/include/qobjdefs.h
mirclistbox.o: /usr/local/lib/qt/include/qglobal.h
mirclistbox.o: /usr/local/lib/qt/include/qstring.h
mirclistbox.o: /usr/local/lib/qt/include/qarray.h
mirclistbox.o: /usr/local/lib/qt/include/qgarray.h
mirclistbox.o: /usr/local/lib/qt/include/qshared.h
mirclistbox.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mirclistbox.o: /usr/include/features.h /usr/include/sys/cdefs.h
mirclistbox.o: /usr/local/lib/qt/include/qevent.h
mirclistbox.o: /usr/local/lib/qt/include/qwindefs.h
mirclistbox.o: /usr/local/lib/qt/include/qrect.h
mirclistbox.o: /usr/local/lib/qt/include/qsize.h
mirclistbox.o: /usr/local/lib/qt/include/qpoint.h
mirclistbox.o: /usr/local/lib/qt/include/qlistbox.h
mirclistbox.o: /usr/local/lib/qt/include/qtablevw.h
mirclistbox.o: /usr/local/lib/qt/include/qframe.h
mirclistbox.o: /usr/local/lib/qt/include/qwidget.h
mirclistbox.o: /usr/local/lib/qt/include/qpaintd.h
mirclistbox.o: /usr/local/lib/qt/include/qpalette.h
mirclistbox.o: /usr/local/lib/qt/include/qcolor.h
mirclistbox.o: /usr/local/lib/qt/include/qcursor.h
mirclistbox.o: /usr/local/lib/qt/include/qfont.h
mirclistbox.o: /usr/local/lib/qt/include/qfontmet.h
mirclistbox.o: /usr/local/lib/qt/include/qfontinf.h
mirclistbox.o: /usr/local/lib/qt/include/qpixmap.h
mirclistbox.o: /usr/local/lib/qt/include/qscrbar.h
mirclistbox.o: /usr/local/lib/qt/include/qrangect.h
mirclistbox.o: /usr/local/lib/qt/include/qdrawutl.h
mirclistbox.o: /usr/local/lib/qt/include/qpainter.h
mirclistbox.o: /usr/local/lib/qt/include/qregion.h
mirclistbox.o: /usr/local/lib/qt/include/qpen.h
mirclistbox.o: /usr/local/lib/qt/include/qbrush.h
mirclistbox.o: /usr/local/lib/qt/include/qpntarry.h
mirclistbox.o: /usr/local/lib/qt/include/qwmatrix.h
mirclistbox.o: /usr/local/lib/qt/include/qmetaobj.h
mirclistbox.o: /usr/local/lib/qt/include/qconnect.h
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
irclistitem.o: /usr/local/lib/qt/include/qiodev.h /usr/include/stdlib.h
irclistitem.o: /usr/include/errno.h /usr/include/linux/errno.h
irclistitem.o: /usr/include/asm/errno.h /usr/include/alloca.h
mirclistitem.o: irclistitem.h /usr/local/lib/qt/include/qobject.h
mirclistitem.o: /usr/local/lib/qt/include/qobjdefs.h
mirclistitem.o: /usr/local/lib/qt/include/qglobal.h
mirclistitem.o: /usr/local/lib/qt/include/qstring.h
mirclistitem.o: /usr/local/lib/qt/include/qarray.h
mirclistitem.o: /usr/local/lib/qt/include/qgarray.h
mirclistitem.o: /usr/local/lib/qt/include/qshared.h
mirclistitem.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mirclistitem.o: /usr/include/features.h /usr/include/sys/cdefs.h
mirclistitem.o: /usr/local/lib/qt/include/qevent.h
mirclistitem.o: /usr/local/lib/qt/include/qwindefs.h
mirclistitem.o: /usr/local/lib/qt/include/qrect.h
mirclistitem.o: /usr/local/lib/qt/include/qsize.h
mirclistitem.o: /usr/local/lib/qt/include/qpoint.h
mirclistitem.o: /usr/local/lib/qt/include/qcolor.h
mirclistitem.o: /usr/local/lib/qt/include/qlistbox.h
mirclistitem.o: /usr/local/lib/qt/include/qtablevw.h
mirclistitem.o: /usr/local/lib/qt/include/qframe.h
mirclistitem.o: /usr/local/lib/qt/include/qwidget.h
mirclistitem.o: /usr/local/lib/qt/include/qpaintd.h
mirclistitem.o: /usr/local/lib/qt/include/qpalette.h
mirclistitem.o: /usr/local/lib/qt/include/qcursor.h
mirclistitem.o: /usr/local/lib/qt/include/qfont.h
mirclistitem.o: /usr/local/lib/qt/include/qfontmet.h
mirclistitem.o: /usr/local/lib/qt/include/qfontinf.h
mirclistitem.o: /usr/local/lib/qt/include/qpixmap.h
mirclistitem.o: /usr/local/lib/qt/include/qpainter.h
mirclistitem.o: /usr/local/lib/qt/include/qregion.h
mirclistitem.o: /usr/local/lib/qt/include/qpen.h
mirclistitem.o: /usr/local/lib/qt/include/qbrush.h
mirclistitem.o: /usr/local/lib/qt/include/qpntarry.h
mirclistitem.o: /usr/local/lib/qt/include/qwmatrix.h
mirclistitem.o: /usr/local/lib/qt/include/qstrlist.h
mirclistitem.o: /usr/local/lib/qt/include/qlist.h
mirclistitem.o: /usr/local/lib/qt/include/qglist.h
mirclistitem.o: /usr/local/lib/qt/include/qcollect.h
mirclistitem.o: /usr/local/lib/qt/include/qdstream.h
mirclistitem.o: /usr/local/lib/qt/include/qiodev.h
mirclistitem.o: /usr/local/lib/qt/include/qmetaobj.h
mirclistitem.o: /usr/local/lib/qt/include/qconnect.h
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
alistbox.o: /usr/local/lib/qt/include/qpixmap.h irclistitem.h
alistbox.o: /usr/local/lib/qt/include/qpainter.h
alistbox.o: /usr/local/lib/qt/include/qregion.h
alistbox.o: /usr/local/lib/qt/include/qpen.h
alistbox.o: /usr/local/lib/qt/include/qbrush.h
alistbox.o: /usr/local/lib/qt/include/qpntarry.h
alistbox.o: /usr/local/lib/qt/include/qwmatrix.h
alistbox.o: /usr/local/lib/qt/include/qstrlist.h
alistbox.o: /usr/local/lib/qt/include/qlist.h
alistbox.o: /usr/local/lib/qt/include/qglist.h
alistbox.o: /usr/local/lib/qt/include/qcollect.h
alistbox.o: /usr/local/lib/qt/include/qdstream.h
alistbox.o: /usr/local/lib/qt/include/qiodev.h linelistitem.h
mtoplevel.o: toplevel.h /usr/include/unistd.h /usr/include/features.h
mtoplevel.o: /usr/include/sys/cdefs.h /usr/include/posix_opt.h
mtoplevel.o: /usr/include/gnu/types.h /usr/include/confname.h
mtoplevel.o: /usr/include/sys/types.h /usr/include/linux/types.h
mtoplevel.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
mtoplevel.o: /usr/include/asm/types.h /usr/include/sys/bitypes.h
mtoplevel.o: /usr/include/signal.h /usr/include/linux/signal.h
mtoplevel.o: /usr/include/asm/signal.h /usr/local/lib/qt/include/qwidget.h
mtoplevel.o: /usr/local/lib/qt/include/qwindefs.h
mtoplevel.o: /usr/local/lib/qt/include/qobjdefs.h
mtoplevel.o: /usr/local/lib/qt/include/qglobal.h
mtoplevel.o: /usr/local/lib/qt/include/qobject.h
mtoplevel.o: /usr/local/lib/qt/include/qstring.h
mtoplevel.o: /usr/local/lib/qt/include/qarray.h
mtoplevel.o: /usr/local/lib/qt/include/qgarray.h
mtoplevel.o: /usr/local/lib/qt/include/qshared.h
mtoplevel.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mtoplevel.o: /usr/local/lib/qt/include/qevent.h
mtoplevel.o: /usr/local/lib/qt/include/qrect.h
mtoplevel.o: /usr/local/lib/qt/include/qsize.h
mtoplevel.o: /usr/local/lib/qt/include/qpoint.h
mtoplevel.o: /usr/local/lib/qt/include/qpaintd.h
mtoplevel.o: /usr/local/lib/qt/include/qpalette.h
mtoplevel.o: /usr/local/lib/qt/include/qcolor.h
mtoplevel.o: /usr/local/lib/qt/include/qcursor.h
mtoplevel.o: /usr/local/lib/qt/include/qfont.h
mtoplevel.o: /usr/local/lib/qt/include/qfontmet.h
mtoplevel.o: /usr/local/lib/qt/include/qfontinf.h
mtoplevel.o: /usr/local/lib/qt/include/qsocknot.h
mtoplevel.o: /usr/local/lib/qt/include/qlined.h
mtoplevel.o: /usr/local/lib/qt/include/qmlined.h
mtoplevel.o: /usr/local/lib/qt/include/qtablevw.h
mtoplevel.o: /usr/local/lib/qt/include/qframe.h
mtoplevel.o: /usr/local/lib/qt/include/qlist.h
mtoplevel.o: /usr/local/lib/qt/include/qglist.h
mtoplevel.o: /usr/local/lib/qt/include/qcollect.h
mtoplevel.o: /usr/local/lib/qt/include/qlayout.h
mtoplevel.o: /usr/local/lib/qt/include/qgmanagr.h
mtoplevel.o: /usr/local/lib/qt/include/qintdict.h
mtoplevel.o: /usr/local/lib/qt/include/qgdict.h
mtoplevel.o: /usr/local/lib/qt/include/qlistbox.h
mtoplevel.o: /usr/local/lib/qt/include/qpixmap.h
mtoplevel.o: /usr/local/lib/qt/include/qscrbar.h
mtoplevel.o: /usr/local/lib/qt/include/qrangect.h
mtoplevel.o: /usr/local/lib/qt/include/qdrawutl.h
mtoplevel.o: /usr/local/lib/qt/include/qpainter.h
mtoplevel.o: /usr/local/lib/qt/include/qregion.h
mtoplevel.o: /usr/local/lib/qt/include/qpen.h
mtoplevel.o: /usr/local/lib/qt/include/qbrush.h
mtoplevel.o: /usr/local/lib/qt/include/qpntarry.h
mtoplevel.o: /usr/local/lib/qt/include/qwmatrix.h
mtoplevel.o: /usr/local/lib/qt/include/qregexp.h
mtoplevel.o: /usr/local/lib/qt/include/qaccel.h
mtoplevel.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
mtoplevel.o: /usr/local/lib/qt/include/qstrlist.h
mtoplevel.o: /usr/local/lib/qt/include/qdstream.h
mtoplevel.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h alistbox.h
mtoplevel.o: messageReceiver.h ksircprocess.h iocontroller.h
mtoplevel.o: /usr/local/lib/qt/include/qdict.h KSTicker/ksticker.h
mtoplevel.o: /usr/local/lib/qt/include/qpopmenu.h
mtoplevel.o: /usr/local/lib/qt/include/qmenudta.h UserMenuRef.h
mtoplevel.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
mtoplevel.o: /usr/local/lib/qt/include/qbutton.h
mtoplevel.o: /usr/local/lib/qt/include/qpushbt.h
mtoplevel.o: /usr/local/lib/qt/include/qcombo.h irclistbox.h
mtoplevel.o: /usr/local/lib/qt/include/qmetaobj.h
mtoplevel.o: /usr/local/lib/qt/include/qconnect.h
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
mahistlineedit.o: ahistlineedit.h /usr/local/lib/qt/include/qlined.h
mahistlineedit.o: /usr/local/lib/qt/include/qwidget.h
mahistlineedit.o: /usr/local/lib/qt/include/qwindefs.h
mahistlineedit.o: /usr/local/lib/qt/include/qobjdefs.h
mahistlineedit.o: /usr/local/lib/qt/include/qglobal.h
mahistlineedit.o: /usr/local/lib/qt/include/qobject.h
mahistlineedit.o: /usr/local/lib/qt/include/qstring.h
mahistlineedit.o: /usr/local/lib/qt/include/qarray.h
mahistlineedit.o: /usr/local/lib/qt/include/qgarray.h
mahistlineedit.o: /usr/local/lib/qt/include/qshared.h
mahistlineedit.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mahistlineedit.o: /usr/include/features.h /usr/include/sys/cdefs.h
mahistlineedit.o: /usr/local/lib/qt/include/qevent.h
mahistlineedit.o: /usr/local/lib/qt/include/qrect.h
mahistlineedit.o: /usr/local/lib/qt/include/qsize.h
mahistlineedit.o: /usr/local/lib/qt/include/qpoint.h
mahistlineedit.o: /usr/local/lib/qt/include/qpaintd.h
mahistlineedit.o: /usr/local/lib/qt/include/qpalette.h
mahistlineedit.o: /usr/local/lib/qt/include/qcolor.h
mahistlineedit.o: /usr/local/lib/qt/include/qcursor.h
mahistlineedit.o: /usr/local/lib/qt/include/qfont.h
mahistlineedit.o: /usr/local/lib/qt/include/qfontmet.h
mahistlineedit.o: /usr/local/lib/qt/include/qfontinf.h
mahistlineedit.o: /usr/local/lib/qt/include/qstrlist.h
mahistlineedit.o: /usr/local/lib/qt/include/qlist.h
mahistlineedit.o: /usr/local/lib/qt/include/qglist.h
mahistlineedit.o: /usr/local/lib/qt/include/qcollect.h
mahistlineedit.o: /usr/local/lib/qt/include/qdstream.h
mahistlineedit.o: /usr/local/lib/qt/include/qiodev.h
mahistlineedit.o: /usr/local/lib/qt/include/qkeycode.h
mahistlineedit.o: /usr/local/lib/qt/include/qmetaobj.h
mahistlineedit.o: /usr/local/lib/qt/include/qconnect.h
malistbox.o: alistbox.h /usr/local/lib/qt/include/qobject.h
malistbox.o: /usr/local/lib/qt/include/qobjdefs.h
malistbox.o: /usr/local/lib/qt/include/qglobal.h
malistbox.o: /usr/local/lib/qt/include/qstring.h
malistbox.o: /usr/local/lib/qt/include/qarray.h
malistbox.o: /usr/local/lib/qt/include/qgarray.h
malistbox.o: /usr/local/lib/qt/include/qshared.h
malistbox.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
malistbox.o: /usr/include/features.h /usr/include/sys/cdefs.h
malistbox.o: /usr/local/lib/qt/include/qevent.h
malistbox.o: /usr/local/lib/qt/include/qwindefs.h
malistbox.o: /usr/local/lib/qt/include/qrect.h
malistbox.o: /usr/local/lib/qt/include/qsize.h
malistbox.o: /usr/local/lib/qt/include/qpoint.h
malistbox.o: /usr/local/lib/qt/include/qwidget.h
malistbox.o: /usr/local/lib/qt/include/qpaintd.h
malistbox.o: /usr/local/lib/qt/include/qpalette.h
malistbox.o: /usr/local/lib/qt/include/qcolor.h
malistbox.o: /usr/local/lib/qt/include/qcursor.h
malistbox.o: /usr/local/lib/qt/include/qfont.h
malistbox.o: /usr/local/lib/qt/include/qfontmet.h
malistbox.o: /usr/local/lib/qt/include/qfontinf.h
malistbox.o: /usr/local/lib/qt/include/qlistbox.h
malistbox.o: /usr/local/lib/qt/include/qtablevw.h
malistbox.o: /usr/local/lib/qt/include/qframe.h
malistbox.o: /usr/local/lib/qt/include/qpixmap.h irclistitem.h
malistbox.o: /usr/local/lib/qt/include/qpainter.h
malistbox.o: /usr/local/lib/qt/include/qregion.h
malistbox.o: /usr/local/lib/qt/include/qpen.h
malistbox.o: /usr/local/lib/qt/include/qbrush.h
malistbox.o: /usr/local/lib/qt/include/qpntarry.h
malistbox.o: /usr/local/lib/qt/include/qwmatrix.h
malistbox.o: /usr/local/lib/qt/include/qstrlist.h
malistbox.o: /usr/local/lib/qt/include/qlist.h
malistbox.o: /usr/local/lib/qt/include/qglist.h
malistbox.o: /usr/local/lib/qt/include/qcollect.h
malistbox.o: /usr/local/lib/qt/include/qdstream.h
malistbox.o: /usr/local/lib/qt/include/qiodev.h
malistbox.o: /usr/local/lib/qt/include/qmetaobj.h
malistbox.o: /usr/local/lib/qt/include/qconnect.h
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
mUserMenuRefData.o: UserMenuRefData.h /usr/local/lib/qt/include/qframe.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qwidget.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qwindefs.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qobjdefs.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qglobal.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qobject.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qstring.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qarray.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qgarray.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qshared.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qgeneric.h
mUserMenuRefData.o: /usr/include/string.h /usr/include/features.h
mUserMenuRefData.o: /usr/include/sys/cdefs.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qevent.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qrect.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qsize.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qpoint.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qpaintd.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qpalette.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qcolor.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qcursor.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qfont.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qfontmet.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qfontinf.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qchkbox.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qbutton.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qpushbt.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qlistbox.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qtablevw.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qpixmap.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qlined.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qcombo.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qmetaobj.h
mUserMenuRefData.o: /usr/local/lib/qt/include/qconnect.h
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
iocontroller.o: KSTicker/ksticker.h /usr/local/lib/qt/include/qpopmenu.h
iocontroller.o: /usr/local/lib/qt/include/qmenudta.h UserMenuRef.h
iocontroller.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
iocontroller.o: /usr/local/lib/qt/include/qbutton.h
iocontroller.o: /usr/local/lib/qt/include/qpushbt.h
iocontroller.o: /usr/local/lib/qt/include/qcombo.h irclistbox.h
iocontroller.o: control_message.h config.h
miocontroller.o: iocontroller.h /usr/local/lib/qt/include/qobject.h
miocontroller.o: /usr/local/lib/qt/include/qobjdefs.h
miocontroller.o: /usr/local/lib/qt/include/qglobal.h
miocontroller.o: /usr/local/lib/qt/include/qstring.h
miocontroller.o: /usr/local/lib/qt/include/qarray.h
miocontroller.o: /usr/local/lib/qt/include/qgarray.h
miocontroller.o: /usr/local/lib/qt/include/qshared.h
miocontroller.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
miocontroller.o: /usr/include/features.h /usr/include/sys/cdefs.h
miocontroller.o: /usr/local/lib/qt/include/qevent.h
miocontroller.o: /usr/local/lib/qt/include/qwindefs.h
miocontroller.o: /usr/local/lib/qt/include/qrect.h
miocontroller.o: /usr/local/lib/qt/include/qsize.h
miocontroller.o: /usr/local/lib/qt/include/qpoint.h
miocontroller.o: /usr/local/lib/qt/include/qwidget.h
miocontroller.o: /usr/local/lib/qt/include/qpaintd.h
miocontroller.o: /usr/local/lib/qt/include/qpalette.h
miocontroller.o: /usr/local/lib/qt/include/qcolor.h
miocontroller.o: /usr/local/lib/qt/include/qcursor.h
miocontroller.o: /usr/local/lib/qt/include/qfont.h
miocontroller.o: /usr/local/lib/qt/include/qfontmet.h
miocontroller.o: /usr/local/lib/qt/include/qfontinf.h
miocontroller.o: /usr/local/lib/qt/include/qsocknot.h
miocontroller.o: /usr/local/lib/qt/include/qdict.h
miocontroller.o: /usr/local/lib/qt/include/qgdict.h
miocontroller.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
miocontroller.o: ksircprocess.h /usr/local/lib/qt/include/qmetaobj.h
miocontroller.o: /usr/local/lib/qt/include/qconnect.h
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
open_top.o: KSTicker/ksticker.h /usr/local/lib/qt/include/qpopmenu.h
open_top.o: /usr/local/lib/qt/include/qmenudta.h UserMenuRef.h
open_top.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
open_top.o: irclistbox.h
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
mopen_top.o: open_top.h open_topData.h /usr/local/lib/qt/include/qdialog.h
mopen_top.o: /usr/local/lib/qt/include/qwidget.h
mopen_top.o: /usr/local/lib/qt/include/qwindefs.h
mopen_top.o: /usr/local/lib/qt/include/qobjdefs.h
mopen_top.o: /usr/local/lib/qt/include/qglobal.h
mopen_top.o: /usr/local/lib/qt/include/qobject.h
mopen_top.o: /usr/local/lib/qt/include/qstring.h
mopen_top.o: /usr/local/lib/qt/include/qarray.h
mopen_top.o: /usr/local/lib/qt/include/qgarray.h
mopen_top.o: /usr/local/lib/qt/include/qshared.h
mopen_top.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mopen_top.o: /usr/include/features.h /usr/include/sys/cdefs.h
mopen_top.o: /usr/local/lib/qt/include/qevent.h
mopen_top.o: /usr/local/lib/qt/include/qrect.h
mopen_top.o: /usr/local/lib/qt/include/qsize.h
mopen_top.o: /usr/local/lib/qt/include/qpoint.h
mopen_top.o: /usr/local/lib/qt/include/qpaintd.h
mopen_top.o: /usr/local/lib/qt/include/qpalette.h
mopen_top.o: /usr/local/lib/qt/include/qcolor.h
mopen_top.o: /usr/local/lib/qt/include/qcursor.h
mopen_top.o: /usr/local/lib/qt/include/qfont.h
mopen_top.o: /usr/local/lib/qt/include/qfontmet.h
mopen_top.o: /usr/local/lib/qt/include/qfontinf.h
mopen_top.o: /usr/local/lib/qt/include/qpushbt.h
mopen_top.o: /usr/local/lib/qt/include/qbutton.h
mopen_top.o: /usr/local/lib/qt/include/qcombo.h messageReceiver.h toplevel.h
mopen_top.o: /usr/include/unistd.h /usr/include/posix_opt.h
mopen_top.o: /usr/include/gnu/types.h /usr/include/confname.h
mopen_top.o: /usr/include/sys/types.h /usr/include/linux/types.h
mopen_top.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
mopen_top.o: /usr/include/asm/types.h /usr/include/sys/bitypes.h
mopen_top.o: /usr/include/signal.h /usr/include/linux/signal.h
mopen_top.o: /usr/include/asm/signal.h /usr/local/lib/qt/include/qsocknot.h
mopen_top.o: /usr/local/lib/qt/include/qlined.h
mopen_top.o: /usr/local/lib/qt/include/qmlined.h
mopen_top.o: /usr/local/lib/qt/include/qtablevw.h
mopen_top.o: /usr/local/lib/qt/include/qframe.h
mopen_top.o: /usr/local/lib/qt/include/qlist.h
mopen_top.o: /usr/local/lib/qt/include/qglist.h
mopen_top.o: /usr/local/lib/qt/include/qcollect.h
mopen_top.o: /usr/local/lib/qt/include/qlayout.h
mopen_top.o: /usr/local/lib/qt/include/qgmanagr.h
mopen_top.o: /usr/local/lib/qt/include/qintdict.h
mopen_top.o: /usr/local/lib/qt/include/qgdict.h
mopen_top.o: /usr/local/lib/qt/include/qlistbox.h
mopen_top.o: /usr/local/lib/qt/include/qpixmap.h
mopen_top.o: /usr/local/lib/qt/include/qscrbar.h
mopen_top.o: /usr/local/lib/qt/include/qrangect.h
mopen_top.o: /usr/local/lib/qt/include/qdrawutl.h
mopen_top.o: /usr/local/lib/qt/include/qpainter.h
mopen_top.o: /usr/local/lib/qt/include/qregion.h
mopen_top.o: /usr/local/lib/qt/include/qpen.h
mopen_top.o: /usr/local/lib/qt/include/qbrush.h
mopen_top.o: /usr/local/lib/qt/include/qpntarry.h
mopen_top.o: /usr/local/lib/qt/include/qwmatrix.h
mopen_top.o: /usr/local/lib/qt/include/qregexp.h
mopen_top.o: /usr/local/lib/qt/include/qaccel.h
mopen_top.o: /usr/local/lib/qt/include/qkeycode.h irclistitem.h
mopen_top.o: /usr/local/lib/qt/include/qstrlist.h
mopen_top.o: /usr/local/lib/qt/include/qdstream.h
mopen_top.o: /usr/local/lib/qt/include/qiodev.h ahistlineedit.h alistbox.h
mopen_top.o: ksircprocess.h iocontroller.h /usr/local/lib/qt/include/qdict.h
mopen_top.o: KSTicker/ksticker.h /usr/local/lib/qt/include/qpopmenu.h
mopen_top.o: /usr/local/lib/qt/include/qmenudta.h UserMenuRef.h
mopen_top.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
mopen_top.o: irclistbox.h /usr/local/lib/qt/include/qmetaobj.h
mopen_top.o: /usr/local/lib/qt/include/qconnect.h
mopen_topData.o: open_topData.h /usr/local/lib/qt/include/qdialog.h
mopen_topData.o: /usr/local/lib/qt/include/qwidget.h
mopen_topData.o: /usr/local/lib/qt/include/qwindefs.h
mopen_topData.o: /usr/local/lib/qt/include/qobjdefs.h
mopen_topData.o: /usr/local/lib/qt/include/qglobal.h
mopen_topData.o: /usr/local/lib/qt/include/qobject.h
mopen_topData.o: /usr/local/lib/qt/include/qstring.h
mopen_topData.o: /usr/local/lib/qt/include/qarray.h
mopen_topData.o: /usr/local/lib/qt/include/qgarray.h
mopen_topData.o: /usr/local/lib/qt/include/qshared.h
mopen_topData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mopen_topData.o: /usr/include/features.h /usr/include/sys/cdefs.h
mopen_topData.o: /usr/local/lib/qt/include/qevent.h
mopen_topData.o: /usr/local/lib/qt/include/qrect.h
mopen_topData.o: /usr/local/lib/qt/include/qsize.h
mopen_topData.o: /usr/local/lib/qt/include/qpoint.h
mopen_topData.o: /usr/local/lib/qt/include/qpaintd.h
mopen_topData.o: /usr/local/lib/qt/include/qpalette.h
mopen_topData.o: /usr/local/lib/qt/include/qcolor.h
mopen_topData.o: /usr/local/lib/qt/include/qcursor.h
mopen_topData.o: /usr/local/lib/qt/include/qfont.h
mopen_topData.o: /usr/local/lib/qt/include/qfontmet.h
mopen_topData.o: /usr/local/lib/qt/include/qfontinf.h
mopen_topData.o: /usr/local/lib/qt/include/qpushbt.h
mopen_topData.o: /usr/local/lib/qt/include/qbutton.h
mopen_topData.o: /usr/local/lib/qt/include/qcombo.h
mopen_topData.o: /usr/local/lib/qt/include/qmetaobj.h
mopen_topData.o: /usr/local/lib/qt/include/qconnect.h
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
ksircprocess.o: KSTicker/ksticker.h /usr/local/lib/qt/include/qpopmenu.h
ksircprocess.o: /usr/local/lib/qt/include/qmenudta.h UserMenuRef.h
ksircprocess.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
ksircprocess.o: /usr/local/lib/qt/include/qbutton.h
ksircprocess.o: /usr/local/lib/qt/include/qpushbt.h
ksircprocess.o: /usr/local/lib/qt/include/qcombo.h irclistbox.h ioBroadcast.h
ksircprocess.o: ioDiscard.h ioDCC.h /usr/local/lib/qt/include/qprogdlg.h
ksircprocess.o: /usr/local/lib/qt/include/qsemimodal.h
ksircprocess.o: /usr/local/lib/qt/include/qlabel.h
ksircprocess.o: /usr/local/lib/qt/include/qprogbar.h KSProgress/ksprogress.h
ksircprocess.o: KSProgress/ksprogressdata.h dccDialog.h dccDialogData.h
mksircprocess.o: ksircprocess.h /usr/local/lib/qt/include/qobject.h
mksircprocess.o: /usr/local/lib/qt/include/qobjdefs.h
mksircprocess.o: /usr/local/lib/qt/include/qglobal.h
mksircprocess.o: /usr/local/lib/qt/include/qstring.h
mksircprocess.o: /usr/local/lib/qt/include/qarray.h
mksircprocess.o: /usr/local/lib/qt/include/qgarray.h
mksircprocess.o: /usr/local/lib/qt/include/qshared.h
mksircprocess.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mksircprocess.o: /usr/include/features.h /usr/include/sys/cdefs.h
mksircprocess.o: /usr/local/lib/qt/include/qevent.h
mksircprocess.o: /usr/local/lib/qt/include/qwindefs.h
mksircprocess.o: /usr/local/lib/qt/include/qrect.h
mksircprocess.o: /usr/local/lib/qt/include/qsize.h
mksircprocess.o: /usr/local/lib/qt/include/qpoint.h iocontroller.h
mksircprocess.o: /usr/local/lib/qt/include/qwidget.h
mksircprocess.o: /usr/local/lib/qt/include/qpaintd.h
mksircprocess.o: /usr/local/lib/qt/include/qpalette.h
mksircprocess.o: /usr/local/lib/qt/include/qcolor.h
mksircprocess.o: /usr/local/lib/qt/include/qcursor.h
mksircprocess.o: /usr/local/lib/qt/include/qfont.h
mksircprocess.o: /usr/local/lib/qt/include/qfontmet.h
mksircprocess.o: /usr/local/lib/qt/include/qfontinf.h
mksircprocess.o: /usr/local/lib/qt/include/qsocknot.h
mksircprocess.o: /usr/local/lib/qt/include/qdict.h
mksircprocess.o: /usr/local/lib/qt/include/qgdict.h
mksircprocess.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
mksircprocess.o: /usr/local/lib/qt/include/qmetaobj.h
mksircprocess.o: /usr/local/lib/qt/include/qconnect.h
mksircprocess.o: /usr/local/lib/qt/include/qlist.h
mksircprocess.o: /usr/local/lib/qt/include/qglist.h
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
mopen_ksirc.o: open_ksirc.h open_ksircData.h
mopen_ksirc.o: /usr/local/lib/qt/include/qdialog.h
mopen_ksirc.o: /usr/local/lib/qt/include/qwidget.h
mopen_ksirc.o: /usr/local/lib/qt/include/qwindefs.h
mopen_ksirc.o: /usr/local/lib/qt/include/qobjdefs.h
mopen_ksirc.o: /usr/local/lib/qt/include/qglobal.h
mopen_ksirc.o: /usr/local/lib/qt/include/qobject.h
mopen_ksirc.o: /usr/local/lib/qt/include/qstring.h
mopen_ksirc.o: /usr/local/lib/qt/include/qarray.h
mopen_ksirc.o: /usr/local/lib/qt/include/qgarray.h
mopen_ksirc.o: /usr/local/lib/qt/include/qshared.h
mopen_ksirc.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mopen_ksirc.o: /usr/include/features.h /usr/include/sys/cdefs.h
mopen_ksirc.o: /usr/local/lib/qt/include/qevent.h
mopen_ksirc.o: /usr/local/lib/qt/include/qrect.h
mopen_ksirc.o: /usr/local/lib/qt/include/qsize.h
mopen_ksirc.o: /usr/local/lib/qt/include/qpoint.h
mopen_ksirc.o: /usr/local/lib/qt/include/qpaintd.h
mopen_ksirc.o: /usr/local/lib/qt/include/qpalette.h
mopen_ksirc.o: /usr/local/lib/qt/include/qcolor.h
mopen_ksirc.o: /usr/local/lib/qt/include/qcursor.h
mopen_ksirc.o: /usr/local/lib/qt/include/qfont.h
mopen_ksirc.o: /usr/local/lib/qt/include/qfontmet.h
mopen_ksirc.o: /usr/local/lib/qt/include/qfontinf.h
mopen_ksirc.o: /usr/local/lib/qt/include/qpushbt.h
mopen_ksirc.o: /usr/local/lib/qt/include/qbutton.h
mopen_ksirc.o: /usr/local/lib/qt/include/qcombo.h iocontroller.h
mopen_ksirc.o: /usr/local/lib/qt/include/qsocknot.h
mopen_ksirc.o: /usr/local/lib/qt/include/qdict.h
mopen_ksirc.o: /usr/local/lib/qt/include/qgdict.h
mopen_ksirc.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
mopen_ksirc.o: ksircprocess.h /usr/local/lib/qt/include/qmetaobj.h
mopen_ksirc.o: /usr/local/lib/qt/include/qconnect.h
mopen_ksirc.o: /usr/local/lib/qt/include/qlist.h
mopen_ksirc.o: /usr/local/lib/qt/include/qglist.h
mopen_ksircData.o: open_ksircData.h /usr/local/lib/qt/include/qdialog.h
mopen_ksircData.o: /usr/local/lib/qt/include/qwidget.h
mopen_ksircData.o: /usr/local/lib/qt/include/qwindefs.h
mopen_ksircData.o: /usr/local/lib/qt/include/qobjdefs.h
mopen_ksircData.o: /usr/local/lib/qt/include/qglobal.h
mopen_ksircData.o: /usr/local/lib/qt/include/qobject.h
mopen_ksircData.o: /usr/local/lib/qt/include/qstring.h
mopen_ksircData.o: /usr/local/lib/qt/include/qarray.h
mopen_ksircData.o: /usr/local/lib/qt/include/qgarray.h
mopen_ksircData.o: /usr/local/lib/qt/include/qshared.h
mopen_ksircData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mopen_ksircData.o: /usr/include/features.h /usr/include/sys/cdefs.h
mopen_ksircData.o: /usr/local/lib/qt/include/qevent.h
mopen_ksircData.o: /usr/local/lib/qt/include/qrect.h
mopen_ksircData.o: /usr/local/lib/qt/include/qsize.h
mopen_ksircData.o: /usr/local/lib/qt/include/qpoint.h
mopen_ksircData.o: /usr/local/lib/qt/include/qpaintd.h
mopen_ksircData.o: /usr/local/lib/qt/include/qpalette.h
mopen_ksircData.o: /usr/local/lib/qt/include/qcolor.h
mopen_ksircData.o: /usr/local/lib/qt/include/qcursor.h
mopen_ksircData.o: /usr/local/lib/qt/include/qfont.h
mopen_ksircData.o: /usr/local/lib/qt/include/qfontmet.h
mopen_ksircData.o: /usr/local/lib/qt/include/qfontinf.h
mopen_ksircData.o: /usr/local/lib/qt/include/qpushbt.h
mopen_ksircData.o: /usr/local/lib/qt/include/qbutton.h
mopen_ksircData.o: /usr/local/lib/qt/include/qcombo.h
mopen_ksircData.o: /usr/local/lib/qt/include/qmetaobj.h
mopen_ksircData.o: /usr/local/lib/qt/include/qconnect.h
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
servercontroller.o: /usr/local/lib/qt/include/qpixmap.h
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
servercontroller.o: alistbox.h KSTicker/ksticker.h UserMenuRef.h
servercontroller.o: UserMenuRefData.h /usr/local/lib/qt/include/qchkbox.h
servercontroller.o: irclistbox.h KSircColour.h KSircColourData.h config.h
servercontroller.o: control_message.h FilterRuleEditor.h
servercontroller.o: FilterRuleEditorData.h
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
mservercontroller.o: servercontroller.h /usr/local/lib/qt/include/qdict.h
mservercontroller.o: /usr/local/lib/qt/include/qgdict.h
mservercontroller.o: /usr/local/lib/qt/include/qcollect.h
mservercontroller.o: /usr/local/lib/qt/include/qglobal.h
mservercontroller.o: /usr/local/lib/qt/include/qgeneric.h
mservercontroller.o: /usr/local/lib/qt/include/qpixmap.h
mservercontroller.o: /usr/local/lib/qt/include/qpaintd.h
mservercontroller.o: /usr/local/lib/qt/include/qwindefs.h
mservercontroller.o: /usr/local/lib/qt/include/qobjdefs.h
mservercontroller.o: /usr/local/lib/qt/include/qrect.h
mservercontroller.o: /usr/local/lib/qt/include/qsize.h
mservercontroller.o: /usr/local/lib/qt/include/qpoint.h
mservercontroller.o: /usr/local/lib/qt/include/qcolor.h
mservercontroller.o: /usr/local/lib/qt/include/qshared.h
mservercontroller.o: servercontrollerData.h control.h
mservercontroller.o: /usr/local/lib/qt/include/qframe.h
mservercontroller.o: /usr/local/lib/qt/include/qwidget.h
mservercontroller.o: /usr/local/lib/qt/include/qobject.h
mservercontroller.o: /usr/local/lib/qt/include/qstring.h
mservercontroller.o: /usr/local/lib/qt/include/qarray.h
mservercontroller.o: /usr/local/lib/qt/include/qgarray.h
mservercontroller.o: /usr/include/string.h /usr/include/features.h
mservercontroller.o: /usr/include/sys/cdefs.h
mservercontroller.o: /usr/local/lib/qt/include/qevent.h
mservercontroller.o: /usr/local/lib/qt/include/qpalette.h
mservercontroller.o: /usr/local/lib/qt/include/qcursor.h
mservercontroller.o: /usr/local/lib/qt/include/qfont.h
mservercontroller.o: /usr/local/lib/qt/include/qfontmet.h
mservercontroller.o: /usr/local/lib/qt/include/qfontinf.h ksircprocess.h
mservercontroller.o: iocontroller.h /usr/local/lib/qt/include/qsocknot.h
mservercontroller.o: messageReceiver.h /usr/local/lib/qt/include/qmetaobj.h
mservercontroller.o: /usr/local/lib/qt/include/qconnect.h
mservercontrollerData.o: servercontrollerData.h control.h
mservercontrollerData.o: /usr/local/lib/qt/include/qframe.h
mservercontrollerData.o: /usr/local/lib/qt/include/qwidget.h
mservercontrollerData.o: /usr/local/lib/qt/include/qwindefs.h
mservercontrollerData.o: /usr/local/lib/qt/include/qobjdefs.h
mservercontrollerData.o: /usr/local/lib/qt/include/qglobal.h
mservercontrollerData.o: /usr/local/lib/qt/include/qobject.h
mservercontrollerData.o: /usr/local/lib/qt/include/qstring.h
mservercontrollerData.o: /usr/local/lib/qt/include/qarray.h
mservercontrollerData.o: /usr/local/lib/qt/include/qgarray.h
mservercontrollerData.o: /usr/local/lib/qt/include/qshared.h
mservercontrollerData.o: /usr/local/lib/qt/include/qgeneric.h
mservercontrollerData.o: /usr/include/string.h /usr/include/features.h
mservercontrollerData.o: /usr/include/sys/cdefs.h
mservercontrollerData.o: /usr/local/lib/qt/include/qevent.h
mservercontrollerData.o: /usr/local/lib/qt/include/qrect.h
mservercontrollerData.o: /usr/local/lib/qt/include/qsize.h
mservercontrollerData.o: /usr/local/lib/qt/include/qpoint.h
mservercontrollerData.o: /usr/local/lib/qt/include/qpaintd.h
mservercontrollerData.o: /usr/local/lib/qt/include/qpalette.h
mservercontrollerData.o: /usr/local/lib/qt/include/qcolor.h
mservercontrollerData.o: /usr/local/lib/qt/include/qcursor.h
mservercontrollerData.o: /usr/local/lib/qt/include/qfont.h
mservercontrollerData.o: /usr/local/lib/qt/include/qfontmet.h
mservercontrollerData.o: /usr/local/lib/qt/include/qfontinf.h
mservercontrollerData.o: /usr/local/lib/qt/include/qmetaobj.h
mservercontrollerData.o: /usr/local/lib/qt/include/qconnect.h
control.o: control.h
mcontrol.o: control.h /usr/local/lib/qt/include/qmetaobj.h
mcontrol.o: /usr/local/lib/qt/include/qconnect.h
mcontrol.o: /usr/local/lib/qt/include/qobject.h
mcontrol.o: /usr/local/lib/qt/include/qobjdefs.h
mcontrol.o: /usr/local/lib/qt/include/qglobal.h
mcontrol.o: /usr/local/lib/qt/include/qstring.h
mcontrol.o: /usr/local/lib/qt/include/qarray.h
mcontrol.o: /usr/local/lib/qt/include/qgarray.h
mcontrol.o: /usr/local/lib/qt/include/qshared.h
mcontrol.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mcontrol.o: /usr/include/features.h /usr/include/sys/cdefs.h
mcontrol.o: /usr/local/lib/qt/include/qevent.h
mcontrol.o: /usr/local/lib/qt/include/qwindefs.h
mcontrol.o: /usr/local/lib/qt/include/qrect.h
mcontrol.o: /usr/local/lib/qt/include/qsize.h
mcontrol.o: /usr/local/lib/qt/include/qpoint.h
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
mKSircColour.o: KSircColour.h KSircColourData.h
mKSircColour.o: /usr/local/lib/qt/include/qframe.h
mKSircColour.o: /usr/local/lib/qt/include/qwidget.h
mKSircColour.o: /usr/local/lib/qt/include/qwindefs.h
mKSircColour.o: /usr/local/lib/qt/include/qobjdefs.h
mKSircColour.o: /usr/local/lib/qt/include/qglobal.h
mKSircColour.o: /usr/local/lib/qt/include/qobject.h
mKSircColour.o: /usr/local/lib/qt/include/qstring.h
mKSircColour.o: /usr/local/lib/qt/include/qarray.h
mKSircColour.o: /usr/local/lib/qt/include/qgarray.h
mKSircColour.o: /usr/local/lib/qt/include/qshared.h
mKSircColour.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mKSircColour.o: /usr/include/features.h /usr/include/sys/cdefs.h
mKSircColour.o: /usr/local/lib/qt/include/qevent.h
mKSircColour.o: /usr/local/lib/qt/include/qrect.h
mKSircColour.o: /usr/local/lib/qt/include/qsize.h
mKSircColour.o: /usr/local/lib/qt/include/qpoint.h
mKSircColour.o: /usr/local/lib/qt/include/qpaintd.h
mKSircColour.o: /usr/local/lib/qt/include/qpalette.h
mKSircColour.o: /usr/local/lib/qt/include/qcolor.h
mKSircColour.o: /usr/local/lib/qt/include/qcursor.h
mKSircColour.o: /usr/local/lib/qt/include/qfont.h
mKSircColour.o: /usr/local/lib/qt/include/qfontmet.h
mKSircColour.o: /usr/local/lib/qt/include/qfontinf.h
mKSircColour.o: /usr/local/lib/qt/include/qmetaobj.h
mKSircColour.o: /usr/local/lib/qt/include/qconnect.h
mKSircColourData.o: KSircColourData.h /usr/local/lib/qt/include/qframe.h
mKSircColourData.o: /usr/local/lib/qt/include/qwidget.h
mKSircColourData.o: /usr/local/lib/qt/include/qwindefs.h
mKSircColourData.o: /usr/local/lib/qt/include/qobjdefs.h
mKSircColourData.o: /usr/local/lib/qt/include/qglobal.h
mKSircColourData.o: /usr/local/lib/qt/include/qobject.h
mKSircColourData.o: /usr/local/lib/qt/include/qstring.h
mKSircColourData.o: /usr/local/lib/qt/include/qarray.h
mKSircColourData.o: /usr/local/lib/qt/include/qgarray.h
mKSircColourData.o: /usr/local/lib/qt/include/qshared.h
mKSircColourData.o: /usr/local/lib/qt/include/qgeneric.h
mKSircColourData.o: /usr/include/string.h /usr/include/features.h
mKSircColourData.o: /usr/include/sys/cdefs.h
mKSircColourData.o: /usr/local/lib/qt/include/qevent.h
mKSircColourData.o: /usr/local/lib/qt/include/qrect.h
mKSircColourData.o: /usr/local/lib/qt/include/qsize.h
mKSircColourData.o: /usr/local/lib/qt/include/qpoint.h
mKSircColourData.o: /usr/local/lib/qt/include/qpaintd.h
mKSircColourData.o: /usr/local/lib/qt/include/qpalette.h
mKSircColourData.o: /usr/local/lib/qt/include/qcolor.h
mKSircColourData.o: /usr/local/lib/qt/include/qcursor.h
mKSircColourData.o: /usr/local/lib/qt/include/qfont.h
mKSircColourData.o: /usr/local/lib/qt/include/qfontmet.h
mKSircColourData.o: /usr/local/lib/qt/include/qfontinf.h
mKSircColourData.o: /usr/local/lib/qt/include/qmetaobj.h
mKSircColourData.o: /usr/local/lib/qt/include/qconnect.h
FilterRuleEditor.o: FilterRuleEditor.h FilterRuleEditorData.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qdialog.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qwidget.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qwindefs.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qobjdefs.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qglobal.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qobject.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qstring.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qarray.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qgarray.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qshared.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qgeneric.h
FilterRuleEditor.o: /usr/include/string.h /usr/include/features.h
FilterRuleEditor.o: /usr/include/sys/cdefs.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qevent.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qrect.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qsize.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qpoint.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qpaintd.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qpalette.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qcolor.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qcursor.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qfont.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qfontmet.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qfontinf.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qpushbt.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qbutton.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qlistbox.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qtablevw.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qframe.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qpixmap.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qlined.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qmsgbox.h
FilterRuleEditor.o: /usr/local/lib/qt/include/qregexp.h
FilterRuleEditorData.o: FilterRuleEditorData.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qdialog.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qwidget.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qwindefs.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qobjdefs.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qglobal.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qobject.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qstring.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qarray.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qgarray.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qshared.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qgeneric.h
FilterRuleEditorData.o: /usr/include/string.h /usr/include/features.h
FilterRuleEditorData.o: /usr/include/sys/cdefs.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qevent.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qrect.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qsize.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qpoint.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qpaintd.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qpalette.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qcolor.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qcursor.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qfont.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qfontmet.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qfontinf.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qpushbt.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qbutton.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qlistbox.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qtablevw.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qframe.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qpixmap.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qlined.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qlabel.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qlayout.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qgmanagr.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qintdict.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qgdict.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qcollect.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qlist.h
FilterRuleEditorData.o: /usr/local/lib/qt/include/qglist.h
mFilterRuleEditor.o: FilterRuleEditor.h FilterRuleEditorData.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qdialog.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qwidget.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qwindefs.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qobjdefs.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qglobal.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qobject.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qstring.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qarray.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qgarray.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qshared.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qgeneric.h
mFilterRuleEditor.o: /usr/include/string.h /usr/include/features.h
mFilterRuleEditor.o: /usr/include/sys/cdefs.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qevent.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qrect.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qsize.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qpoint.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qpaintd.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qpalette.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qcolor.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qcursor.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qfont.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qfontmet.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qfontinf.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qpushbt.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qbutton.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qlistbox.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qtablevw.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qframe.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qpixmap.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qlined.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qmetaobj.h
mFilterRuleEditor.o: /usr/local/lib/qt/include/qconnect.h
mFilterRuleEditorData.o: FilterRuleEditorData.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qdialog.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qwidget.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qwindefs.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qobjdefs.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qglobal.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qobject.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qstring.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qarray.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qgarray.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qshared.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qgeneric.h
mFilterRuleEditorData.o: /usr/include/string.h /usr/include/features.h
mFilterRuleEditorData.o: /usr/include/sys/cdefs.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qevent.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qrect.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qsize.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qpoint.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qpaintd.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qpalette.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qcolor.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qcursor.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qfont.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qfontmet.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qfontinf.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qpushbt.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qbutton.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qlistbox.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qtablevw.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qframe.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qpixmap.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qlined.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qmetaobj.h
mFilterRuleEditorData.o: /usr/local/lib/qt/include/qconnect.h
ioDiscard.o: ioDiscard.h /usr/local/lib/qt/include/qobject.h
ioDiscard.o: /usr/local/lib/qt/include/qobjdefs.h
ioDiscard.o: /usr/local/lib/qt/include/qglobal.h
ioDiscard.o: /usr/local/lib/qt/include/qstring.h
ioDiscard.o: /usr/local/lib/qt/include/qarray.h
ioDiscard.o: /usr/local/lib/qt/include/qgarray.h
ioDiscard.o: /usr/local/lib/qt/include/qshared.h
ioDiscard.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
ioDiscard.o: /usr/include/features.h /usr/include/sys/cdefs.h
ioDiscard.o: /usr/local/lib/qt/include/qevent.h
ioDiscard.o: /usr/local/lib/qt/include/qwindefs.h
ioDiscard.o: /usr/local/lib/qt/include/qrect.h
ioDiscard.o: /usr/local/lib/qt/include/qsize.h
ioDiscard.o: /usr/local/lib/qt/include/qpoint.h
ioDiscard.o: /usr/local/lib/qt/include/qdict.h
ioDiscard.o: /usr/local/lib/qt/include/qgdict.h
ioDiscard.o: /usr/local/lib/qt/include/qcollect.h messageReceiver.h
ioDiscard.o: ksircprocess.h iocontroller.h
ioDiscard.o: /usr/local/lib/qt/include/qwidget.h
ioDiscard.o: /usr/local/lib/qt/include/qpaintd.h
ioDiscard.o: /usr/local/lib/qt/include/qpalette.h
ioDiscard.o: /usr/local/lib/qt/include/qcolor.h
ioDiscard.o: /usr/local/lib/qt/include/qcursor.h
ioDiscard.o: /usr/local/lib/qt/include/qfont.h
ioDiscard.o: /usr/local/lib/qt/include/qfontmet.h
ioDiscard.o: /usr/local/lib/qt/include/qfontinf.h
ioDiscard.o: /usr/local/lib/qt/include/qsocknot.h
ioDCC.o: ioDCC.h /usr/local/lib/qt/include/qobject.h
ioDCC.o: /usr/local/lib/qt/include/qobjdefs.h
ioDCC.o: /usr/local/lib/qt/include/qglobal.h
ioDCC.o: /usr/local/lib/qt/include/qstring.h
ioDCC.o: /usr/local/lib/qt/include/qarray.h
ioDCC.o: /usr/local/lib/qt/include/qgarray.h
ioDCC.o: /usr/local/lib/qt/include/qshared.h
ioDCC.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
ioDCC.o: /usr/include/features.h /usr/include/sys/cdefs.h
ioDCC.o: /usr/local/lib/qt/include/qevent.h
ioDCC.o: /usr/local/lib/qt/include/qwindefs.h
ioDCC.o: /usr/local/lib/qt/include/qrect.h /usr/local/lib/qt/include/qsize.h
ioDCC.o: /usr/local/lib/qt/include/qpoint.h /usr/local/lib/qt/include/qdict.h
ioDCC.o: /usr/local/lib/qt/include/qgdict.h
ioDCC.o: /usr/local/lib/qt/include/qcollect.h
ioDCC.o: /usr/local/lib/qt/include/qprogdlg.h
ioDCC.o: /usr/local/lib/qt/include/qsemimodal.h
ioDCC.o: /usr/local/lib/qt/include/qwidget.h
ioDCC.o: /usr/local/lib/qt/include/qpaintd.h
ioDCC.o: /usr/local/lib/qt/include/qpalette.h
ioDCC.o: /usr/local/lib/qt/include/qcolor.h
ioDCC.o: /usr/local/lib/qt/include/qcursor.h
ioDCC.o: /usr/local/lib/qt/include/qfont.h
ioDCC.o: /usr/local/lib/qt/include/qfontmet.h
ioDCC.o: /usr/local/lib/qt/include/qfontinf.h
ioDCC.o: /usr/local/lib/qt/include/qpushbt.h
ioDCC.o: /usr/local/lib/qt/include/qbutton.h
ioDCC.o: /usr/local/lib/qt/include/qlabel.h
ioDCC.o: /usr/local/lib/qt/include/qframe.h
ioDCC.o: /usr/local/lib/qt/include/qprogbar.h KSProgress/ksprogress.h
ioDCC.o: KSProgress/ksprogressdata.h messageReceiver.h ksircprocess.h
ioDCC.o: iocontroller.h /usr/local/lib/qt/include/qsocknot.h dccDialog.h
ioDCC.o: dccDialogData.h /usr/local/lib/qt/include/qlistbox.h
ioDCC.o: /usr/local/lib/qt/include/qtablevw.h
ioDCC.o: /usr/local/lib/qt/include/qpixmap.h
ioDCC.o: /usr/local/lib/qt/include/qregexp.h
mioDCC.o: ioDCC.h /usr/local/lib/qt/include/qobject.h
mioDCC.o: /usr/local/lib/qt/include/qobjdefs.h
mioDCC.o: /usr/local/lib/qt/include/qglobal.h
mioDCC.o: /usr/local/lib/qt/include/qstring.h
mioDCC.o: /usr/local/lib/qt/include/qarray.h
mioDCC.o: /usr/local/lib/qt/include/qgarray.h
mioDCC.o: /usr/local/lib/qt/include/qshared.h
mioDCC.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mioDCC.o: /usr/include/features.h /usr/include/sys/cdefs.h
mioDCC.o: /usr/local/lib/qt/include/qevent.h
mioDCC.o: /usr/local/lib/qt/include/qwindefs.h
mioDCC.o: /usr/local/lib/qt/include/qrect.h /usr/local/lib/qt/include/qsize.h
mioDCC.o: /usr/local/lib/qt/include/qpoint.h
mioDCC.o: /usr/local/lib/qt/include/qdict.h
mioDCC.o: /usr/local/lib/qt/include/qgdict.h
mioDCC.o: /usr/local/lib/qt/include/qcollect.h
mioDCC.o: /usr/local/lib/qt/include/qprogdlg.h
mioDCC.o: /usr/local/lib/qt/include/qsemimodal.h
mioDCC.o: /usr/local/lib/qt/include/qwidget.h
mioDCC.o: /usr/local/lib/qt/include/qpaintd.h
mioDCC.o: /usr/local/lib/qt/include/qpalette.h
mioDCC.o: /usr/local/lib/qt/include/qcolor.h
mioDCC.o: /usr/local/lib/qt/include/qcursor.h
mioDCC.o: /usr/local/lib/qt/include/qfont.h
mioDCC.o: /usr/local/lib/qt/include/qfontmet.h
mioDCC.o: /usr/local/lib/qt/include/qfontinf.h
mioDCC.o: /usr/local/lib/qt/include/qpushbt.h
mioDCC.o: /usr/local/lib/qt/include/qbutton.h
mioDCC.o: /usr/local/lib/qt/include/qlabel.h
mioDCC.o: /usr/local/lib/qt/include/qframe.h
mioDCC.o: /usr/local/lib/qt/include/qprogbar.h KSProgress/ksprogress.h
mioDCC.o: KSProgress/ksprogressdata.h messageReceiver.h ksircprocess.h
mioDCC.o: iocontroller.h /usr/local/lib/qt/include/qsocknot.h dccDialog.h
mioDCC.o: dccDialogData.h /usr/local/lib/qt/include/qlistbox.h
mioDCC.o: /usr/local/lib/qt/include/qtablevw.h
mioDCC.o: /usr/local/lib/qt/include/qpixmap.h
mioDCC.o: /usr/local/lib/qt/include/qmetaobj.h
mioDCC.o: /usr/local/lib/qt/include/qconnect.h
mioDCC.o: /usr/local/lib/qt/include/qlist.h
mioDCC.o: /usr/local/lib/qt/include/qglist.h
dccDialog.o: dccDialog.h dccDialogData.h /usr/local/lib/qt/include/qframe.h
dccDialog.o: /usr/local/lib/qt/include/qwidget.h
dccDialog.o: /usr/local/lib/qt/include/qwindefs.h
dccDialog.o: /usr/local/lib/qt/include/qobjdefs.h
dccDialog.o: /usr/local/lib/qt/include/qglobal.h
dccDialog.o: /usr/local/lib/qt/include/qobject.h
dccDialog.o: /usr/local/lib/qt/include/qstring.h
dccDialog.o: /usr/local/lib/qt/include/qarray.h
dccDialog.o: /usr/local/lib/qt/include/qgarray.h
dccDialog.o: /usr/local/lib/qt/include/qshared.h
dccDialog.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
dccDialog.o: /usr/include/features.h /usr/include/sys/cdefs.h
dccDialog.o: /usr/local/lib/qt/include/qevent.h
dccDialog.o: /usr/local/lib/qt/include/qrect.h
dccDialog.o: /usr/local/lib/qt/include/qsize.h
dccDialog.o: /usr/local/lib/qt/include/qpoint.h
dccDialog.o: /usr/local/lib/qt/include/qpaintd.h
dccDialog.o: /usr/local/lib/qt/include/qpalette.h
dccDialog.o: /usr/local/lib/qt/include/qcolor.h
dccDialog.o: /usr/local/lib/qt/include/qcursor.h
dccDialog.o: /usr/local/lib/qt/include/qfont.h
dccDialog.o: /usr/local/lib/qt/include/qfontmet.h
dccDialog.o: /usr/local/lib/qt/include/qfontinf.h
dccDialog.o: /usr/local/lib/qt/include/qpushbt.h
dccDialog.o: /usr/local/lib/qt/include/qbutton.h
dccDialog.o: /usr/local/lib/qt/include/qlistbox.h
dccDialog.o: /usr/local/lib/qt/include/qtablevw.h
dccDialog.o: /usr/local/lib/qt/include/qpixmap.h
mdccDialog.o: dccDialog.h dccDialogData.h /usr/local/lib/qt/include/qframe.h
mdccDialog.o: /usr/local/lib/qt/include/qwidget.h
mdccDialog.o: /usr/local/lib/qt/include/qwindefs.h
mdccDialog.o: /usr/local/lib/qt/include/qobjdefs.h
mdccDialog.o: /usr/local/lib/qt/include/qglobal.h
mdccDialog.o: /usr/local/lib/qt/include/qobject.h
mdccDialog.o: /usr/local/lib/qt/include/qstring.h
mdccDialog.o: /usr/local/lib/qt/include/qarray.h
mdccDialog.o: /usr/local/lib/qt/include/qgarray.h
mdccDialog.o: /usr/local/lib/qt/include/qshared.h
mdccDialog.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mdccDialog.o: /usr/include/features.h /usr/include/sys/cdefs.h
mdccDialog.o: /usr/local/lib/qt/include/qevent.h
mdccDialog.o: /usr/local/lib/qt/include/qrect.h
mdccDialog.o: /usr/local/lib/qt/include/qsize.h
mdccDialog.o: /usr/local/lib/qt/include/qpoint.h
mdccDialog.o: /usr/local/lib/qt/include/qpaintd.h
mdccDialog.o: /usr/local/lib/qt/include/qpalette.h
mdccDialog.o: /usr/local/lib/qt/include/qcolor.h
mdccDialog.o: /usr/local/lib/qt/include/qcursor.h
mdccDialog.o: /usr/local/lib/qt/include/qfont.h
mdccDialog.o: /usr/local/lib/qt/include/qfontmet.h
mdccDialog.o: /usr/local/lib/qt/include/qfontinf.h
mdccDialog.o: /usr/local/lib/qt/include/qpushbt.h
mdccDialog.o: /usr/local/lib/qt/include/qbutton.h
mdccDialog.o: /usr/local/lib/qt/include/qlistbox.h
mdccDialog.o: /usr/local/lib/qt/include/qtablevw.h
mdccDialog.o: /usr/local/lib/qt/include/qpixmap.h
mdccDialog.o: /usr/local/lib/qt/include/qmetaobj.h
mdccDialog.o: /usr/local/lib/qt/include/qconnect.h
dccDialogData.o: dccDialogData.h /usr/local/lib/qt/include/qframe.h
dccDialogData.o: /usr/local/lib/qt/include/qwidget.h
dccDialogData.o: /usr/local/lib/qt/include/qwindefs.h
dccDialogData.o: /usr/local/lib/qt/include/qobjdefs.h
dccDialogData.o: /usr/local/lib/qt/include/qglobal.h
dccDialogData.o: /usr/local/lib/qt/include/qobject.h
dccDialogData.o: /usr/local/lib/qt/include/qstring.h
dccDialogData.o: /usr/local/lib/qt/include/qarray.h
dccDialogData.o: /usr/local/lib/qt/include/qgarray.h
dccDialogData.o: /usr/local/lib/qt/include/qshared.h
dccDialogData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
dccDialogData.o: /usr/include/features.h /usr/include/sys/cdefs.h
dccDialogData.o: /usr/local/lib/qt/include/qevent.h
dccDialogData.o: /usr/local/lib/qt/include/qrect.h
dccDialogData.o: /usr/local/lib/qt/include/qsize.h
dccDialogData.o: /usr/local/lib/qt/include/qpoint.h
dccDialogData.o: /usr/local/lib/qt/include/qpaintd.h
dccDialogData.o: /usr/local/lib/qt/include/qpalette.h
dccDialogData.o: /usr/local/lib/qt/include/qcolor.h
dccDialogData.o: /usr/local/lib/qt/include/qcursor.h
dccDialogData.o: /usr/local/lib/qt/include/qfont.h
dccDialogData.o: /usr/local/lib/qt/include/qfontmet.h
dccDialogData.o: /usr/local/lib/qt/include/qfontinf.h
dccDialogData.o: /usr/local/lib/qt/include/qpushbt.h
dccDialogData.o: /usr/local/lib/qt/include/qbutton.h
dccDialogData.o: /usr/local/lib/qt/include/qlistbox.h
dccDialogData.o: /usr/local/lib/qt/include/qtablevw.h
dccDialogData.o: /usr/local/lib/qt/include/qpixmap.h
mdccDialogData.o: dccDialogData.h /usr/local/lib/qt/include/qframe.h
mdccDialogData.o: /usr/local/lib/qt/include/qwidget.h
mdccDialogData.o: /usr/local/lib/qt/include/qwindefs.h
mdccDialogData.o: /usr/local/lib/qt/include/qobjdefs.h
mdccDialogData.o: /usr/local/lib/qt/include/qglobal.h
mdccDialogData.o: /usr/local/lib/qt/include/qobject.h
mdccDialogData.o: /usr/local/lib/qt/include/qstring.h
mdccDialogData.o: /usr/local/lib/qt/include/qarray.h
mdccDialogData.o: /usr/local/lib/qt/include/qgarray.h
mdccDialogData.o: /usr/local/lib/qt/include/qshared.h
mdccDialogData.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
mdccDialogData.o: /usr/include/features.h /usr/include/sys/cdefs.h
mdccDialogData.o: /usr/local/lib/qt/include/qevent.h
mdccDialogData.o: /usr/local/lib/qt/include/qrect.h
mdccDialogData.o: /usr/local/lib/qt/include/qsize.h
mdccDialogData.o: /usr/local/lib/qt/include/qpoint.h
mdccDialogData.o: /usr/local/lib/qt/include/qpaintd.h
mdccDialogData.o: /usr/local/lib/qt/include/qpalette.h
mdccDialogData.o: /usr/local/lib/qt/include/qcolor.h
mdccDialogData.o: /usr/local/lib/qt/include/qcursor.h
mdccDialogData.o: /usr/local/lib/qt/include/qfont.h
mdccDialogData.o: /usr/local/lib/qt/include/qfontmet.h
mdccDialogData.o: /usr/local/lib/qt/include/qfontinf.h
mdccDialogData.o: /usr/local/lib/qt/include/qpushbt.h
mdccDialogData.o: /usr/local/lib/qt/include/qbutton.h
mdccDialogData.o: /usr/local/lib/qt/include/qlistbox.h
mdccDialogData.o: /usr/local/lib/qt/include/qtablevw.h
mdccDialogData.o: /usr/local/lib/qt/include/qpixmap.h
mdccDialogData.o: /usr/local/lib/qt/include/qmetaobj.h
mdccDialogData.o: /usr/local/lib/qt/include/qconnect.h
linelistitem.o: /usr/local/lib/qt/include/qpainter.h
linelistitem.o: /usr/local/lib/qt/include/qpaintd.h
linelistitem.o: /usr/local/lib/qt/include/qwindefs.h
linelistitem.o: /usr/local/lib/qt/include/qobjdefs.h
linelistitem.o: /usr/local/lib/qt/include/qglobal.h
linelistitem.o: /usr/local/lib/qt/include/qrect.h
linelistitem.o: /usr/local/lib/qt/include/qsize.h
linelistitem.o: /usr/local/lib/qt/include/qpoint.h
linelistitem.o: /usr/local/lib/qt/include/qcolor.h
linelistitem.o: /usr/local/lib/qt/include/qfontmet.h
linelistitem.o: /usr/local/lib/qt/include/qfont.h
linelistitem.o: /usr/local/lib/qt/include/qstring.h
linelistitem.o: /usr/local/lib/qt/include/qarray.h
linelistitem.o: /usr/local/lib/qt/include/qgarray.h
linelistitem.o: /usr/local/lib/qt/include/qshared.h
linelistitem.o: /usr/local/lib/qt/include/qgeneric.h /usr/include/string.h
linelistitem.o: /usr/include/features.h /usr/include/sys/cdefs.h
linelistitem.o: /usr/local/lib/qt/include/qfontinf.h
linelistitem.o: /usr/local/lib/qt/include/qregion.h
linelistitem.o: /usr/local/lib/qt/include/qpen.h
linelistitem.o: /usr/local/lib/qt/include/qbrush.h
linelistitem.o: /usr/local/lib/qt/include/qpntarry.h
linelistitem.o: /usr/local/lib/qt/include/qwmatrix.h
linelistitem.o: /usr/local/lib/qt/include/qlistbox.h
linelistitem.o: /usr/local/lib/qt/include/qtablevw.h
linelistitem.o: /usr/local/lib/qt/include/qframe.h
linelistitem.o: /usr/local/lib/qt/include/qwidget.h
linelistitem.o: /usr/local/lib/qt/include/qobject.h
linelistitem.o: /usr/local/lib/qt/include/qevent.h
linelistitem.o: /usr/local/lib/qt/include/qpalette.h
linelistitem.o: /usr/local/lib/qt/include/qcursor.h
linelistitem.o: /usr/local/lib/qt/include/qpixmap.h linelistitem.h
