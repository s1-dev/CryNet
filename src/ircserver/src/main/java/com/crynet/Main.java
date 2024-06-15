package com.crynet;

import com.fasterxml.jackson.core.exc.StreamReadException;
import com.fasterxml.jackson.databind.DatabindException;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.io.File;
import java.io.IOException;

public class Main {

    // TODO fix issue where absolute path is required
    private static String PATH = "<INSERT_PATH>";

    public static void main(String[] args) throws StreamReadException, DatabindException, IOException {
        // Open configuration file
        ObjectMapper objectMapper = new ObjectMapper();
        Config srvConfig = objectMapper.readValue(new File(PATH), Config.class);
        srvConfig.setIsEncrypted();    

        // start server
        IrcServer server = new IrcServer(srvConfig);
        server.start();
    }
}
