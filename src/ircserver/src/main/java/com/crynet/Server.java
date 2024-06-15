package com.crynet;

import com.crynet.connections.Connection;

public interface Server {
    public Config getConfig();
    public void parseRawInput(Connection connection, String command);
}
