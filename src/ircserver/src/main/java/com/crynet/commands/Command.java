package com.crynet.commands;

public interface Command {
    public void performDuty();
    public CommandType getCommandType();
    public boolean isValidCommand();
}
