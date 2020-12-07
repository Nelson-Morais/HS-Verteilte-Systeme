package de.hsos.morais.nelson;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ChatProxy extends Remote {
    public void sendMessage (String username, String message) throws RemoteException;
}
