package com.crynet.commands;

import com.crynet.ClientData;
import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;
import com.crynet.utils.StringUtils;

public class PartCmd extends Command {
    private ChannelManager channelManager;
    private ClientData sender; 
    private String hostname;
    private final String MSG_SYNTAX = ":%s!%s@%s PART %s %s \n";
    private final String ERROR_MSG = "The following errors occured: \n";
    private final int PARAM_COUNT = 2;
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

        String joinedMessage = StringUtils.joinStringArray(params, 2);
        for (String channelName : channels) {
            Channel currChannel = null;
            if (channelManager.channelNameExists(channelName)) {
                currChannel = channelManager.getChannel(channelName);
            } else {
                errorMsg.append(String.format("The %s channel does not exist \n", channelName));
                continue;
            }
            if (currChannel != null && currChannel.containsUser(connection)) {
                if (params.length == PARAM_COUNT)
                    messageChannel(currChannel, joinedMessage);
                currChannel.removeConnectedUser(connection);
                channelManager.removeIfEmpty(currChannel.getName());
                sender.removeChannel(currChannel);
            } else {
                String currError = String.format("You are not a part of the %s channel \n", currChannel.getName());
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

    private void messageChannel(Channel channel, String message) {
        String partingMessage = String.format(
                    MSG_SYNTAX,
                    sender.getNickname(),
                    sender.getRealname(),
                    hostname,
                    channel.getName(),
                    message.substring(1)
        );
        channel.broadcastMessage(partingMessage, connection, true);
    }

    /*
     * PART command syntax:
     *  /PART [<CHANNEL>,...] :<MESSAGE>
     */
    protected void checkParams() {
        if (params.length < PARAM_COUNT) {
            System.out.println("case1");
            isValid = false;
            return;
        }

        if (params[1].length() > MAX_CHANNELS_INPUT) {
            System.out.println("case2");
            isValid = false;
            return;
        }

        if (params.length >= PARAM_COUNT && StringUtils.joinStringArray(params, 2).length() > MAX_MSG_LEN) {
            System.out.println("case3");
            isValid = false;
            return;
        }

        if (params.length >= PARAM_COUNT && params[2].charAt(0) != ':') { // If the optional message is set
            System.out.println("case4");
            isValid = false;
            return;
        }

        isValid = true;
    }

}
