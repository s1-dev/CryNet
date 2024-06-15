package com.crynet;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import com.crynet.channels.ChannelManager;
import com.crynet.commands.ValidateCmd;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionHandler;
import com.crynet.connections.ConnectionManager;
import com.crynet.messages.ExternalMessage;
import com.crynet.messages.GeneralMessage;
import com.crynet.messages.InternalErrorMessage;
import com.crynet.messages.Message;
import com.crynet.messages.MessageParser;

public class IrcServer implements Server {
    // Declare private/global vars for class
    private ServerSocket serverSocket;
    private Config srvConfig;
    private ConnectionManager connectionManager; // instantiate in main or server?
    private MessageParser messageParser;
    private ChannelManager channelManager;

    public IrcServer(Config srvConfig) {
        this.srvConfig = srvConfig;
        this.connectionManager = new ConnectionManager(this);
        this.channelManager = new ChannelManager();
        this.messageParser = new MessageParser(2); // Minparams is 2 for now TEMP
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
        
        if(msg instanceof GeneralMessage) {
            if(connection.isValidated()) { // also check if user is in channel
                ((GeneralMessage)msg).outputMessage();
            } else {
                connection.messageClient("ERROR: Validate and join channel in order to send general messages");
            }
            return;
        }

        ExternalMessage externalMessage = (ExternalMessage) msg;

        // if msg command is null, then send general command error
        if (externalMessage.getCommand() == null) {
            connection.messageClient("ERROR: Unable to ascertain command syntax");
            return;
        }

        // check if validated
        // if not validated and command is to validate proceed
        // else send error and terminate, or ask to validate?
        // if validated then parse command and check if is coming from master or bot
        if (!connection.isValidated() && externalMessage.getCommand() instanceof ValidateCmd == false) {
            connection.messageClient("TODO: ERROR MESSAGE"); // hard coded, maybe make static class with errors
            return;
        }
        
        externalMessage.getCommand().performDuty();
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
