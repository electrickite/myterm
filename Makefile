.POSIX:
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/man

RM = rm -f
INSTALL = install

CFLAGS_EXTRA = -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -Os -s
CFLAGS := $(CFLAGS_EXTRA) $(CFLAGS) -std=c99

TARGET = myterm
MAN = $(TARGET).1

all: $(TARGET)

$(TARGET): $(TARGET).c

.PHONY: install uninstall clean

install: all
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/bin/
	$(INSTALL) -d $(DESTDIR)$(MANPREFIX)/man1/
	$(INSTALL) -m 0755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/
	$(INSTALL) -m 0644 $(MAN) $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	$(RM) $(DESTDIR)$(MANPREFIX)/man1/$(MAN)

clean:
	$(RM) $(TARGET)
