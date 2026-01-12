DEBUG := 1

CC = gcc
CFLAGS =
LDFLAGS = add PHONY rules.
FLAGS = -Wall -mtune=native -march=native

SRCS = monitor.c
TARGET = monitor

ifeq ($(DEBUG),1)
FLAGS += -O -g -DDEBUG
else
FLAGS += -O2
endif

.PHONY: clean

all: $(TARGET) po/monitor.pot

$(TARGET): $(SRCS)
	$(CC) $(FLAGS) $(SRCS) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f *.d *.o $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/bin/$(TARGET)
	chmod go+rx /usr/bin/$(TARGET)

translation:
	xgettext --keyword=_ --language=C --add-comments -o po/$(TARGET).pot $(TARGET).c

po/de/$(TARGET).mo: po/de/$(TARGET).po
	msgfmt --output-file=$@ $<

po/de/$(TARGET).po: po/$(TARGET).pot
	msgmerge --update $@ $<

po/$(TARGET).pot: $(TARGET).c
	xgettext -k_ -j -lC -c -o po/$(TARGET).pot $(TARGET).c
