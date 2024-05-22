package com.crynet;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.util.UUID;

public class ConnectionHandler implements Connection {
    private Socket clientSocket;
    private String connectionId;
    private BufferedWriter output;
    private BufferedReader input;
    private boolean isAlive;
    private boolean isValidated;
    private ClientData client;
    private ConnectionManager connectionManager;

    public ConnectionHandler(Socket clientSocket, ConnectionManager connectionManager) {
        this.clientSocket = clientSocket;
        this.connectionId = UUID.randomUUID().toString();
        this.connectionManager = connectionManager;
        isAlive = true;
        isValidated = false;
    }

    @Override
    public void run() {
        try {
            output = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
            input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

            while(isAlive) {
                handleCommand(input.readLine());
            }

            clean();
        } catch (IOException e) {
            e.printStackTrace();
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

    @Override
    public ClientData getClientData() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getClientData'");
    }

    private void handleCommand(String command) {
        try {
            output.write("TEST123"); // NOT WORKING FIX
            output.flush();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.out.println(command);
        if(command != null) {
            connectionManager.handleConnectionCommand(this, command);
        } else {
            clean();
        }
    }

    private void clean() {
        connectionManager.closeConnection(this);
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
    public void messageClient(String serverResponse) {
        try {
            System.out.println("naw this hit");
            output.write(serverResponse);
            output.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
