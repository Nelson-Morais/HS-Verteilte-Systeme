
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class MessageReceiver {
    private DatagramSocket socket;
    private InetAddress address;
    private int port;
    private int len;
    private byte[] data = new byte[1024];
    private String[] parsedString;
    private String input;
    private String filedata;

    private String filename;
    private int chunkSize;
    private int chunkNumber;
    private int actualChunkSize;

    private int sessionKey;
    private String flag;
    private String error;

    public MessageReceiver(int port) throws SocketException {
        this.port= port;
        socket = new DatagramSocket(port);
    }


    void receivePacket() throws IOException {

        DatagramPacket packet = new DatagramPacket(data, data.length);
        socket.receive(packet);
        address = packet.getAddress();
        //port = packet.getPort();
        len = packet.getLength();
        data = packet.getData();
        input = new String(data, 0, len);
        System.out.printf("Anfrage von %s mit der Länge %d:%n%s%n", address, len, input);
        parseString();
    }


    void parseString() {

        String temp = input.replaceAll("[<>]","");
        temp = temp.replace("HSOSSTP_","");
        parsedString = temp.split(";");
        flag = parsedString[0];
        switch (flag) {

            case "INITX":
                chunkSize = Integer.parseInt(parsedString[1]);
                filename = parsedString[2];

                break;
            case "ERROR":
                error = parsedString[1];
                break;
            case "SIDXX":
                sessionKey = Integer.parseInt(parsedString[1]);
                break;
            case "GETXX":
                sessionKey = Integer.parseInt(parsedString[1]);
                chunkNumber = Integer.parseInt(parsedString[2]);
                break;
            case "DATAX":
                chunkNumber = Integer.parseInt(parsedString[1]);
                actualChunkSize = Integer.parseInt(parsedString[2]);
                filedata = "";
                for (int i = 3; i < parsedString.length; i++)
                    filedata += parsedString[i];


            default:
                break;

        }

    }


    public String getFlag() {
        return this.flag;
    }

    public String getFilename() {
        return this.filename;
    }

    public String getError() {
        return error;
    }

    public int getSessionKey() {
        return sessionKey;
    }

    public InetAddress getAddress() {
        return address;
    }

    public int getChunkSize() {
        return chunkSize;
    }

    public int getPort(){
        return port;
    }

    public int getChunkNumber() { return chunkNumber;}

    public String getFiledata() {
        return this.filedata;
    }

    public int getActualChunkSize() {return this.actualChunkSize;}

}
