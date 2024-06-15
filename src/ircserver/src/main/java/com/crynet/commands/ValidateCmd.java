package com.crynet.commands;

import org.apache.commons.codec.digest.DigestUtils;

import com.crynet.Config;
import com.crynet.connections.Connection;

public class ValidateCmd implements Command { // Custom IRC command
    private String[] params;
    private boolean isValid;
    private boolean isMasterMsg;
    private Config srvConfig;
    private Connection connection;
    private CommandType commandType;
    private final int PARAM_COUNT = 3;
    private final String ERROR_MSG = "Unable to connect :/";
    private final String SUCCESS_MSG = "Validation sucessful, USER and NICK now";

    public ValidateCmd(boolean isMasterMsg, String[] params, Connection connection, Config srvConfig) {
        this.params = params;
        this.isMasterMsg = isMasterMsg;
        this.connection = connection;
        this.srvConfig = srvConfig;
        this.commandType = CommandType.VALIDATE;
        checkParams();
    }

    @Override
    public void performDuty() {
        // check auth token of either master or bot
        // tell user to user USER and NICK commands 
        if (!isValid) {
            connection.messageClient(ERROR_MSG);
            connection.kill();
            return;
        }

        String sha256sum = DigestUtils.sha256Hex(params[1]).toLowerCase();
        boolean failedCheck = false;

        if(isMasterMsg) {
            if(!sha256sum.equals(srvConfig.getMasterAuthDigest().toLowerCase())) {
                failedCheck = true;
            }
        } else {
            if (!sha256sum.equals(srvConfig.getBotAuthDigest().toLowerCase())) {
                failedCheck = true;
            }
        }

        if(failedCheck) {
            connection.messageClient(ERROR_MSG);
            connection.kill();
        } else {
            connection.messageClient(SUCCESS_MSG);
            connection.validate();
        }
    }

    @Override
    public CommandType getCommandType() {
        return commandType;
    }

    @Override
    public boolean isValidCommand() {
        return isValid;
    }

    /*
     * VALIDATE command syntax:
     *  /VALIDATE <AUTH_TOKEN> <MASTER|BOT>
     */
    private void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        if(isMasterMsg && !params[2].equals("MASTER")) {
            isValid = false;
            return;
        }
        isValid = true;
    }

}
