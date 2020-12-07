package de.hsos.morais.nelson;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;

public class ChatClient {
    static Scanner scanner;
    public static String username = "";

    public static void main(String[] args) {
        Registry registry = null;
        ChatServer chatServer;
        ChatProxy chatProxy = null;
        try {
            registry = LocateRegistry.getRegistry();
            chatServer = (ChatServer) registry.lookup("ChatServer");
            System.out.println("Server found"); //Flush input

            scanner = new Scanner(System.in);
            while (true) {
                System.out.println("1 - Subscribe\n2 - Unsubscribe\n3 - Send Message\n4 - Exit");
                int action = scanner.hasNextInt() ? scanner.nextInt() : 0;
                scanner.nextLine();
                switch (action) {
                    case 1 -> {
                        if (!username.isEmpty()) {
                            System.out.println("You are already subscribed.");
                            break;
                        }
                        ClientProxyImpl clientProxy = new ClientProxyImpl();
                        ClientProxy handle = (ClientProxy) UnicastRemoteObject.exportObject(clientProxy, 0);
                        System.out.print("Username: ");
                        username = scanner.nextLine();
                        chatProxy = chatServer.subscribeUser(username, handle);
                        if (chatProxy == null) {
                            username = "";
                            System.err.println("Error occured while subscribing. Please try again.");
                        }
                        System.out.println("Successfully subscribed");
                    }
                    case 2 -> {
                        if (username.isEmpty()) {
                            System.out.println("You are already unsubscribed");
                            break;
                        }
                        if (chatServer.unsubscribeUser(username)) {
                            System.out.println("Successfully unsubscribed");
                            username = "";
                        } else System.err.println("Error occured while unsubscribing.");
                    }
                    case 3 -> {
                        if (!username.isEmpty()) {
                            System.out.print("Message: ");
                            assert chatProxy != null;
                            chatProxy.sendMessage(scanner.nextLine());
                            break;
                        }
                        System.err.println("You aren't subscribed yet. Please subscribe before sending messages");
                    }
                    case 4 -> {
                        if (!username.isEmpty()) {
                            if (chatServer.unsubscribeUser(username)) {
                                System.out.println("Successfully unsubscribed. Bye bye.");
                                username = "";
                            } else System.err.println("Error occured while unsubscribing. Exiting anyways.");
                        }
                        System.exit(0);
                    }
                    default -> System.out.println("Input invalid. Please try again.");
                }
            }
        } catch (Exception exception) {
            System.out.println("Couldn't reach server. Exiting program.");
            System.exit(-1);
        }
    }

}
