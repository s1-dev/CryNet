package com.crynet.commands;

import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class TopicCmd extends Command {
    private ChannelManager channelManager;
    private final int PARAM_COUNT = 3;
    private final int MAX_TOPIC_LEN = 20;
    private final String SUCCESS_MSG = "Topic of the %s channel changed";
    private final String ERROR_MSG = "You are not in the %s channel";

    public TopicCmd(String[] params, Connection connection, ChannelManager channelManager) {
        super(params, connection);
        this.channelManager = channelManager;
    }

    @Override
    public void performDuty() {
        Channel deseiredChannel = channelManager.getChannel(params[1]);
        if (deseiredChannel.containsUser(connection)) {
            deseiredChannel.setTopic(params[2], connection);
            deseiredChannel.broadcastMessage(String.format("TOPIC has been changed to: %s", params[2]));
        } else {
            connection.messageClient(String.format(ERROR_MSG, deseiredChannel.getName()));
            return;
        }
        connection.messageClient(String.format(SUCCESS_MSG, deseiredChannel.getName()));
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.TOPIC;
    }

    /*
     * TOPIC command syntax:
     *  /TOPIC <CHANNEL_NAME> <TOPIC_CONTENTS>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        if (channelManager.getChannel(params[1]) == null) {
            isValid = false;
            return;
        }

        if (params[2].length() > MAX_TOPIC_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
