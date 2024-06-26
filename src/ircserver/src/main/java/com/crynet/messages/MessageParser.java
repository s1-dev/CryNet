package com.crynet.messages;

import com.crynet.Server;
import com.crynet.commands.Command;
import com.crynet.commands.CommandBuilder;
import com.crynet.connections.Connection;

public class MessageParser {
    public Message parseMessage(Connection connection, String rawInput, Server srvInstance) {
        if (rawInput.replace(" ", "").equals("")) {
            return new InternalErrorMessage("ERROR: No message contents \n");
        }
        
        Message msg = null;
        String[] params = rawInput.split(" ");
        String identifier = params[params.length-1]; // bot or master will be specified at the end of each 
        boolean isMasterMsg = connection.isValidated() ? connection.isMasterConnection() : identifier.equals("MASTER");

        if (!connection.isValidated() && !identifier.equals("BOT") && !identifier.equals("MASTER")) {
            return new InternalErrorMessage("ERROR: Unknown identifier \n"); 
        }

        if (params.length > 0 && params[0].charAt(0) == '/') {
            msg = new CommandMessage(rawInput);
            Command builtCommand = CommandBuilder.buildCommand(isMasterMsg, params, connection, srvInstance);
            ((CommandMessage) msg).setCommand(builtCommand);
        } else {
            msg = new GeneralMessage(rawInput, connection, srvInstance.getConfig().getServerHostname());
        }

        return msg;
    }
}
