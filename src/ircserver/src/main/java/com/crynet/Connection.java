package com.crynet;

public interface Connection extends Runnable {
    public String getId();
    public void kill();
    public ClientData getClientData();
    public void validate();
    public boolean isValidated();
    public void messageClient(String serverResponse);
}
