package com.crynet;

import java.util.ArrayList;
import java.util.List;

import com.crynet.channels.Channel;

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
        for (Channel channel : connectedChannels) {
            if (channel.getName().equals(channelName))
                return channel;
        }
        return null;
    }

    public List<Channel> getAllConnectedChannels() {
        return connectedChannels;
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
