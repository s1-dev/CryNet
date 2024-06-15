package com.crynet.channels;

import java.util.List;
import java.util.UUID;

import com.crynet.connections.Connection;

public class Channel {
    private String channelId;
    private String channelName;
    private String channelTopic;
    private List<Connection> connectedUsers;
    
    public Channel(String channelName, String channelTopic) {
        this.channelName = channelName;
        this.channelTopic = channelTopic;
        this.channelId = UUID.randomUUID().toString();
    }


    public synchronized void broadcastMessage(String message) {
        for (Connection user : connectedUsers) {
            user.messageClient(message);
        }
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

}
