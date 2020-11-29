package de.hsos.morais.nelson;

import de.hsos.morais.nelson.ChatProxy;
import de.hsos.morais.nelson.ChatServerImpl;

import java.rmi.RemoteException;

public class ChatProxyImpl implements ChatProxy {
    ChatServerImpl chatServer=null;
    String username;
    public ChatProxyImpl(ChatServerImpl chatServer, String username) {
        this.chatServer=chatServer;
        this.username = username;
    }

    @Override
    public void sendMessage(String message) throws RemoteException {
        System.out.print("Sending message: "+ message + " from " + username + " to ");
        chatServer.getClients().forEach((key, value) -> {
            try {
                value.receiveMessage(username, message);
                System.out.print(key + ", ");
            } catch (RemoteException remoteException) {
                System.err.println("Can't receive subscribed client.");
                remoteException.printStackTrace();
            }
        });
        System.out.println();
    }
}
