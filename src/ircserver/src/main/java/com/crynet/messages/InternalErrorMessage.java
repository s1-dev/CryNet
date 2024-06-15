package com.crynet.messages;

public class InternalErrorMessage implements Message {
    private String errorMsg;
    private String connectionId;

    public InternalErrorMessage(String errorMsg, String connectionId) {
        this.errorMsg = errorMsg;
        this.connectionId = connectionId;
    }

    @Override
    public String getMessageContents() {
        return errorMsg;
    }

    @Override
    public String getConnectionId() {
        return connectionId;
    }
}
