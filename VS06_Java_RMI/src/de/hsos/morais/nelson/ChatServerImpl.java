package de.hsos.morais.nelson;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.util.Map;

public class ChatServerImpl extends UnicastRemoteObject implements ChatServer {
    Map<String, ClientProxy> clients = new HashMap<>();

    protected ChatServerImpl() throws RemoteException { }

    @Override
    public ChatProxy subscribeUser(String username, ClientProxy handle) {
        clients.put(username, handle);
        System.out.println(username + " registered.");

        ChatProxyImpl chatProxyImpl = new ChatProxyImpl(this, username);
        ChatProxy chatProxy = null;
        try {
            chatProxy = (ChatProxy) UnicastRemoteObject.exportObject(chatProxyImpl,0);
        } catch (RemoteException remoteException) {
            remoteException.printStackTrace();
            System.out.println("Can't register " + username);
            return null;
        }
        return chatProxy;
    }

    @Override
    public boolean unsubscribeUser(String username) {
        return clients.remove(username) != null;
    }

    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.createRegistry(Registry.REGISTRY_PORT);
//            ChatServerImpl chatServer = new ChatServerImpl();
//            ChatServer stub = (ChatServer) UnicastRemoteObject.exportObject(chatServer, 0);
            Naming.rebind("ChatServer", new ChatServerImpl());    //Server registriert URL für entferntes Object, rebind zum sicherstellen, dass erneut gebunden wird.
            System.out.println("Server registered");
        } catch (RemoteException e) {
            System.err.println("Exception occurred while registering the server.");
            e.printStackTrace();
            System.exit(-1);
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }

    }
    public Map<String, ClientProxy> getClients() {
        return clients;
    }
}
