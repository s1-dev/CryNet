package com.crynet.commands;

import com.crynet.Config;
import com.crynet.connections.Connection;

public class CommandBuilder {

    public static Command buildCommand(boolean isMasterMsg, String[] params, Connection connection, Config srvConfig) {
        Command cmd = null;
        CommandType commandType = determineCommandType(params[0]);
        System.out.println("COMMAND TYPE: " + commandType);
        if(commandType.equals(CommandType.VALIDATE)) {
            cmd = new ValidateCmd(isMasterMsg, params, connection, srvConfig);
            if (!cmd.isValidCommand())
                return null;
        } else { // if unknown, return null
            return null;
        }

        return cmd;
    }

    private static CommandType determineCommandType(String commandIdentifier) {
        if(commandIdentifier.equals("/VALIDATE")) {
            return CommandType.VALIDATE;
        }
        return CommandType.UNKNOWN;
    }
}
