package com.crynet;

import java.util.ArrayList;
import java.util.List;

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

    public void handleConnectionCommand(Connection connection, String command) {
        ircServerInstance.parseCommand(connection, command);
    }
}
