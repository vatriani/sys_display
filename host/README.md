# Monitor programm on host system
Deamon programm constantly checking system vitals with  

`$ nvidia-smi --query-gpu=temperature.gpu,utilization.gpu,power.draw --format=csv -l 5`  
`$ sensors -A -u`  
`$ liquidctl status`  

## Compile
`$ make monitor`

## Run watchdog
`$ ./monitor`

Monitor programm can also runned in background with `$ ./monitor &`.  
For developing or debugging `$ ./monitor -D`.  
If the monitor programm can't find the device, check for /dev/ttyUSB0. It's
used as an default value. Changes with `$ ./monitor -p {device-path}`.
