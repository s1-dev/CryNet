package com.crynet.messages;

public class InternalErrorMessage implements Message {
    private String errorMsg;

    public InternalErrorMessage(String errorMsg) {
        this.errorMsg = errorMsg;
    }

    @Override
    public String getMessageContents() {
        return errorMsg;
    }
}
