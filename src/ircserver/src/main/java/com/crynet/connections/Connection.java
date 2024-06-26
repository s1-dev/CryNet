package com.crynet.connections;

import com.crynet.ClientData;

public interface Connection extends Runnable {
    public String getId();
    public void kill();
    public ClientData getClientData();
    public void setClientData(ClientData client);
    public void validate();
    public boolean isValidated();
    public boolean isMasterConnection();
    public void messageClient(String serverResponse); 
}
