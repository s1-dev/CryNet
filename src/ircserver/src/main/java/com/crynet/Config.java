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
    private String masterAuthDigest;
    private String botAuthDigest;

    public String getServerHostname() {
        return serverHostname;
    }

    public void setServerHostname(String serverHostname) {
        this.serverHostname = serverHostname;
    }

    public int getPortNumber() {
        return portNumber; 
    }

    public void setPortNumber(int portNumber) {
        this.portNumber = portNumber;
    }

    public void setMaxChannels(int maxChannels) {
        this.maxChannels = maxChannels;
    }

    public int getMaxChannels() {
        return maxChannels;
    }

    public void setMaxMessageLength(int maxMessageLength) {
        this.maxMessageLength = maxMessageLength;
    }

    public int getMaxMessageLength() {
        return maxMessageLength;
    }

    public void setMaxNickLength(int maxNickLength) {
        this.maxNickLength = maxNickLength;
    }

    public int getMaxNickLength() {
        return maxNickLength;
    }

    public void setMaxConnections(int maxConnections) {
        this.maxConnections = maxConnections;
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
}
