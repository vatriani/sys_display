.PHONY: clean

all: pio

pio:
	pio run -e nodemcuv2

upload:
	pio run --target upload -e nodemcuv2

ci-check:
test:
local_test:
	pio test -e native

clean:
	pio run --target clean
