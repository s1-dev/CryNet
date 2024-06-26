package com.crynet.channels;

import java.util.Collection;
import java.util.HashMap;

public class ChannelManager {
    // KEY->NAME VAL->CHANNEL OBJ
    private HashMap<String, Channel> activeChannels;
    private int maxChannels;
    private int maxChannelUsers;

    public ChannelManager(int maxChannels, int maxChannelUsers) {
        activeChannels = new HashMap<>();
        this.maxChannels = maxChannels;
        this.maxChannelUsers = maxChannelUsers;
    }

    public void addChannel(Channel channel) {
        if (activeChannels.size() <= maxChannels) {
            activeChannels.put(channel.getName(), channel);
            channel.setMaxChannelUsers(maxChannelUsers);
        }
    }

    public void removeIfEmpty(String channelName) {
        if (activeChannels.get(channelName).getUserCount() == 0) {
            removeChannel(channelName);
        }
    } 

    public void forceRemove(String channelName) {
        removeChannel(channelName);
    }

    public Channel getChannel(String channelName) {
        return activeChannels.get(channelName);
    }

    public boolean channelNameExists(String channelName) {
        return activeChannels.containsKey(channelName);
    } 

    public Collection<Channel> getAllChannels() {
        return activeChannels.values();
    }

    public boolean isAtMax() {
        return activeChannels.size() == maxChannels;
    }

    private void removeChannel(String channelName) {
        activeChannels.remove(channelName);
    }
}
