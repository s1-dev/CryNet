package com.crynet.commands;

import com.crynet.ClientData;
import com.crynet.Server;
import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;

public class QuitCmd extends Command {
    private ChannelManager channelManager;
    private ConnectionManager connectionManager;
    private String hostname;
    private final ClientData sender;
    private final String MSG_SYNTAX = ":%s!%s@%s QUIT %s %s";
    private final int PARAM_COUNT = 2;
    private final int MAX_MSG_LEN = 100;

    public QuitCmd(String[] params, Connection connection, Server srvInstance) {
        super(params, connection);
        this.sender = connection.getClientData();
        this.hostname = srvInstance.getConfig().getServerHostname();
        this.channelManager = srvInstance.getChannelManager();
        this.connectionManager = srvInstance.getConnectionManager();
    }

    @Override
    public void performDuty() {
        if (params.length != PARAM_COUNT) {  
            connectionManager.closeConnection(connection);
            return;
        }

        for (Channel currChannel : connection.getClientData().getAllConnectedChannels()) {
            String quitingMessage = String.format(
                MSG_SYNTAX,
                sender.getNickname(),
                sender.getRealname(),
                hostname,
                currChannel.getName(),
                params[2]
            );
            currChannel.broadcastMessage(quitingMessage);
            currChannel.removeConnectedUser(connection);
            channelManager.removeIfEmpty(currChannel.getName());
        }
        connectionManager.closeConnection(connection);
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.QUIT;
    }

    /*
     * QUIT command syntax:
     *  /QUIT <MESSAGE>
     */    
    protected void checkParams() {
        if (params.length != PARAM_COUNT || params.length != PARAM_COUNT-1) {
            isValid = false;
            return;
        }

        if (params[1].length() > MAX_MSG_LEN) {
            isValid = false;
            return;
        }

        if (params.length == PARAM_COUNT && params[1].charAt(0) != ':') { // If the optional message is set and missing colon
            isValid = false;
            return;
        }

        isValid = true;
    }

}
