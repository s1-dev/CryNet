import json
import os
import hashlib
import random
import string

MASTER_PROGAM_CONFIG_PATH = "src/master/resources/config.json"
SERVER_CONFIG_PATH = "src/ircserver/src/main/resources/config.json"


def generateRandomString(length):
    characters = string.ascii_letters + string.digits
    random_string = ''.join(random.choices(characters, k=length))
    return random_string

def computeSha256Hex(inputStr):
    encodedString = inputStr.encode("utf-8")
    sha256sum = hashlib.sha256()
    sha256sum.update(encodedString)
    return sha256sum.hexdigest().lower()

def printConfig(configPath):
    if not os.path.exists(configPath):
        print(f"Config file {configPath} does not exist.")
        return

    with open(configPath, 'r') as f:
        config = json.load(f)

    print("Current configuration:")
    print(json.dumps(config, indent=4))

def updateConfig(configPath, updates):
    if not os.path.exists(configPath):
        print(f"Config file {configPath} does not exist. Creating a new one.")
        with open(configPath, 'w') as f:
            json.dump({}, f, indent=4)


    with open(configPath, 'r') as f:
        config = json.load(f)

    config.update(updates)

    with open(configPath, 'w') as f:
        json.dump(config, f, indent=4)

def updateMasterConfig():
    print("~~~~~~~~~~~~~~~~~Master config~~~~~~~~~~~~~~~~~")
    printConfig(MASTER_PROGAM_CONFIG_PATH)

    server = input("Server address (IP): ")
    port = input("Port number (6667|6697): ")
    if port != "6667": # TEMP
        print("ERROR: Currently only port 6667 is supported")
        port = "6667"
    masterValidatePass = input("Master validation passphrase: ")
    masterValidatePass = computeSha256Hex(masterValidatePass)
    botValidatePass = input("Bot validation passphrase: ")
    botValidatePass = computeSha256Hex(botValidatePass)
    
    configUpdates = {
        "server": server,
        "port": int(port),
        "masterValidatePass": masterValidatePass,
        "botValidatePass": botValidatePass
    }

    updateConfig(MASTER_PROGAM_CONFIG_PATH, configUpdates)

    print("~~~~~~~~~~~~~~~~~Updated master config~~~~~~~~~~~~~~~~~")
    printConfig(MASTER_PROGAM_CONFIG_PATH)

def updateServerConfig():
    print("~~~~~~~~~~~~~~~~~Server config~~~~~~~~~~~~~~~~~")
    printConfig(SERVER_CONFIG_PATH)
    
    serverHostname = input("Server hostname (IP): ")
    portNumber = input("Port number (6667|6697): ")
    if portNumber != "6667": # TEMP
        print("ERROR: Currently only port 6667 is supported")
        portNumber = "6667"

    maxChannels = input("Max channels (default is 7): ")
    if maxChannels == "":
        maxChannels = "7"

    maxChannelUsers = input("Max channel users (default is 500): ")
    if maxChannelUsers == "":
        maxChannelUsers = "500"

    maxMessageLength = input("Max message length (default is 300): ")
    if maxMessageLength == "":
        maxMessageLength = "300"

    maxNickLength = input("Max nick length (default is 20): ")
    if maxNickLength == "":
        maxNickLength = "20"   

    maxConnections = input("Max connections (default is 500): ")
    if maxConnections == "":
        maxConnections = "500" 

    masterAuthDigest = input("Master validation password (this must be the same as what is configured on the master program): ")
    masterAuthDigest = computeSha256Hex(masterAuthDigest)

    botAuthDigest = input("Bot validation password (this must be the same as what is configured on the master program): ")
    botAuthDigest = computeSha256Hex(botAuthDigest)

    configUpdates = {
        "serverHostname": serverHostname,
        "portNumber": int(portNumber),
        "maxChannels": int(maxChannels),
        "maxChannelUsers": int(maxChannelUsers),
        "maxMessageLength": int(maxMessageLength),
        "maxNickLength": int(maxNickLength),
        "maxConnections": int(maxConnections),
        "masterAuthDigest": masterAuthDigest,
        "botAuthDigest": botAuthDigest,
        "shutdownSalt": "NOT_YET_IMPLEMENTED_" + generateRandomString(10), # TEMP
        "shutdownDigest": "NOT_YET_IMPLEMENTED_" + generateRandomString(10) # TEMP
    }
    updateConfig(SERVER_CONFIG_PATH, configUpdates)

    print("~~~~~~~~~~~~~~~~~Updated server config~~~~~~~~~~~~~~~~~")
    printConfig(SERVER_CONFIG_PATH)
    

def main():
    print("Welcome to CryNet project setup, please enter revelant information to setup configuration: ")

    configToUpdate = input("What config would you like to update? (MASTER|SERVER|BOTH) ")

    if configToUpdate == "MASTER" or configToUpdate == "master":
        updateMasterConfig()
    elif configToUpdate == "SERVER" or configToUpdate == "server":
        updateServerConfig()
    elif configToUpdate == "BOTH" or configToUpdate == "both":
        updateMasterConfig()
        updateServerConfig()
    
    print("Setup complete. Ensure that password hashes match across configs.")



if __name__ == "__main__":
    main()
