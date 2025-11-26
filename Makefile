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

all: $(TARGET) po/$(TARGET).pot pio

$(TARGET): $(SRCS)
	$(CC) $(FLAGS) $(SRCS) -o $(TARGET)

pio:
	pio run -e nodemcuv2

upload:
	pio run --target upload -e nodemcuv2

ci-check:
test:
local_test:
	pio test -e native

run:
	./$(TARGET)

docs:
	doxygen

clean:
	rm -f *.d *.o $(TARGET)
	pio run --target clean

install: $(TARGET)
	cp $(TARGET) /usr/bin/$(TARGET)
	chmod go+rx /usr/bin/$(TARGET)

translation:
	xgettext --keyword=_ --language=C --add-comments -o po/$(TARGET).pot $(TARGET).c

po/de/$(TERGET).mo: po/de/$(TARGET).po
	msgfmt --output-file=$@ $<

po/de/$(TARGET).po: po/$(TARGET).pot
	msgmerge --update $@ $<

po/$(TARGET).pot: $(TARGET).c
	xgettext -k_ -j -lC -c -o po/$(TARGET).pot $(TARGET).c
