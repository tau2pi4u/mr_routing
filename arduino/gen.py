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
"""#pragma once
#define NETWORK \"{0}\"
#define PASSWORD \"{1}\"
#define SERVER_IP \"{2}\"""".format(networkCfg['network'], networkCfg['password'], networkCfg['server_ip']))
file.close()

if not os.path.isdir('../build/'):
    os.mkdir('../build')
if not os.path.isdir('../build/sketch'):
    os.mkdir('../build/sketch')
shutil.copyfile("../gen/gen.hpp", "../build/sketch/gen.hpp")