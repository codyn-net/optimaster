include Makefile.doxygen

all: doxygen

install-data-hook: all
	cp -r html $(datadir)/doc/optimaster

EXTRA_DIST = doxygen.cfg
