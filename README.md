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
Deamon programm constantly checking system vitals. Check host folder for more
infos.


## nodemcu
Simple solution for showing system vitals. Check arduino folder for more
infos.
