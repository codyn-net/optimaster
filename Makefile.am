
include Makefile.am.inc

optimaster_CFLAGS += $(GCRYPT_CFLAGS)
optimaster_LDFLAGS += $(GCRYPT_LIBS)

opticonfdir = $(sysconfdir)
opticonf_DATA = \
	optimaster.conf

EXTRA_DIST = \
	$(opticonf_DATA)
