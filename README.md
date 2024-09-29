[![C/C++ CI](https://github.com/vatriani/sys_display/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/vatriani/sys_display/actions/workflows/c-cpp.yml)[![PlatformIO CI](https://github.com/vatriani/sys_display/actions/workflows/pio.yml/badge.svg)](https://github.com/vatriani/sys_display/actions/workflows/pio.yml)

Monitor program runs on linux host system and fetches some vitals, sends it over
serial port to an nodemcu witch displays the vitals on a small screen.
The small screen switches automatically between displayed pages.

## Compile
`$ make`

## Upload to nodemcu
`$ pio run --target upload`

## Run watchdog
`$ ./monitor`

## Infos to display
### nvidia
`nvidia-smi --query-gpu=temperature.gpu,utilization.gpu,power.draw --format=csv -l 5`

Outputs:
- GPU temperature
- GPU utilization
- Power draw
### lm_sensors
`sensors -A -u`

Outputs:
- CPU temperature
- CPU MHZ
- CPU utilization

- Fan1
- Fan2
- Fan3
### liquidctl
`liquidctl status`

Outputs:
- Fanspeed
- Pumpspeed
- Watertemp

## Libaries used
### u8g2 Display libary
https://github.com/olikraus/u8g2
