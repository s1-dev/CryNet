package com.crynet.messages;

import com.crynet.Server;
import com.crynet.commands.Command;
import com.crynet.commands.CommandBuilder;
import com.crynet.commands.CommandType;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;

public class MessageParser {
    public Message parseMessage(Connection connection, String rawInput, Server srvInstance) {
        Message msg = null;
        String[] params = rawInput.split(" ");
        String identifier = params[params.length-1]; // bot or master will be specified at the end of each 
        boolean isMasterMsg = connection.isValidated() ? connection.isMasterConnection() : identifier.equals("MASTER");

        if (!connection.isValidated() && !identifier.equals("BOT") && !identifier.equals("MASTER")) {
            return new InternalErrorMessage("ERROR: Unknown identifier"); 
        }

        if (params[0].charAt(0) == '/') {
            msg = new CommandMessage(rawInput);
            Command builtCommand = CommandBuilder.buildCommand(isMasterMsg, params, connection, srvInstance);
            ((CommandMessage) msg).setCommand(builtCommand);
        } else {
            msg = new GeneralMessage(rawInput, connection.getClientData(), srvInstance.getConfig().getServerHostname());
        }

        if (isMasterMsg && isValidateMsg(msg))
            setMasterConnection(connection.getId(), srvInstance.getConnectionManager());

        return msg;
    }

    private boolean isValidateMsg(Message msg) {
        return msg instanceof CommandMessage && ((CommandMessage)msg).getCommand().getCommandType().equals(CommandType.VALIDATE);
    }

    private void setMasterConnection(String connectionId, ConnectionManager connectionManager) {
        connectionManager.markMasterConnection(connectionId);
    }
}
