package com.crynet.commands;

import com.crynet.utils.StringUtils;
import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class TopicCmd extends Command {
    private ChannelManager channelManager;
    private final int PARAM_COUNT = 3;
    private final int MAX_TOPIC_LEN = 200;
    private final String SUCCESS_MSG = "Topic of the %s channel changed \n";
    private final String ERROR_MSG_1 = "Given channel does not exist \n";
    private final String ERROR_MSG_2 = "You are not in the %s channel \n";

    public TopicCmd(String[] params, Connection connection, ChannelManager channelManager) {
        super(params, connection);
        this.channelManager = channelManager;
    }

    @Override
    public void performDuty() {
        if (!channelManager.channelNameExists(params[1])) {
            connection.messageClient(ERROR_MSG_1);
        }
        String topicMsg = StringUtils.joinStringArray(params, 2);
        Channel deseiredChannel = channelManager.getChannel(params[1]);
        if (deseiredChannel.containsUser(connection)) {
            deseiredChannel.setTopic(topicMsg.substring(1), connection);
            deseiredChannel.broadcastMessage(String.format("TOPIC has been changed to: %s", topicMsg.substring(1)), connection, false);
        } else {
            connection.messageClient(String.format(ERROR_MSG_2, deseiredChannel.getName()));
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
     *  /TOPIC <CHANNEL_NAME> :<TOPIC_CONTENTS>
     */
    protected void checkParams() {
        if (params.length < PARAM_COUNT) { // since message will be all params past first 2 params
            isValid = false;
            return;
        }

        if (params[2].charAt(0) != ':') {
            isValid = false;
            return;
        }

        if (StringUtils.joinStringArray(params, 2).length() > MAX_TOPIC_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
