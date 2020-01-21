# Makefile for mr_routing

# Port vars
PORT=/dev/ttyUSB0

# CLI vars
CLI=bin/arduino-cli
CLI_CFG=~/.arduino15/arduino-cli.yaml
CLI_PKG=~/.arduino15/package_index.json

# Lib vars
LIB=~/Arduino/libraries/LiquidCrystal_I2C
LIBH=$(LIB)/LiquidCrystal_I2C.h

# Ino vars
INO_DIR=build/sketch
INO=$(INO_DIR)/sketch.ino

# Bin
BIN=$(INO_DIR)/sketch.esp8266.esp8266.nodemcuv2.bin

# Gen vars
GEN=$(INO_DIR)/gen.hpp

all : cli upload

build : $(GEN) $(COPY) $(BIN)

# Install the arduino-cli

.PHONY : cli

cli : $(CLI) esp8266 $(LIBH)

bin/arduino-cli : 
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

$(CLI_CFG) : 
	$(CLI) config init

$(CLI_PKG) : $(CLI_CFG)
	$(CLI) core update-index --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json

.PHONY : esp8266

esp8266 : bin/arduino-cli $(CLI_CFG) $(CLI_PKG)
	$(CLI) core install --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json esp8266:esp8266


$(LIBH) : $(CLI)
	git clone https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library.git $(LIB)

# Build commands

$(GEN) : network.json
	cd arduino; python3 gen.py; cd ..

$(INO) : $(GEN)
	cp arduino/*.?pp build/sketch/; \
	cp arduino/arduino.ino $(INO)

$(BIN) : $(GEN) $(INO)
	$(CLI) compile --fqbn esp8266:esp8266:nodemcuv2 $(INO_DIR)

.PHONY : compile

compile : $(GEN) $(INO) $(BIN)


# Upload commands

.PHONY : listports

listports : $(CLI)
	$(CLI) board list

.PHONY : upload

upload : $(BIN) $(CLI)
	$(CLI) upload -p $(PORT) --fqbn esp8266:esp8266:nodemcuv2:xtal=80,vt=flash,exception=disabled,ssl=all,eesz=4M,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=115200 $(INO_DIR)


# Clean commands

clean : 
	rm -rf build/ gen/

check_reset:
	@echo -n "Are you sure? This will delete the ~/Arduino/ folder and the ~/.arduino15/ folder! [y/N] " && read ans && [ $${ans:-N} = y ]

.PHONY: clean check_clean

reset : clean check_reset
	rm -rf bin/ ~/.arduino15/ ~/Arduino/

