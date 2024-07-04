package com.crynet;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;

public class ClientData {
    private String username;
    private String realname;
    private String nickname;
    private List<Channel> connectedChannels;

    public ClientData() {
        this.connectedChannels = new ArrayList<>();
    }

    public void addChannel(Channel channel) {
        connectedChannels.add(channel);
    }

    public void removeChannel(Channel channel) {
        connectedChannels.remove(channel);
    }

    public Channel getConnectedChannel(String channelName) {
        for (Channel channel : getAllConnectedChannels()) {
            if (channel.getName().equals(channelName))
                return channel;
        }
        return null;
    }

    public int countOfConnectedChannels() { 
        return connectedChannels.size();
    }

    public void leaveAllChannels(ChannelManager channelManager) { // Find better solution than passing ChannelManager object to method
        for (Channel channel : getAllConnectedChannels()) {
            channel.removeUserViaNick(nickname);
            channelManager.removeIfEmpty(channel.getName());
            removeChannel(channel);
        }
    }

    public List<Channel> getAllConnectedChannels() {
        return Collections.unmodifiableList(new ArrayList<>(connectedChannels)); // Immutable list in order to avoid java.util.ConcurrentModificationException
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getRealname() {
        return realname;
    }

    public void setRealname(String realname) {
        this.realname = realname;
    }

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public boolean isRegistered() {
        return nickname != null && username != null && realname != null;
    }
}
