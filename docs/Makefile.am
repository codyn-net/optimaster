all: doxygen.cfg
	$(DOXYGEN) doxygen.cfg

install-data-hook: all
	cp -r html $(datadir)/doc/optimaster

EXTRA_DIST = doxygen.cfg
