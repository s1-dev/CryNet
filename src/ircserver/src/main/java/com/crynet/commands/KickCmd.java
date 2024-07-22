package com.crynet.commands;

import com.crynet.ClientData;
import com.crynet.Server;
import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;
import com.crynet.utils.StringUtils;

public class KickCmd extends Command {
    private ChannelManager channelManager;
    private ConnectionManager connectionManager;
    private ClientData sender;
    private boolean isMasterMsg;
    private String hostname;
    private final String MSG_SYNTAX = ":%s!%s@%s KICK %s %s \n";
    private final String SUCCESS_MSG = "User kicked! \n";
    private final String ERROR_MSG_1 = "Insufficient permissions to KICK \n";
    private final String ERROR_MSG_2 = "Inputted channel does not exist \n";
    private final String ERROR_MSG_3 = "Inputted nickname is not currently in said channel \n";
    private final int PARAM_COUNT = 4;
    private final int MAX_COMMENT_LEN = 50;

    public KickCmd(String[] params, Connection connection, boolean isMasterMsg, Server srvInstance) {
        super(params, connection);
        this.isMasterMsg = isMasterMsg;
        this.channelManager = srvInstance.getChannelManager();
        this.connectionManager = srvInstance.getConnectionManager();
        this.sender = connection.getClientData();
        this.hostname = srvInstance.getConfig().getServerHostname();
    }

    @Override
    public void performDuty() {
        if (!isMasterMsg) {
            connection.messageClient(ERROR_MSG_1);
            return;
        }

        if (!channelManager.channelNameExists(params[1])) {
            connection.messageClient(ERROR_MSG_2);
            return;
        }

        Channel specifiedChannel = channelManager.getChannel(params[1]);
        if (!specifiedChannel.containsNick(params[2])) {
            connection.messageClient(ERROR_MSG_3);
            return;
        }

        String kickMsg = StringUtils.joinStringArray(params, 3);
        Connection connectionToKick = connectionManager.getConnectionViaNick(params[2]);
        
        specifiedChannel.removeUserViaNick(params[2]);
        sendKickMessage(connectionToKick, kickMsg, specifiedChannel);
        connectionToKick.getClientData().removeChannel(specifiedChannel);
        channelManager.removeIfEmpty(specifiedChannel.getName());
        connection.messageClient(SUCCESS_MSG);
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.KICK;
    }


    private void sendKickMessage(Connection receiver, String message, Channel channel) {
        String kickMessage = String.format(
            MSG_SYNTAX,
            sender.getNickname(),
            sender.getUsername(),
            hostname,
            channel.getName(),
            message.substring(1)
        );
        receiver.messageClient(kickMessage);
    }


    /*
     * KICK command syntax:
     *  /KICK <CHANNEL> <NICKNAME> :<COMMENT> 
     */
    protected void checkParams() {
        if (params.length < PARAM_COUNT) {
            isValid = false;
            return;
        }
    
        if(StringUtils.joinStringArray(params, 3).length() > MAX_COMMENT_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
