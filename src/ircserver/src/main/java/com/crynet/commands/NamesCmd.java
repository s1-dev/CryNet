package com.crynet.commands;

import java.util.List;

import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;

public class NamesCmd extends Command {
    private ChannelManager channelManager;
    private final int PARAM_COUNT = 2;
    private final String BASE_MESSAGE = "Names for the following channels: \n";

    public NamesCmd(String[] params, Connection connection, ChannelManager channelManager) {
        super(params, connection);
        this.channelManager = channelManager;
    }

    @Override
    public void performDuty() {
        String[] channelNames = params[1].split(",");

        StringBuilder result = new StringBuilder(BASE_MESSAGE);
        for (String channelName : channelNames) {
            if (channelManager.getChannel(channelName) != null) {
                Channel currChannel = channelManager.getChannel(channelName);
                String namesList = buildNamesList(currChannel.getConnectedNicks());
                String channelLine = String.format("%s :%s \n", currChannel.getName(), namesList);
                result.append(channelLine);
            } else {
                result.append(String.format("The specified channel, %s, does not exist \n", channelName));
            }
        }
        connection.messageClient(result.toString());
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.NAMES;
    }

    private String buildNamesList(List<String> nicks) {
        StringBuilder result = new StringBuilder();
        for (String nickname : nicks) {
            result.append(String.format("%s ", nickname));
        }
        return result.toString();
    }

    /*
     * NAMES command syntax:
     *  /NAMES [<CHANNEL>]
     */    
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        isValid = true;
    }

}
