package com.crynet.commands;

import com.crynet.connections.Connection;

public class UserCmd extends Command {
    private final int PARAM_COUNT = 5;
    private final int MAX_USERNAME_LEN = 30;
    private final int MAX_REALNAME_LEN = 30;
    private final String ERROR_MSG = "Nickname must be set first";
    private final String SUCCESS_MSG = "Username and realname set";

    public UserCmd(String[] params, Connection connection) {
        super(params, connection);
    }

    @Override
    public void performDuty() {
        if (connection.getClientData().getNickname() == null) { // Nick must be set first
            connection.messageClient(ERROR_MSG);
            return;
        }
        connection.getClientData().setUsername(params[1]);
        connection.getClientData().setRealname(params[4]);

        connection.messageClient(SUCCESS_MSG);
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.USER;
    }

    /*
     * USER command syntax:
     *  /USER <USERNAME> 0 * <REALNAME>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        if (params[1].length() > MAX_USERNAME_LEN) { 
            isValid = false;
            return;
        }

        if (!params[2].equals("0")) {
            isValid = false;
            return;
        }

        if (!params[3].equals("*")) {
            isValid = false;
            return;
        }

        if (params[4].length() > MAX_REALNAME_LEN) { 
            isValid = false;
            return;
        }

        isValid = true;
    }

}
