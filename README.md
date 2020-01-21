# mr_routing

## Setup
To perform this setup, you will need:
* A raspberry pi with an internet connection
* A router with a 2.4GHz wireless network.
* Some LOL1n Arduino boards, ready for programming.
There are quite a few steps. Please follow this guide carefully, it'll save time in the long run.

### Preparing the LOL1n boards and libraries
* Follow step 1 of this guide **carefully** for each LOL1n board.
* If you're having issues, make sure the test program works. 
* https://www.instructables.com/id/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/

### Raspberry Pi
#### Python setup
* Install Python 3
  * To check your python version, type: `python -V`.
  * If this says 2.7, try typing `python3 -V`.
  * It should work with all versions of python3, but 3.5 is the minimum it has been tested with.
  * Earlier versions may work but are not directly supported.
* `sudo python3 -m pip install flask flask_restful`
  * If it says pip is not installed, run:
  * `wget https://bootstrap.pypa.io/get-pip.py`
  * `sudo python3 get-pip.py`
* then repeat the above steps

#### Getting the Repo
* `git clone https://github.com/tau2pi4u/mr_routing.git`
* `cd mr_routing/api`
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
* Make a note of the local ip address of your server. This can be found by typing `ifconfig` and should look like `192.168.0.X`.

### Arduino
#### Do this once:
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
* `make compile`

#### Do this for each arduino
* `make upload`
  * If it can't find the board, type
  * `make listports`
  * and try
  * `make upload PORT=/dev/ttyUSB0` 
  * but replace `/dev/ttyUSB0` with the other listed ports
* Note the mac address shown on the screen

### Updating the config
* Go back to `config.json`
* In address map, add pairs of mac addresses and names. Names should be in the form `A#` where # is a number up to 4 digits. 
* This might look something like: 
```json
"address_map" : {
        "CC:50:E3:CD:A7:47" : "A0",
        "CC:50:E3:CD:A7:52" : "A1"
        },
```
* Build the connections using the `connections` dictionary. These connections can either be `both`, meaning it is assumed that trains can go both ways, or `one-way`, meaning that trains can only go from the first named node to the second named node.
* This might look something like:
```json
"connections":[
        ["A0", "A1", "both"],
        ["A1", "A2", "one-way"],
        ["A2", "A0", "one-way"]
]
```
* Start the server by going to the `api` folder and running `./api.sh`
* The arduinos should find the server and display their configurations (wait 10 seconds for them to repeat the check).

