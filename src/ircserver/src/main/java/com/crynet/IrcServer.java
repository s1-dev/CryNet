package com.crynet;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import com.crynet.channels.ChannelManager;
import com.crynet.commands.CommandType;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionHandler;
import com.crynet.connections.ConnectionManager;
import com.crynet.messages.CommandMessage;
import com.crynet.messages.ExternalMessage;
import com.crynet.messages.GeneralMessage;
import com.crynet.messages.InternalErrorMessage;
import com.crynet.messages.Message;
import com.crynet.messages.MessageParser;

public class IrcServer implements Server {
    // Declare private/global vars for class
    private ServerSocket serverSocket;
    private Config srvConfig;
    private ConnectionManager connectionManager; 
    private MessageParser messageParser;
    private ChannelManager channelManager;

    public IrcServer(Config srvConfig) {
        this.srvConfig = srvConfig;
        this.connectionManager = new ConnectionManager(this, srvConfig.getMaxConnections());
        this.channelManager = new ChannelManager(srvConfig.getMaxChannels(), srvConfig.getMaxChannelUsers());
        this.messageParser = new MessageParser();
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

    public void stop() { // This will be accessed if Master program instructs server to shutdown, via a SHUTDOWN command
        // stop server
        System.out.println("Server stopping...");
        // Free necessary memory (if JVM doesn't do that for some reason)

        connectionManager.closeAllConnections();
        System.exit(0); // Will this suffice? Or do we need a different means 
    }

    public Config getConfig() {
        return srvConfig;
    }

    public ChannelManager getChannelManager() {
        return channelManager;
    }

    public void parseRawInput(Connection connection, String rawInput) {
        Message msg = messageParser.parseMessage(connection, rawInput, this);

        if (msg instanceof InternalErrorMessage) {
            connection.messageClient(msg.getMessageContents());
            return;
        }

        ExternalMessage externalMessage = (ExternalMessage) msg;

        if (externalMessage instanceof GeneralMessage) {
            if (!connection.isValidated() && connection.getClientData().getAllConnectedChannels().isEmpty() && !connection.getClientData().isRegistered()) {
                connection.messageClient("ERROR: Validate, register, and join a channel in order to send general messages");
                return;
            }
        }

        if (externalMessage instanceof CommandMessage) {
            if (((CommandMessage)externalMessage).getCommand() == null) {
                connection.messageClient("ERROR: Incorrect command syntax");
                return;
            }

            if (!connection.isValidated() && ((CommandMessage)externalMessage).getCommand().getCommandType() != CommandType.VALIDATE) {
                connection.messageClient("ERROR: Validate and register before making commands");
                return;
            }

            if (!connection.getClientData().isRegistered()) {
                connection.messageClient("ERROR: Must be registed to perform more commands");
            }
        }

        externalMessage.proceedWithAction();
    }

    private void acceptConnections() {
        boolean run = true;
        while(run) {
            try {
                Socket clientSocket = serverSocket.accept();
                Connection newConnection = new ConnectionHandler(clientSocket, connectionManager);
                if (connectionManager.isAtCapacity()) {
                    clientSocket.close();
                    continue; // TEST IF THIS WORKS
                }
                connectionManager.addConnection(newConnection);
                new Thread(newConnection).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public ConnectionManager getConnectionManager() {
        return connectionManager;
    }
}
