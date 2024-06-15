package com.crynet.messages;

import com.crynet.commands.Command;

public class CommandMessage implements ExternalMessage {
    private String msgContents;
    private String connectionId;
    private Command command;

    public CommandMessage(String msgContents, String connectionId) {
        this.msgContents = msgContents;
        this.connectionId = connectionId;
    }

    @Override
    public String getMessageContents() {
        return msgContents;
    }

    @Override
    public String getConnectionId() {
        return connectionId;
    }

    @Override
    public Command getCommand() {
        return command;
    }

    @Override
    public void setCommand(Command command) {
        this.command = command;
    }
}
