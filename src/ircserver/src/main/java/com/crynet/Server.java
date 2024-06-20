package com.crynet;

import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;

public interface Server {
    public Config getConfig();
    public void parseRawInput(Connection connection, String command);
    public ChannelManager getChannelManager();
    public ConnectionManager getConnectionManager();
    public void stop();
}
