package com.crynet;

import com.fasterxml.jackson.core.exc.StreamReadException;
import com.fasterxml.jackson.databind.DatabindException;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.io.File;
import java.io.IOException;

public class Main {
    private static final String CONFIG_PATH = "src/main/resources/config.json"; // Starting from where the pom.xml is located
    public static void main(String[] args) throws StreamReadException, DatabindException, IOException {
        // Open configuration file
        ObjectMapper objectMapper = new ObjectMapper();
        Config srvConfig = objectMapper.readValue(new File(CONFIG_PATH), Config.class);
        srvConfig.setIsEncrypted();    

        // start server
        IrcServer server = new IrcServer(srvConfig);
        server.start();
    }
}
