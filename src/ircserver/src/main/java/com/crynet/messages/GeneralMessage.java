package com.crynet.messages;

import java.util.List;

import com.crynet.ClientData;
import com.crynet.channels.Channel;
import com.crynet.connections.Connection;

public class GeneralMessage implements ExternalMessage {
    private String messageContents;
    private Connection connection;
    private ClientData sender;
    private String hostname;
    private final String MSG_SYNTAX = ":%s!%s@%s MSG %s %s \n";

    public GeneralMessage(String messageContents, Connection connection, String hostname) {
        this.messageContents = messageContents;
        this.hostname = hostname;
        this.connection = connection;
        this.sender = connection.getClientData();
    }

    @Override
    public String getMessageContents() {
        return messageContents;
    }

    @Override
    public void proceedWithAction() { // For now, general message is sent to all channels the user is connected to
        List<Channel> allConnectedChannels = sender.getAllConnectedChannels();
        
        for (Channel channel : allConnectedChannels) {
            String formattedMessage = String.format(
                MSG_SYNTAX,
                sender.getNickname(),
                sender.getUsername(),
                hostname,
                channel.getName(),
                messageContents
            );
            channel.broadcastMessage(formattedMessage, connection, true);
        }
    }
}
