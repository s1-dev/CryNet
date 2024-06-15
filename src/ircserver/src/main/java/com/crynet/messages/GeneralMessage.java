package com.crynet.messages;

public class GeneralMessage implements Message {
    private String messageContents;
    private String connectionId;

    public GeneralMessage(String messageContents, String connectionId) {
        this.messageContents = messageContents;
        this.connectionId = connectionId;
    }


    public String getMessageContents() {
        return messageContents;
    }

    public String getConnectionId() {
        return connectionId;
    }

    public void outputMessage() {
        // TODO: SEND MSG TO CHANNEL
    }

}
