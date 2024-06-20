package com.crynet.commands;

import org.apache.commons.codec.digest.DigestUtils;

import com.crynet.Config;
import com.crynet.Server;
import com.crynet.connections.Connection;

public class ShutdownCmd extends Command {
    private Server srvInstance;
    private boolean isMasterMsg;
    private Config serverConfig;
    private final String ERROR_MSG_1 = "Insufficient permissions for shutdown";
    private final String ERROR_MSG_2 = "Unable to shutdown :/";
    private final String SUCCESS_MSG = "Server shutting down!";
    private final int PARAM_COUNT = 2;

    public ShutdownCmd(String[] params, Connection connection, boolean isMasterMsg, Server srvInstance) {
        super(params, connection);
        this.srvInstance = srvInstance;
        this.isMasterMsg = isMasterMsg;
        this.serverConfig = srvInstance.getConfig();
    }

    @Override
    public void performDuty() {
        if (!isMasterMsg) {
            connection.messageClient(ERROR_MSG_1);
            return;
        }

        String userShutdownToken = params[1];
        String serverSideSalt = serverConfig.getShutdownSalt();
        String sha256sum = DigestUtils.sha256Hex(userShutdownToken + serverSideSalt).toLowerCase();

        if (!sha256sum.equals(serverConfig.getShutdownDigest())) {
            connection.messageClient(ERROR_MSG_2);
            return;
        }

        connection.messageClient(SUCCESS_MSG);
        srvInstance.stop();
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.SHUTDOWN;
    }
    
    /*
     * SHUTDOWN command syntax:
     *  /SHUTDOWN <SHUTDOWN_TOKEN>
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
