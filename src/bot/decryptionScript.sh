#!/bin/bash

BOT_NAME="temp"

printf "Uh oh, your files have been encrypted!\n"

printf "To decrypt, answer this... what is the answer to life the universe and everything?\n"

while true; do
    # Prompt the user for input
    echo -n "Solution: "
    read userInput

    # Check if the user input is 4
    if [ "$userInput" -eq 42 ]; then
        break
    else
        echo "Incorrect, try again."
    fi
done

printf "CONGRATS!!! You are correct.\n"
printf "To decrypt your files, follow the below instructions\n"

printf "Here is the location of the bot that caused your predicatment:\n"
find / -type f -name $BOT_NAME 2>/dev/null

printf "Now, decrypt your files by issuing the following command in the directory of the bot:\n"
printf "$BOT_NAME DECRYPT UnhackablePassword123!\n"