# Makefile for mr_routing

CLI=bin/arduino-cli
CLI_CFG=~/.arduino15/arduino-cli.yaml
CLI_PKG=~/.arduino15/package_index.json
LIB=~/Arduino/libraries/LiquidCrystal_I2C
LIBH=$(LIB)/LiquidCrystal_I2C.h

all : cli build

build : gen copy compile

# Install the arduino-cli

.PHONY : cli

cli : bin/arduino-cli esp8266 $(LIBH)

bin/arduino-cli : 
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

$(CLI_CFG) : 
	$(CLI) config init

$(CLI_PKG) : $(CLI_CFG)
	$(CLI) core update-index --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json

.PHONY : esp8266

esp8266 : bin/arduino-cli $(CLI_CFG) $(CLI_PKG)
	$(CLI) core install --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json esp8266:esp8266


$(LIBH) : gen bin/arduino-cli
	git clone https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library.git $(LIB)

# Build commands

.PHONY : gen 

gen : network.json
	cd arduino; python3 gen.py; cd ..

.PHONY : copy

copy : gen
	cp arduino/*.?pp build/sketch/; \
	cp arduino/arduino.ino build/sketch/arduino.ino

compile : gen copy
	$(CLI) compile --fqbn esp8266:esp8266:nodemcuv2 build/sketch/arduino.ino

# Clean commands

clean : 
	rm -rf build/ gen/

check_reset:
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]

.PHONY: clean check_clean

reset : clean check_reset
	rm -rf bin/ ~/.arduino15/ ~/Arduino/
