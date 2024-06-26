package com.crynet.commands;

import com.crynet.Server;
import com.crynet.connections.Connection;

public class CommandBuilder {

    public static Command buildCommand(boolean isMasterMsg, String[] params, Connection connection, Server srvInstance) {
        Command cmd = null;
        CommandType commandType = determineCommandType(params[0]);
        
        if (commandType.equals(CommandType.VALIDATE)) { // Find a more programmatic approach
            cmd = new ValidateCmd(params, connection, isMasterMsg, srvInstance);
        } else if (commandType.equals(CommandType.USER)) {
            cmd = new UserCmd(params, connection);
        } else if (commandType.equals(CommandType.NICK)) {
            cmd = new NickCmd(params, connection, srvInstance.getConnectionManager());
        } else if (commandType.equals(CommandType.JOIN)) {
            cmd = new JoinCmd(params, connection, srvInstance.getChannelManager());
        } else if (commandType.equals(CommandType.TOPIC)) {
            cmd = new TopicCmd(params, connection, srvInstance.getChannelManager());
        } else if (commandType.equals(CommandType.KICK)) {
            cmd = new KickCmd(params, connection, isMasterMsg, srvInstance);
        } else if (commandType.equals(CommandType.LIST)) {
            cmd = new ListCmd(params, connection, srvInstance.getChannelManager());
        } else if (commandType.equals(CommandType.SHUTDOWN)) {
            cmd = new ShutdownCmd(params, connection, isMasterMsg, srvInstance);        
        } else if (commandType.equals(CommandType.PRIVMSG)) {
            cmd = new PrivmsgCmd(params, connection, srvInstance);
        } else if (commandType.equals(CommandType.PART)) {
            cmd = new PartCmd(params, connection, srvInstance.getChannelManager(), srvInstance.getConfig().getServerHostname());
        } else if (commandType.equals(CommandType.QUIT)) {
            cmd = new QuitCmd(params, connection, srvInstance);
        } else if (commandType.equals(CommandType.NAMES)) {
            cmd = new NamesCmd(params, connection, srvInstance.getChannelManager());
        } else { // if unknown, return null
            return null;
        }

        if (!cmd.isValidCommand())
            return null;

        return cmd;
    }

    private static CommandType determineCommandType(String commandIdentifier) {
        if (commandIdentifier.equals("/VALIDATE")) {
            return CommandType.VALIDATE;
        } else if (commandIdentifier.equals("/USER")) {
            return CommandType.USER;
        } else if (commandIdentifier.equals("/NICK")) {
            return CommandType.NICK;
        } else if (commandIdentifier.equals("/JOIN")) {
            return CommandType.JOIN;
        } else if (commandIdentifier.equals("/TOPIC")) {
            return CommandType.TOPIC;
        } else if (commandIdentifier.equals("/KICK")) {
            return CommandType.KICK;
        } else if (commandIdentifier.equals("/LIST")) {
            return CommandType.LIST;
        } else if (commandIdentifier.equals("/SHUTDOWN")) {
            return CommandType.SHUTDOWN;
        } else if (commandIdentifier.equals("/PRIVMSG")) {
            return CommandType.PRIVMSG;
        } else if (commandIdentifier.equals("/PART")) {
            return CommandType.PART;
        } else if (commandIdentifier.equals("/QUIT")) {
            return CommandType.QUIT;
        } else if (commandIdentifier.equals("/NAMES")) {
            return CommandType.NAMES;
        }
        return CommandType.UNKNOWN;
    }
}
