package com.crynet.commands;

import java.util.Collection;

import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class ListCmd extends Command {
    private ChannelManager channelManager;
    private boolean isValid;
    private final int PARAM_COUNT = 2;
    private final int MAX_INPUT_LEN = 200;

    public ListCmd(String[] params, Connection connection, ChannelManager channelManager) {
        super(params, connection);
        this.channelManager = channelManager;
    }

    @Override
    public void performDuty() {
        String[] channelNames = getChannelNames(params[1]);

        StringBuilder result = new StringBuilder("Channels list: \n");
        for (String channelName : channelNames) {
            if (channelManager.getChannel(channelName) != null) {
                Channel currChannel = channelManager.getChannel(channelName);
                String channelLine = String.format("%s - %d users \n", currChannel.getName(), currChannel.getUserCount());
                result.append(channelLine);
            } else {
                result.append(String.format("The specified channel, %s, does not exist \n", channelName));
            }
        }
        connection.messageClient(result.toString());
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.LIST;
    }

    @Override
    public boolean isValidCommand() {
        return isValid;
    }

    private String[] getChannelNames(String channelList) {
        if (channelList.equals("*")) {
            Collection<Channel> allChannels = channelManager.getAllChannels();
            String[] channelNames = new String[allChannels.size()];
            int i = 0;

            for (Channel channel : allChannels) {
                channelNames[i] = channel.getName();
                i++;
            }

            return channelNames;
        } 
        return channelList.split(",");
    }

    /*
     * LIST command syntax:
     *  /LIST <*|[<CHANNEL>]>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        } 

        if (params[1].length() > MAX_INPUT_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    } 
}
