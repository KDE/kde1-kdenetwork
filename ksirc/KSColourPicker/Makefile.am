
INCLUDES= $(all_includes) -include $(srcdir)/../kplunger/kplunger.h

# you can add here more. This one gets installed 
noinst_LIBRARIES = libkscolourpicker.a

libkscolourpicker_a_SOURCES = \
kscolourpicker.cpp \
mkscolourpicker.cpp \
kscolourpickerData.cpp \
mkscolourpickerData.cpp

noinst_HEADERS = \
kscolourpicker.h \
kscolourpickerData.h

# just to make sure, automake makes them 
BUILTSOURCES = \
mkscolourpicker.cpp \
mkscolourpickerData.cpp 

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@

