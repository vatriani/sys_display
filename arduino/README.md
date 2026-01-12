# nodemcu
Simple solution for showing system vitals. An nodemcu would be an overkill, but
an Arduino should do the same thing. (with some code changes)

## Connecting the display
- D1 => SDA
- D2 => SCK
- 3V => VCC
- G => GND

## Compile
`$ make pio`

## Upload to nodemcu
`$ make upload`

## Libaries used:
- u8g2 Display libary - https://github.com/olikraus/u8g2
