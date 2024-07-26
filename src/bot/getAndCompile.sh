#!/bin/bash

# THIS SCRIPT IS FOR THOSE WHO WANT/NEED TO MANUALLY COMPILE THE BUG BY GETTING IT FROM A WEB SERVER
# This is mostly for debugging

if [[ $# -ne 1 ]]; then
	printf "Correct syntax: ./getCppFile.sh <URL>\n"
	exit 1
fi

url=$1

wget -r -np -nH --cut-dirs=1 "${url}/include"
wget -r -np -nH --cut-dirs=1 "${url}/src"
curl "${url}/Makefile" -o Makefile
curl "${url}/manualCompilationScript.sh" -o manualCompilationScript.sh

sed -i 's/\r//' manualCompilationScript.sh
chmod 777 manualCompilationScript.sh
./manualCompilationScript.sh
mv $0 ..
mv irc_bot .. # name of out file
rm *
mv ../$0 .
mv ../irc_bot .

exit 0