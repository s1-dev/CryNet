package com.crynet.commands;

import com.crynet.ClientData;
import com.crynet.Server;
import com.crynet.channels.Channel;
import com.crynet.channels.ChannelManager;
import com.crynet.connections.Connection;
import com.crynet.connections.ConnectionManager;

public class PrivmsgCmd extends Command {
    private ConnectionManager connectionManager;
    private ChannelManager channelManager;
    private ClientData sender;
    private String hostname;
    private final int PARAM_COUNT = 3;
    private final int MAX_MSG_LEN = 500;
    private final String MSG_SYNTAX = ":%s!%s@%s PRIVMSG %s %s";
    private final String ERROR_MSG = "The following receivers did not exist: \n";

    public PrivmsgCmd(String[] params, Connection connection, Server srvInstance) {
        super(params, connection);
        this.sender = connection.getClientData();
        this.hostname = srvInstance.getConfig().getServerHostname();
        this.connectionManager = srvInstance.getConnectionManager();
        this.channelManager = srvInstance.getChannelManager();
    }

    @Override
    public void performDuty() {
        String[] receivers = params[1].split(",");

        StringBuilder result = new StringBuilder(ERROR_MSG);

        for (String receiver : receivers) {
            boolean receiverExists = false;
            boolean isChannel = receiver.charAt(0) == '#';
            String message = String.format(
                MSG_SYNTAX,
                sender.getNickname(),
                sender.getRealname(),
                hostname,
                receiver,
                params[2]
            );

            if (isChannel) { // Receiver is a channel
                receiverExists = messageChannel(receiver.substring(1), message);
            } else { // Receiver is a nickname
                receiverExists = messageUser(receiver, message);
            }

            if (!receiverExists) {
                String subErrorMsg = isChannel ? String.format("The channel %s \n", receiver) : String.format("The nickname %s \n", receiver);
                result.append(subErrorMsg);
            }
        }

        if (!result.toString().equals(ERROR_MSG)) // If some receivers did not exist
            connection.messageClient(result.toString());
    }

    @Override
    public CommandType getCommandType() {
        return CommandType.PRIVMSG;
    }

    private boolean messageChannel(String channelName, String message) {
        Channel channel = channelManager.getChannel(channelName);
        if (channel != null) {
            channel.broadcastMessage(message); 
        }
        return false;
    }

    private boolean messageUser(String nickname, String message) {
        Connection connectionWithNick = connectionManager.getConnectionViaNick(nickname);
        if (connectionWithNick != null) {
            connectionWithNick.messageClient(message);
            return true;
        }
        return false;
    }

    /*
     * PRIVMSG command syntax:
     *  /PRIVMSG [<RECEIVER>,...] :<MESSAGE>
     *  <RECEIVER> = (#<CHANNEL_NAME>|<NICKNAME>)
     */
    protected void checkParams() {
        if (params.length != PARAM_COUNT) {
            isValid = false;
            return;
        }

        if (params[2].charAt(0) != ':') {
            isValid = false;
            return;
        }

        if (params[2].length() > MAX_MSG_LEN) {
            isValid = false;
            return;
        }

        isValid = true;
    }
}
