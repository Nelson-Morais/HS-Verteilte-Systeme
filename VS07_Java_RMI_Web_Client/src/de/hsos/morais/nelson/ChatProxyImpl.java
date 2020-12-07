package de.hsos.morais.nelson;

import de.hsos.morais.nelson.ChatProxy;
import de.hsos.morais.nelson.ChatServerImpl;

import java.rmi.RemoteException;
import java.util.ArrayList;

public class ChatProxyImpl implements ChatProxy {
    ChatServerImpl chatServer=null;
    String username;
    public ChatProxyImpl(ChatServerImpl chatServer) {
        this.chatServer=chatServer;
    }

    @Override
    public void sendMessage(String username, String message) throws RemoteException {
        ArrayList<String> deadClients = new ArrayList<>();
        System.out.print("Sending message: "+ message + " from " + username + " to ");
        chatServer.getClients().forEach((key, value) -> {
            try {
                value.receiveMessage(username, message);
                System.out.print(key + ", ");
            } catch (RemoteException remoteException) {
                System.out.println();
                System.err.println("Can't reach " + key + ". Deleting client from list");
                deadClients.add(key);
            }
        });
        deadClients.forEach((value) -> {
          chatServer.getClients().remove(value);
        });
        System.out.println();
    }
}
