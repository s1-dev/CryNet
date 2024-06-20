package com.crynet.channels;

import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import com.crynet.connections.Connection;

public class Channel {
    private String channelId;
    private String channelName;
    private String channelTopic;
    private List<Connection> connectedUsers;
    private int maxUsers;
    private final String DEFAULT_TOPIC = "Topic not set for the %s channel";
    
    public Channel(String channelName) {
        this.channelName = prependPoundSymbol(channelName);
        this.channelId = UUID.randomUUID().toString();
        this.channelTopic = String.format(DEFAULT_TOPIC, channelName);
    }

    private String prependPoundSymbol(String channelName) {
        if (channelName.charAt(0) != '#') {
            return String.format("#%s", channelName);
        }
        return channelName;
    }

    public synchronized void addConnectedUser(Connection connectedUser) {
        if (!connectedUsers.contains(connectedUser) && connectedUsers.size() < maxUsers) {
            connectedUsers.add(connectedUser);
            connectedUser.messageClient(channelTopic);
        }
    }

    public synchronized void removeConnectedUser(Connection connectedUser) {
        if (connectedUsers.contains(connectedUser)) {
            connectedUsers.remove(connectedUser);
        }
    }

    public synchronized void removeUserViaNick(String nickname) {
        Connection connectedUser = searchForNick(nickname);
        if (connectedUser != null) {
            connectedUsers.remove(connectedUser);
        } 
    }

    public synchronized List<String> getConnectedNicks() {
        List<String> nicks = new LinkedList<>();
        for (Connection connectedUser : connectedUsers) {
            nicks.add(connectedUser.getClientData().getNickname());
        }
        return nicks;
    }

    public synchronized void broadcastMessage(String message) {
        for (Connection user : connectedUsers) {
            user.messageClient(message);
        }
    }

    public synchronized int getUserCount() {
        return connectedUsers.size();
    }

    public synchronized boolean containsUser(Connection connectedUser) {
        return connectedUsers.contains(connectedUser);
    }

    public synchronized boolean containsNick(String nickname) {
        return searchForNick(nickname) != null;
    }

    public String getId() {
        return channelId;
    }

    public String getName() {
        return channelName;
    }

    public String getTopic() {
        return channelTopic;
    }

    public synchronized void setTopic(String channelTopic, Connection connectedUser) {
        if(connectedUsers.contains(connectedUser)) {
            this.channelTopic = channelTopic;
        }
    }

    public synchronized void setMaxChannelUsers(int maxUsers) {
        this.maxUsers = maxUsers;
    }

    private Connection searchForNick(String nickname) {
        for (Connection connectedUser : connectedUsers) {
            if (connectedUser.getClientData().getNickname().equals(nickname))
                return connectedUser;
        }
        return null;
    }
}
