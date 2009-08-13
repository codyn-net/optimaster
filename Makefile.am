
include Makefile.am.inc

optimaster_CFLAGS += $(GCRYPT_CFLAGS)
optimaster_LDFLAGS += $(GCRYPT_LIBS)

opticonfdir = $(datadir)/optimaster
opticonf_DATA = \
	optimaster.conf.ex

EXTRA_DIST = \
	$(opticonf_DATA)
