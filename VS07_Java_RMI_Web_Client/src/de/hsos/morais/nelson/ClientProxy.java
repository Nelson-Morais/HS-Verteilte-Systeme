package de.hsos.morais.nelson;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.List;

public interface ClientProxy extends Remote {
    public void receiveMessage(String username, String message) throws RemoteException;

    ArrayList<String> getMessages() throws RemoteException;
    public void clearMessages() throws RemoteException;
}
