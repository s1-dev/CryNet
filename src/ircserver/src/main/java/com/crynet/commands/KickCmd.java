package com.crynet.commands;

import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class KickCmd extends Command {
    private ChannelManager channelManager;
    private boolean isMasterMsg;
    private final String SUCCESS_MSG = "User kicked!";
    private final String ERROR_MSG_1 = "Insufficient permissions to KICK";
    private final String ERROR_MSG_2 = "Inputted channel does not exist";
    private final String ERROR_MSG_3 = "Inputted nickname is not currently in said channel";
    private final int PARAM_COUNT = 4;
    private final int MAX_COMMENT_LEN = 50;

    public KickCmd(String[] params, Connection connection, boolean isMasterMsg, ChannelManager channelManager) {
        super(params, connection);
        this.isMasterMsg = isMasterMsg;
        this.channelManager = channelManager;
    }

    @Override
    public void performDuty() {
        if (!isMasterMsg) {
            connection.messageClient(ERROR_MSG_1);
            return;
        }

        if (channelManager.getChannel(params[1]) == null) {
            connection.messageClient(ERROR_MSG_2);
            return;
        }

        Channel specifiedChannel = channelManager.getChannel(params[1]);
        if (specifiedChannel.containsNick(params[2])) {
            connection.messageClient(ERROR_MSG_3);
            return;
        }

        specifiedChannel.removeUserViaNick(params[2]);
        channelManager.removeIfEmpty(specifiedChannel.getName());
        connection.messageClient(SUCCESS_MSG);
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.KICK;
    }


    /*
     * KICK command syntax:
     *  /KICK <CHANNEL> <NICKNAME> <COMMENT> 
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }
        
        if (params[1].charAt(0) != '#') {
            isValid = false;
            return;
        }

        if(params[3].length() > MAX_COMMENT_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
