package com.crynet.commands;

import com.crynet.ClientData;
import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class PartCmd extends Command {
    private ChannelManager channelManager;
    private ClientData sender; 
    private String hostname;
    private final String MSG_SYNTAX = ":%s!%s@%s PART %s %s";
    private final String ERROR_MSG = "The following errors occured: \n";
    private final int PARAM_COUNT = 3;
    private final int MAX_MSG_LEN = 500;
    private final int MAX_CHANNELS_INPUT = 500;

    public PartCmd(String[] params, Connection connection, ChannelManager channelManager, String hostname) {
        super(params, connection);
        this.sender = connection.getClientData();
        this.channelManager = channelManager;
        this.hostname = hostname;
    }

    @Override
    public void performDuty() {
        String[] channels = params[1].split(",");

        StringBuilder errorMsg = new StringBuilder(ERROR_MSG);

        
        for (String channelName : channels) {
            Channel currChannel = channelManager.getChannel(channelName);
            if (currChannel != null && currChannel.containsUser(connection)) {
                if (params.length == PARAM_COUNT)
                    messageChannel(currChannel);
                currChannel.removeConnectedUser(connection);
                channelManager.removeIfEmpty(currChannel.getName());
            } else {
                String currError = !currChannel.containsUser(connection) 
                ? String.format("You are not a part of the %s channel \n", currChannel.getName()) 
                : String.format("The %s channel does not exist \n", currChannel.getName());
                errorMsg.append(currError);
            }
        }

        if (!errorMsg.toString().equals(ERROR_MSG)) {
            connection.messageClient(errorMsg.toString());
        }
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.PART;
    }

    private void messageChannel(Channel channel) {
        String partingMessage = String.format(
                    MSG_SYNTAX,
                    sender.getNickname(),
                    sender.getRealname(),
                    hostname,
                    channel.getName(),
                    params[2]
        );
        channel.broadcastMessage(partingMessage);
    }

    /*
     * PART command syntax:
     *  /PART [<CHANNEL>,...] :<MESSAGE>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT || params.length != PARAM_COUNT-1) {
            isValid = false;
            return;
        }

        if (params[1].length() > MAX_CHANNELS_INPUT) {
            isValid = false;
            return;
        }

        if (params[2].length() > MAX_MSG_LEN) {
            isValid = false;
            return;
        }

        if (params.length == PARAM_COUNT && params[2].charAt(0) != ':') { // If the optional message is set
            isValid = false;
            return;
        }

        isValid = true;
    }

}
