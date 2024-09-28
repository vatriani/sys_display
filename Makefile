DEBUG := 1

FLAGS = -Wall -mtune=native -march=native
SRCS = monitor.c
TARGET = monitor

ifeq ($(DEBUG),1)
FLAGS += -O -g -DDEBUG
else
FLAGS += -O2
endif

all: $(TARGET) pio

$(TARGET): $(SRCS)
	gcc $(FLAGS) $(SRCS) -o $(TARGET)

pio:
	pio run

upload:
	pio run --target upload

run:
	./$(TARGET)

doc:
	doxygen

clean:
	rm -f *.d *.o $(TARGET)
	pio run --target clean

install: $(TARGET)
	cp $(TARGET) /usr/bin/$(TARGET)
	chmod go+rx /usr/bin/$(TARGET)

translation:
	xgettext --keyword=_ --language=C --add-comments -o po/monitor.pot monitor.c
	#msginit --input=po/monitor.pot --locale=de --output=po/de/monitor.po
