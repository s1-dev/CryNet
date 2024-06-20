package com.crynet.commands;

import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class JoinCmd extends Command {
    private ChannelManager channelManager;
    private final String SUCCESS_MSG = "Joined all channels";
    private final String ERROR_MSG_1 = "Register before joining a channel";
    private final String ERROR_MSG_2 = "Maximum channels already reached";
    private final int PARAM_COUNT = 2;;

    public JoinCmd(String[] params, Connection connection, ChannelManager channelManager) {
        super(params, connection);
        this.channelManager = channelManager;
    }

    @Override
    public void performDuty() {
        if (!connection.getClientData().isRegistered()) {
            connection.messageClient(ERROR_MSG_1);
            return;
        }

        String[] channelsToJoin = params[1].split(",");
        int channelsAdded = 0;

        for (String channelName : channelsToJoin) {
            if (channelManager.isAtMax()) {
                connection.messageClient(ERROR_MSG_2);
                break;
            }
            mutateChannel(channelName);
            channelsAdded++;
        }

        if (channelsAdded < channelsToJoin.length) {
            String errorMsg = channelsAdded == 1 ? "Only 1 channel added" : String.format("Only %d channels added", channelsAdded);
            connection.messageClient(errorMsg);
        } else {
            connection.messageClient(SUCCESS_MSG);
        }
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.JOIN;
    }

    private void mutateChannel(String channelName) {
        if (channelManager.getChannel(channelName) != null) {
            Channel specifiedChannel = channelManager.getChannel(channelName);
            specifiedChannel.addConnectedUser(connection);
            connection.getClientData().addChannel(specifiedChannel);
        } else {
            Channel newChannel = new Channel(channelName);
            newChannel.addConnectedUser(connection);
            channelManager.addChannel(newChannel);
            connection.getClientData().addChannel(newChannel);
        }
    }

    /*
     * JOIN command syntax:
     *  /JOIN <CHANNEL>{,<CHANNEL>}
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
