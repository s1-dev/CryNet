package com.crynet.commands;

import com.crynet.connections.Connection;

public abstract class Command {
    protected String[] params;
    protected Connection connection;
    protected boolean isValid;
    
    public Command(String[] params, Connection connection) {
        this.params = params;
        this.connection = connection;
        checkParams();
    }


    public abstract void performDuty();
    public abstract CommandType getCommandType();
    public boolean isValidCommand() { return isValid; }
    protected abstract void checkParams();
}
