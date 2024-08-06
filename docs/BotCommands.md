# CryNet Bot Commands

## Command: PING

### Description
The `PING` command is used to send network packet(s) to a target IPv4 address and port.

### Syntax

PING {PROTOCOL/PING_TYPE} {TARGET_IP} {TARGET_PORT} {N_PACKETS|LOOOP}

### Parameters
- **PROTOCOL/PING_TYPE**: The type of packet(s) to be sent
- **TARGET_IP**: The target IPv4 address or destination address
- **TARGET_PORT**: The target port number or destination port
- **N_PACKETS|LOOP**: Either the number of packets to be sent to the target or LOOP. LOOP tells the bot to ping indefintely 

### Examples
- **Example 1**:
    PING TCP-SYN 192.168.1.1 80 LOOP

- **Example 2**: 
    PING UDP 10.0.0.92 9999 1000000

### Packet Types:
- **TCP-SYN**
- **TCP-ACK**
- **TCP-SYN-ACK**
- **TCP-XMAS**
- **UDP**
- **ICMP-REQ**
- **ICMP-REP**
- **DNS**
- **ARP-REQ**

### Notes
* Issueing LOOP as the number of packets will make the bot ping the target until external forces close the bot process

---

## Command: ENCRYPT

### Description
The `ENCRYPT` command is used to encrypt the file system of a bot's host device.

### Syntax

ENCRYPT {ENC_ALGORITHM} {ROOT|HOME} {ENC|DEC}

### Parameters
- **ENC_ALGORITHM**: Symmetric encryption algorithm to use
- **ROOT|HOME**: ROOT to encrypt all files starting from "/", "HOME" to encrypt all files starting from "/home"
- **ENC|DEC**: ENC to encrypt all files, DEC to decrypt any encrypted files

### Examples
- **Example 1**:
    ENCRYPT AES-256 ROOT ENC

- **Example 2**: 
    ENCRYPT AES-128 HOME DEC

### Symmetric encryption algorithms:
- **AES-128**
- **AES-192**
- **AES-256**

### Notes
* Currently, only AES is the symmetric encryption algorithm used, with differing key lengths
* AES is using cipher block chaining mode 
* All encrpyted files will have the ".cry" extension attached to it 
* Only files with the ".cry" extension will be decrpyted 

---

## Command: FORK_BOMB

### Description
The `FORK_BOMB` command is used to start a fork bomb on the bot's host.

### Syntax

FORK_BOMB {BASH|C}

### Parameters
- **BASH|C**: BASH to run a Bash fork bomb, C to run a C based fork bomb

### Examples
- **Example 1**:
    FORK_BOMB BASH

- **Example 2**: 
    FORK_BOMB C

### Notes
* C fork bombs seem to be more damaging than Bash fork bombs, at this time

---

## Command: REPORT

### Description
The `REPORT` command is used retrive vital host information of the bot's host.

### Syntax

REPORT

### Parameters
- N/A

### Examples
- **Example 1**:
    REPORT

### Notes
* This command is expected to be expanded upon, but the following pieces of information are returned: Hostname, OS Info, CPU Info, and Memory Info

---

## Command: GET_PASSWD

### Description
The `GET_PASSWD` command is used to exfiltrate the contents of the "passwd" or "shadow" file of the bot's host.

### Syntax

GET_PASSWD {PASSWD|SHADOW}

### Parameters
- **PASSWD|SHADOW**: PASSWD for the passwd file, SHADOW for the shadow file

### Examples
- **Example 1**:
    GET_PASSWD PASSWD

- **Example 2**:
    GET_PASSWD SHADOW

### Notes
* This command returns each line of one of the aforementioned files
* This command is expected to be replaced by a more sophisticated file exfiltration command in the future

---
