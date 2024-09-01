Some data informations...
# Docs
u8g2 Display libary
https://github.com/olikraus/u8g2

# Compile for arduino
`$ make`
`$ pio run --target upload`
# Compile for host system
`$ make monitor`
`$ ./monitor`

#Infos to display
## nvidia
`nvidia-smi --query-gpu=temperature.gpu,utilization.gpu,power.draw --format=csv -l 5`
Outputs:
- GPU temperature
- GPU utilization
- Power draw
## lm_sensors
`sensors -A -u`
Outputs:
- CPU temperature
- CPU MHZ
- CPU utilization

- Fan1
- Fan2
- Fan3
## liquidctl
`liquidctl status`
Outputs:
- Fanspeed
- Pumpspeed
- Watertemp
