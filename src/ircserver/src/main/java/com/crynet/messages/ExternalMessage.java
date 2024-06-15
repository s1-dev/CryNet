package com.crynet.messages;

import com.crynet.commands.Command;

public interface ExternalMessage extends Message {
    public Command getCommand();
    public void setCommand(Command command);
}
