package com.crynet;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class IrcServer implements Server {
    // Declare private/global vars for class
    private ServerSocket serverSocket;
    private Config srvConfig;
    private ConnectionManager connectionManager; // instantiate in main or server?

    public IrcServer(Config srvConfig) {
        this.srvConfig = srvConfig;
        this.connectionManager = new ConnectionManager(this);
        try {
            this.serverSocket = new ServerSocket(srvConfig.getPortNumber());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void start() {
        // start server
        System.out.printf("%s IRC server starting...\n", srvConfig.getServerHostname());
        acceptConnections();
    }

    public void stop() {
        // stop server
        System.out.println("Server stopping...");
        // Free necessary memory (if JVM doesn't do that for some reason)
        
        System.exit(0); // Will this suffice? Or do we need a different means 
    }

    public Config getConfig() {
        return srvConfig;
    }

    public void parseCommand(Connection connection, String command) {
        // check if validated
        // if not validated and command is to validate proceed
        // else send error and terminate, or ask to validate?
        // if validated then parse command and check if is coming from master or bot
        if (!connection.isValidated()) {
            System.out.println("this hit");
            connection.messageClient("Validation required"); // hard coded, maybe make static class with errors
        }
    }

    private void acceptConnections() {
        boolean run = true;
        while(run) {
            try {
                Socket clientSocket = serverSocket.accept();
                Connection newConnection = new ConnectionHandler(clientSocket, connectionManager);
                connectionManager.addConnection(newConnection);
                new Thread(newConnection).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
