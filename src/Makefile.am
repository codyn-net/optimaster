INCLUDES = \
	-I$(srcdir)\
	-I$(builddir)\
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(PACKAGE_OPTIMASTER_CFLAGS) \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-DCONFDIR=\""$(sysconfdir)"\" \
	-DPREFIXDIR=\""$(prefix)"\" \
	-DPKGDATADIR=\""$(pkgdatadir)"\" \
	-DPKGLIBDIR=\""$(pkglibdir)"\"

bin_PROGRAMS = optimaster

optimaster_CFLAGS = -g $(PACKAGE_OPTIMASTER_CFLAGS)
optimaster_LDFLAGS = $(PACKAGE_OPTIMASTER_LIBS)

optimaster_SOURCES = \
	optimaster.cc \
	application.cc \
	batch.cc \
	config.cc \
	communicator.cc \
	optimizer.cc \
	optimizermanager.cc \
	task.cc \
	taskqueue.cc \
	worker.cc \
	workermanager.cc

noinst_HEADERS = \
	application.hh \
	batch.hh \
	config.hh \
	communicator.hh \
	optimizer.hh \
	optimizermanager.hh \
	task.hh \
	taskqueue.hh \
	worker.hh \
	workermanager.hh

EXTRA_DIST = $(noinst_HEADERS)
