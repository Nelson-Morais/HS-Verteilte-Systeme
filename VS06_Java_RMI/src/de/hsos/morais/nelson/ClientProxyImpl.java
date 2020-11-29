package de.hsos.morais.nelson;

import java.rmi.RemoteException;

public class ClientProxyImpl implements ClientProxy {

    @Override
    public void receiveMessage(String username, String message) throws RemoteException {
        System.out.println(username + ": " + message);
    }
}
