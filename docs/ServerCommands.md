# IRC Server Commands

## Command: JOIN

### Description
The `JOIN` command is used to join a channel or create one if it does not currently exist.

### Syntax

/JOIN <CHANNEL>,...

### Parameters
- **CHANNEL**: The name of the channel to join
- **<CHANNEL>,...**: Indicates that an list of channels to join or create may be supplied, seperated by commas

### Examples
- **Example 1**:
    /JOIN #exampleChannel

- **Example 2**: 
    /JOIN #testChannel1,#testChannel2

### Notes
* If the channel does not exist, it will be created
* Each channel name must start with a pound symbol "#"

---

## Command: KICK

### Description
The `KICK` command removes a user from a channel.

### Syntax

/KICK <CHANNEL> <NICKNAME> :<COMMENT> 

### Parameters
- **CHANNEL**: The name of the channel the user will be kicked from
- **NICKNAME**: The nickname of the user to be kicked from the aforementioned channel
- **COMMENT**: Optional comment that will be sent to the kicked user

### Examples
- **Example 1**:
    /KICK #exampleChannel exampleNick

- **Example 2**: 
    /KICK #exampleChannel exampleNick :You were kicked from #exampleChannel

### Notes
* The optional comment must be denoted by a leading colon ":"

---

## Command: LIST

### Description
The `LIST` command lists all requested channels and the current user counts of each.

### Syntax

/LIST <*|<CHANNEL>...,>

### Parameters
- **"*"**: If an asterick is supplied, all channels will be listed
- **<CHANNEL>...,**: A comma seperated list of channels to list

### Examples
- **Example 1**:
    /LIST *

- **Example 2**: 
    /LIST #channel

- **Example 3**: 
    /LIST #channel1,#channel2,#channel3

### Notes
* N/A

---

## Command: NAMES

### Description
The `NAMES` COMMAND lists the nicknames of users currently in all specified channels.

### Syntax

/NAMES <CHANNEL>,...

### Parameters
- **<CHANNEL>,...**: A comma seperated list of channels to list the nicknames of

### Examples
- **Example 1**:
    /NAMES #channel

- **Example 2**: 
    /NAMES #channel1,#channel2,#channel3

### Notes
* There is no way to list all channels unless all are specified

---

## Command: NICK

### Description
The `NICK` command sets a nickname for a connection.

### Syntax

/NICK <NICKNAME>

### Parameters
- **<NICKNAME>**: Nickname to be applied to the requsting connection

### Examples
- **Example 1**:
    /NICK TEST_NICK

- **Example 2**: 
    /NICK s1-dev

### Notes
* There are no duplicate nicknames allowed

---

## Command: PART

### Description
The `PART` command allows a user to remove itself from specified channels. 

### Syntax

/PART <CHANNEL>,... :<MESSAGE>

### Parameters
- **<CHANNEL>,...**: Comma seperated list of channels the user wants to leave
- **<MESSAGE>**: Optional message that will be broadcasted to each channel the user is leaving

### Examples
- **Example 1**:
    /PART #testChannel

- **Example 2**: 
    /PART #channel1,#channel2

- **Example 3**: 
    /PART #channel1,#channel2 :I'm leaving, goodbye

### Notes
* An optional message is denoted by a leading colon ":"

---

## Command: PRIVMSG

### Description
The `PRIVMSG` command allows a user to send a direct message to specified users and or channels.

### Syntax

/PRIVMSG <(#<CHANNEL_NAME>|<NICKNAME>)>,... :<MESSAGE>

### Parameters
- **<(#<CHANNEL_NAME>|<NICKNAME>)>,...**: Comma seperated list of channels or nicknames to private message
- **<MESSAGE>**: Message to send to all receivers

### Examples
- **Example 1**:
    /PRIVMSG #testChannel :Hello world!

- **Example 2**: 
    /PRIVMSG #channel1,user1 :Hello everyone!

- **Example 3**: 
    /PRIVMSG #channel1,#channel2,user123 :Here's the secret password: P@ssw0rd123!

### Notes
* The message is required, and is denoted by a leading colon ":"

---

## Command: QUIT

### Description
The `QUIT` command allows the user to disconnect from the IRC server.

### Syntax

/QUIT :<MESSAGE>

### Parameters
- **<MESSAGE>**: Message to send to all channels the user is apart of

### Examples
- **Example 1**:
    /QUIT

- **Example 2**: 
    /QUIT :I'm gone forever!

### Notes
* The message is optional and is denoted by a leading colon ":"

---

## Command: SHUTDOWN

### Description
The `SHUTDOWN` command allows the master to shutdown the IRC server remotely. 

### Syntax

/SHUTDOWN <SHUTDOWN_TOKEN>

### Parameters
- **<SHUTDOWN_TOKEN>**: 64-char string acts as a form of authentication

### Examples
- **Example 1**:
    /SHUTDOWN 94ee059335e587e501cc4bf90613e0814f00a7b08bc7c648fd865a2af6a22cc2

### Notes
* N/A

---

## Command: TOPIC

### Description
The `TOPIC` command allows for a user to change the topic of a specified channel.

### Syntax

/TOPIC <CHANNEL_NAME> :<TOPIC_CONTENTS>

### Parameters
- **<CHANNEL_NAME>**: Name of channel to mutate the topic of
- **<TOPIC_CONTENTS>**: Message to be set as channel topic 

### Examples
- **Example 1**:
    /TOPIC #testChannel :Welcome!

- **Example 2**: 
    /TOPIC #testChannel :greatest topic of all time

### Notes
* The message is required and is denoted by a leading colon ":"
* Only one channel can have their topic changed with the TOPIC command
* The new topic is broadcasted to the specified channel

---

## Command: USER

### Description
The `USER` command allows for a user to set their username and realname.

### Syntax

/USER <USERNAME> 0 * <REALNAME>

### Parameters
- **<USERNAME>**: Username to be set for a connected user
- **"0 *"**: These represent the hostname and servername, which are not mutable. It is common practice to include these in IRC USER commands
- **<REALNAME>**: Realname to be set for a connected user

### Examples
- **Example 1**:
    /USER testUsername 0 * testRealName

- **Example 2**: 
    /TOPIC #testChannel :greatest topic of all time

### Notes
* The username and realname must be set, alongside a nickname, in order to join channels, send messages, and issue server commands

---

## Command: VALIDATE

### Description
The `VALIDATE` command ensures that only the master program and IRC bots created from the master program are able to connect via a form of passphrase authentication. 

### Syntax

/VALIDATE <PASSPHRASE> <MASTER|BOT>

### Parameters
- **<PASSPHRASE>**: Passphrase which is hashed via SHA-256 and compared with the server's stored hashed master or bot digest 
- **<MASTER|BOT>**: Identifier to specify whether a bot or the master program is connecting

### Examples
- **Example 1**:
    /VALIDATE password123 MASTER

- **Example 2**: 
    /VALIDATE bestPassword BOT

### Notes
* The given passphrase is hashed via SHA-256
* A connection must issue a VALIDATE command in order to proceed with registration and then other actions
* An incorrect passphrase will automatically terminate the connection
* The identifier param must be capitalized 

---