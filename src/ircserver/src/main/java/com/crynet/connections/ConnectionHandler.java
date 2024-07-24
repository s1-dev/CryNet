package com.crynet.connections;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.util.UUID;

import com.crynet.ClientData;

public class ConnectionHandler implements Connection {
    private Socket clientSocket;
    private String connectionId;
    private BufferedWriter output;
    private BufferedReader input;
    private boolean isAlive;
    private boolean isValidated;
    private ConnectionManager connectionManager;
    private ClientData client;

    public ConnectionHandler(Socket clientSocket, ConnectionManager connectionManager) {
        this.clientSocket = clientSocket;
        this.connectionId = UUID.randomUUID().toString();
        this.connectionManager = connectionManager;
        isAlive = true;
        isValidated = false;
        client = new ClientData();
    }

    @Override
    public void run() {
        try {
            output = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
            input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

            connectionManager.sendWelcomeMessage(this);

            while(isAlive) {
                handleInput(input.readLine());
            }   
            
            clean();
        } catch (IOException e) {
            e.printStackTrace();
            connectionManager.closeConnection(this);
        }
    }

    @Override
    public String getId() {
        return connectionId;
    }

    @Override
    public void kill() {
        isAlive = false;
    }

    private void handleInput(String rawInput) {
        if(rawInput != null) {
            connectionManager.handleConnectionInput(this, rawInput);
        } else {
            clean();
            connectionManager.closeConnection(this);
        }
    }

    private void clean() {
        try {
            clientSocket.close();
            output.close();
            input.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void validate() {
        this.isValidated = true;
    }

    @Override
    public boolean isValidated() {
        return isValidated;
    }

    @Override
    public void messageClient(String serverResponse) { // TODO append newline char before messaging client and remove from formatted messages
        if (!isMasterConnection()) {
            serverResponse = serverResponse.replace("\n", " ");
            serverResponse = serverResponse + "\n";
        }
        try {
            output.write(serverResponse);
            output.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public ClientData getClientData() {
        return client;
    }

    @Override
    public void setClientData(ClientData client) {
        this.client = client;
    }

    @Override
    public boolean isMasterConnection() {
        return connectionManager.isMasterConnection(connectionId);
    }
}
