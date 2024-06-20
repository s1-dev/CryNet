package com.crynet.messages;

import com.crynet.commands.Command;

public class CommandMessage implements ExternalMessage {
    private String msgContents;
    private Command command;

    public CommandMessage(String msgContents) {
        this.msgContents = msgContents;
    }

    @Override
    public String getMessageContents() {
        return msgContents;
    }

    public Command getCommand() {
        return command;
    }

    public void setCommand(Command command) {
        this.command = command;
    }

    @Override
    public void proceedWithAction() {
        command.performDuty();
    }
}
