package com.crynet;

public interface Server {
    public Config getConfig();
    public void parseCommand(Connection connection, String command);
}
