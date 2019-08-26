import os
import json
import shutil

if not os.path.exists("../network.json"):
    print("No network config found at ../network.json")

networkCfgFile = open("../network.json", "r")
networkCfg = json.load(networkCfgFile)
networkCfgFile.close()

if not os.path.isdir("../gen"):
    os.mkdir("../gen")
file = open("../gen/gen.hpp", "w")
file.write(
f"""#pragma once
#define NETWORK \"{networkCfg['network']}\"
#define PASSWORD \"{networkCfg['password']}\"
#define SERVER_IP \"{networkCfg['server_ip']}\"""")
file.close()

shutil.copyfile("../gen/gen.hpp", "../build/sketch/gen.hpp")