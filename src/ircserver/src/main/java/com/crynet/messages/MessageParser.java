package com.crynet.messages;

import com.crynet.Server;
import com.crynet.commands.Command;
import com.crynet.commands.CommandBuilder;
import com.crynet.connections.Connection;

public class MessageParser {

    private int minParams;

    public MessageParser(int minParams) {
        this.minParams = minParams;
    }

    /*
     * GENERAL MESSAGE SYNTAX:
     * <COMMMAND_IDENTIFIER> <PARAM> ... <PARAM> <AUTH_STRING> <IDENTIFIER>
     */
    public Message parseMessage(Connection connection, String rawInput, Server srvInstance) {
        Message msg = null;
        String[] params = rawInput.split(" ");
        String identifier = params[params.length-1]; // bot or master will be specified at the end of each 

        if (params.length < minParams) {
            return new InternalErrorMessage(String.format("ERROR: %d is below the minimum params of %d", params.length, minParams), connection.getId());
        }

        if (!identifier.equals("BOT") && !identifier.equals("MASTER")) {
            return new InternalErrorMessage("ERROR: Unknown identifier", connection.getId()); 
        }

        if (params[0].charAt(0) == '/') {
            msg = new CommandMessage(rawInput, connection.getId());
            Command builtCommand = CommandBuilder.buildCommand(identifier.equals("MASTER"), params, connection, srvInstance.getConfig());
            ((ExternalMessage) msg).setCommand(builtCommand);
        } else {
            msg = new GeneralMessage(rawInput, connection.getId());
            // DO we want to have the general message have a command where it just puts what it's saying into the channel it's in?
            // what else to do?
        }

        return msg;
    }
}
