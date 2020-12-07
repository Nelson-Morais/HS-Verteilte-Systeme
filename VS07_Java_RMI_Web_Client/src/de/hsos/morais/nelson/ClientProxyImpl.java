package de.hsos.morais.nelson;

import java.rmi.RemoteException;
import java.util.ArrayList;

public class ClientProxyImpl implements ClientProxy {
    ArrayList<String> messages;

 public ClientProxyImpl() throws RemoteException{
     messages = new ArrayList<>();
 }
    @Override
    public void receiveMessage(String username, String message) throws RemoteException {
        System.out.println(username + ": " + message);
        messages.add(username + ": " + message);
    }
    public ArrayList<String> getMessages() throws RemoteException{
     return messages;
    }

    public void clearMessages() throws RemoteException{
     messages.clear();
    }
}
