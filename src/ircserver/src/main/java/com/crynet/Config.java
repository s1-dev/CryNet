package com.crynet;

public class Config {
    private String serverHostname;
    private int portNumber; // standard to use 6667 for unencrypted and 6697 for SSL/TLS
    private boolean isEncrypted; // set true if port is 6697
    private Operator operator;
    private int maxChannels;
    private int maxMessageLength;
    private int maxNickLength;
    private int maxConnections;
    private int maxChannelUsers;
    private String shutdownDigest;
    private String shutdownSalt;
    private String masterAuthDigest;
    private String botAuthDigest;

    public String getServerHostname() {
        return serverHostname;
    }

    public int getPortNumber() {
        return portNumber; 
    }

    public int getMaxChannels() {
        return maxChannels;
    }

    public int getMaxMessageLength() {
        return maxMessageLength;
    }

    public int getMaxNickLength() {
        return maxNickLength;
    }

    public int getMaxConnections() {
        return maxConnections;
    }

    public boolean isEncrypted() {
        return isEncrypted;
    }

    public void setIsEncrypted() {
        this.isEncrypted = this.portNumber == 6697;
    }

    public Operator getOperator() {
        return operator;
    }

    public String getMasterAuthDigest() {
        return masterAuthDigest;
    }

    public String getBotAuthDigest() {
        return botAuthDigest;
    }

    public int getMaxChannelUsers() {
        return maxChannelUsers;
    }

    public String getShutdownDigest() {
        return shutdownDigest;
    }

    public String getShutdownSalt() {
        return shutdownSalt;
    }
}
