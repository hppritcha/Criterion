ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -DLOCALEDIR='"$(localedir)"'
SUBDIRS = po samples

lib_LTLIBRARIES = libcriterion.la

WARNINGS = -Wall -Wextra \
	-Wno-unused-result

libcriterion_la_CFLAGS = \
	$(WARNINGS) \
	-std=gnu11 \
	-I$(top_srcdir)/src/ \
	-I$(top_srcdir)/include/ \
	$(COVERAGE_CFLAGS)

libcriterion_la_LDFLAGS = $(COVERAGE_LDFLAGS) -no-undefined -version-info 1:0:0
libcriterion_la_LIBADD = -lpcre

EXTRA_DIST = config.rpath LICENSE

subdirincludedir = $(includedir)/criterion/
subdirinclude_HEADERS = \
	include/criterion/assert.h \
	include/criterion/abort.h \
	include/criterion/common.h \
	include/criterion/criterion.h \
	include/criterion/event.h \
	include/criterion/hooks.h \
	include/criterion/logging.h \
	include/criterion/types.h \
	include/criterion/options.h \
	include/criterion/ordered-set.h \
	include/criterion/stats.h

libcriterion_la_SOURCES = \
	src/abort.c \
	src/abort.h \
	src/event.c \
	src/event.h \
	src/report.c \
	src/report.h \
	src/runner.c \
	src/runner.h \
	src/process.c \
	src/process.h \
	src/stats.c \
	src/stats.h \
	src/log/logging.c \
	src/log/tap.c \
	src/log/normal.c \
	src/options.c \
	src/timer.c \
	src/timer.h \
	src/i18n.c \
	src/i18n.h \
	src/ordered-set.c \
	src/posix-compat.c \
	src/extmatch.c \
	src/extmatch.h \
	src/main.c

TARGET = $(PACKAGE)-$(VERSION)

package: all
	rm -Rf $(TARGET)
	mkdir -p $(TARGET)
	cp -Rf .libs $(TARGET)/lib/
	rm -f $(TARGET)/lib/libcriterion.la
	cp -f libcriterion.la $(TARGET)/lib
	cp -Rf include $(TARGET)
	tar -cvjf $(TARGET).tar.bz2 $(TARGET)

clean-local:
	rm -Rf $(TARGET) $(TARGET).tar.bz2
