package com.crynet.commands;

import org.apache.commons.codec.digest.DigestUtils;

import com.crynet.Config;
import com.crynet.Server;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;

public class ValidateCmd extends Command {
    private boolean isMasterMsg;
    private Config srvConfig;
    private ConnectionManager connectionManager;
    private final int PARAM_COUNT = 3;
    private final String ERROR_MSG_1 = "Already validated";
    private final String ERROR_MSG_2 = "Unable to connect :/";
    private final String SUCCESS_MSG = "Validation sucessful, USER and NICK now";

    public ValidateCmd(String[] params, Connection connection, boolean isMasterMsg, Server srvInstance) {
        super(params, connection);
        this.isMasterMsg = isMasterMsg;
        this.connectionManager = srvInstance.getConnectionManager();
        this.srvConfig = srvInstance.getConfig();
    }

    @Override
    public void performDuty() {
        if (connection.isValidated()) {
            connection.messageClient(ERROR_MSG_1);
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

        if(failedCheck) { // On auth failure, kill connection
            connection.messageClient(ERROR_MSG_2);
            connectionManager.closeConnection(connection);
        } else {
            System.out.println("was validated");
            connection.messageClient(SUCCESS_MSG);
            connection.validate();
        }
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.VALIDATE;
    }

    /*
     * VALIDATE command syntax:
     *  /VALIDATE <PASSPHRASE> <MASTER|BOT>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }
        isValid = true;
    }

}
