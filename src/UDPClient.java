
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.*;



public class UDPClient {
    static final int maxChunkSize = 512;
   static final int clientPort = 4710;
   static final int serverPort = 4803;
    static int chunkNo=0;
    static MessageReceiver messageReceiver;
    static MessageSender messageSender = new MessageSender(serverPort);
    static InetAddress inetAddress;
    static String filename = "example.txt";


    static {
        try {
            inetAddress = InetAddress.getByName("localhost");
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }

    static {
        try {
            messageReceiver = new MessageReceiver(clientPort);
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }

    public UDPClient() throws SocketException, UnknownHostException {
    }

    public static void main(String[] args) throws IOException {
        int argcount = args.length;
        int chunksize = 128;


        switch (argcount) {
            case 1:
                chunksize = Integer.parseInt(args[1]);
                break;
            case 2:
                chunksize = Integer.parseInt(args[1]);
                filename = args[2];
                break;
            case 3:
                inetAddress = InetAddress.getByName(args[0]);
                chunksize = Integer.parseInt(args[1]);
                filename = args[2];
                break;
            case 0:
                break;//Im Fall 0 werden die Standardwerte verwendet
            default:
                break;
        }

        if (chunksize > 512){
            chunksize = maxChunkSize;
        }
        messageSender.initTransfer(inetAddress , chunksize, filename);
        while (true) {
            messageReceiver.receivePacket();
            if (messageReceiver.getFlag().equals("ERROR")) {
                switch (messageReceiver.getError()) {
                    case "FNF":
                        System.err.println("File doesn't exist! Bye!");
                        System.exit(1);
                    case "CNF":
                        System.out.println("File transfer completed");
                        System.exit(2);
                    case "NOS":
                        System.out.println("Trying again...");
                        messageSender.initTransfer(inetAddress, chunksize,filename);
                        break;
                }
            }

            if (messageReceiver.getFlag().equals("SIDXX")) {
                System.out.println("Received SID");
                messageSender.requestChunk(inetAddress, messageReceiver.getSessionKey(), messageReceiver.getChunkSize());   //Request First Chunk
                continue;
            }
            if (messageReceiver.getFlag().equals("DATAX")) {

                AppendTextToFile(messageReceiver.getFiledata());

                if(messageReceiver.getActualChunkSize()<chunksize) {
                    System.out.println("File transmitted");
                    System.exit(2);
                }
                messageSender.requestChunk(inetAddress, messageReceiver.getSessionKey(), chunkNo++);   //Request Next Chunk
                continue;
            }
        }
    }

    private static void AppendTextToFile(String filedata) {
        try {
            File f1 = new File(filename);
                if(f1.exists() && messageReceiver.getChunkNumber() == 0){
                    f1.delete();
                    f1 = new File(filename);
                }
                if (!f1.exists()) {
                    f1.createNewFile();
                }
            FileWriter fileWriter = new FileWriter(f1.getName(), true);
            BufferedWriter bw = new BufferedWriter(fileWriter);
            bw.write(filedata);
            bw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
