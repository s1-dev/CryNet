package com.crynet.commands;

import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;

public class NickCmd extends Command {
    private ConnectionManager connectionManager;
    private final String SUCCESS_MSG = "Nickname set";
    private final String ERROR_MSG = "Nickname is already in use";
    private final int PARAM_COUNT = 2;
    private final int MAX_NICK_LEN = 20;
    private final int MIN_NICK_LEN = 2;

    public NickCmd(String[] params, Connection connection, ConnectionManager connectionManager) {
        super(params, connection);
        this.connectionManager = connectionManager;
    }

    @Override
    public void performDuty() {
        if (connectionManager.connectionRegistered(params[1])) { // There are not allowed to be duplicate nicknames
            connection.messageClient(ERROR_MSG);
            return;
        }
        connection.getClientData().setNickname(params[1]);

        connection.messageClient(SUCCESS_MSG);
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.NICK;
    }

    /*
     * NICK command syntax:
     *  /NICK <NICKNAME>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        if (params[1].length() > MAX_NICK_LEN && params[1].length() < MIN_NICK_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
