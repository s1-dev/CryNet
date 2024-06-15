package com.crynet.connections;

import java.util.ArrayList;
import java.util.List;

import com.crynet.ClientData;
import com.crynet.Server;

public class ConnectionManager {
    private List<Connection> connections;
    private Server ircServerInstance;

    public ConnectionManager(Server ircServerInstance) {
        this.connections = new ArrayList<>();
        this.ircServerInstance = ircServerInstance;
    }

    public synchronized void addConnection(Connection connection) {
        connections.add(connection);
    }

    public synchronized void closeConnection(Connection connection) {
        connections.remove(connection);
        connection.kill();
    }

    public synchronized void validatedConnection(Connection connection, ClientData client) {
        connection.validate();
    }

    public void handleConnectionInput(Connection connection, String command) {
        ircServerInstance.parseRawInput(connection, command);
    }
}
