[![C/C++ CI](https://github.com/vatriani/sys_display/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/vatriani/sys_display/actions/workflows/c-cpp.yml)[![PlatformIO CI](https://github.com/vatriani/sys_display/actions/workflows/pio.yml/badge.svg)](https://github.com/vatriani/sys_display/actions/workflows/pio.yml)

Monitor programm runs on linux host system and fetches some vitals, sends it over
serial port to an nodemcu witch displays the vitals on a small screen.
The small screen switches automatically between displayed pages.

### Displayed infos
- GPU temperature
- GPU utilization
- GPU Power draw
- CPU temperature
- CPU MHZ
- CPU utilization
- Fan 1 Speed
- Fan 2 Speed
- Fan 3 Speed
- Fanspeed
- Pumpspeed
- Water temperature

## Monitor programm on host system
Deamon programm constantly checking system vitals with  

`$ nvidia-smi --query-gpu=temperature.gpu,utilization.gpu,power.draw --format=csv -l 5`  
`$ sensors -A -u`  
`$ liquidctl status`  

### Compile
`$ make monitor`

### Run watchdog
`$ ./monitor`

Monitor programm can also runned in background with `$ ./monitor &`.  
For developing or debugging `$ ./monitor -D`.  
If the monitor programm can't find the device, check for /dev/ttyUSB0. It's
used as an default value. Changes with `$ ./monitor -p {device-path}`.



## nodemcu
Simple solution for showing system vitals. An nodemcu would be an overkill, but
an Arduino should do the same thing. (with some code changes)

### Connecting the display
- D1 => SDA
- D2 => SCK
- 3V => VCC
- G => GND

### Compile
`$ make pio`

## Upload to nodemcu
`$ make upload`

## Libaries used:
- u8g2 Display libary - https://github.com/olikraus/u8g2
