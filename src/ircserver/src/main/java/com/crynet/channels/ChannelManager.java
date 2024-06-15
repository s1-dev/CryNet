package com.crynet.channels;

import java.util.HashMap;
import java.util.List;

public class ChannelManager {
    // KEY->ID VAL->CHANNEL OBJ
    private HashMap<String, Channel> activeChannels;

    public ChannelManager() {
        activeChannels = new HashMap<>();
    }

    public void addChannel(Channel channel) {
        activeChannels.put(channel.getId(), channel);
    }

    public void removeChannel(String channelId) {
        activeChannels.remove(channelId);
    }

    public Channel getChannel(String channelId) {
        return activeChannels.get(channelId);
    }

    public List<Channel> getAllChannels() {
        return (List<Channel>) activeChannels.values();
    }
}
