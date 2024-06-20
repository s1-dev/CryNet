package com.crynet.connections;

import java.util.ArrayList;
import java.util.List;

import com.crynet.ClientData;
import com.crynet.Server;

public class ConnectionManager {
    private List<Connection> connections;
    private Server ircServerInstance;
    private int maxConnections;
    private String masterConnectionId;
    private final String MASTER_NOT_SET = "MASTER CONNECTION IS NOT SET";

    public ConnectionManager(Server ircServerInstance, int maxConnections) {
        this.connections = new ArrayList<>();
        this.ircServerInstance = ircServerInstance;
        this.maxConnections = maxConnections;
        this.masterConnectionId = MASTER_NOT_SET;
    }

    public synchronized Connection getConnectionViaNick(String nickname) {
        for (Connection connection : connections) {
            if (connection.getClientData().getNickname().equals(nickname))
                return connection;
        }
        return null;
    }

    public synchronized void addConnection(Connection connection) {
        connections.add(connection);
    }

    public synchronized void closeConnection(Connection connection) {
        killConnection(connection);
    }

    public synchronized void closeAllConnections() {
        for (Connection connection : connections) {
            killConnection(connection);
        }
    }

    public synchronized boolean connectionRegistered(String nickname) {
        for (Connection conn : connections) {
            ClientData client = conn.getClientData();
            if (client != null) {
                if (client.getNickname().equals(nickname)) {
                    return true;
                }
            }
        }
        return false;
    }

    public void handleConnectionInput(Connection connection, String command) {
        ircServerInstance.parseRawInput(connection, command);
    }

    public synchronized boolean isAtCapacity() {
        return connections.size() >= maxConnections;
    }

    public synchronized void markMasterConnection(String connectionId) {
        if(!masterConnectionId.equals(MASTER_NOT_SET) && containsConnection(connectionId)) {
            this.masterConnectionId = connectionId;
        }
    }

    public synchronized boolean isMasterConnection(String connectionId) {
        return connectionId.equals(masterConnectionId);
    }

    private synchronized boolean containsConnection(String connectionId) {
        for (Connection connection : connections) {
            if (connection.getId().equals(connectionId)) 
                return true;
        }
        return false;
    }

    private void killConnection(Connection connection) {
        connections.remove(connection);
        connection.kill();
    }
}
