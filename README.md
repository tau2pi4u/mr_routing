# mr_routing
## Setup
To perform this setup, you will need:
* A raspberry pi with an internet connection
* A router with a 2.4GHz wireless network.
* Some LOL1n Arduino boards, ready for programming.
There are quite a few steps. Please follow this guide carefully, it'll save time in the long run.
### Preparing the LOL1n boards and libraries
* Follow this guide **carefully** for each LOL1n board. Make sure the test program works. https://www.instructables.com/id/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/
* Install the library LiquidCrystal I2C **by Frank de Brabander**. There are multiple libraries with the same name, it's important that the correct one is installed.
### Raspberry Pi
* Install Python 3.6 or newer (see http://www.knight-of-pi.org/installing-python3-6-on-a-raspberry-pi/ if you're unsure how). Earlier versions of Python 3 may work, but they are not actively supported.
* `sudo python3.6 -m pip install flask flask_restful`
* OR `python3.6 -m pip install --user flask flask_restful`
* `git clone https://github.com/tau2pi4u/mr_routing.git`
* `cd mr_routing/api`
* `chmod +x api.sh`
* Make a file called `config.json` which contains:
```json
{
    "address_map" : { 
        
        },
    "connections":[
        
    ]
}
```
* `./api.sh`
* If everything has worked, you should have a server running. Press CTRL+C to quit.
* Make a note of the local ip address of your server. This can be found by typing `ifconfig`  

### Arduino
* `cd ../`
* Create a file called `network.json` which contains:
```json
{
    "network" : "Network Name",
    "password" : "Network Password",
    "server_ip" : "Server IP"
}
```
* Replace `Network Name`, `Network Password` and `Server IP` with the correct values. It should look something like:
```json
{
    "network" : "mr_routing",
    "password" : "model_railways",
    "server_ip" : "192.168.0.10"
}
```
* `cd arduino`
* `python3.6 gen.py`
* `cp ../gen/gen.hpp gen.hpp`
* Open `arduino.ino` in the arduino IDE.
* Make sure the settings for upload are correct according to the guide from earlier.
* Upload the sketch to the arduino. 
