
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Arrays;

public class MessageSender {
int port;

    public MessageSender(int port) {
        this.port = port;
    }


    void sendMessage(String s,InetAddress inetAddress) throws IOException {
        byte[] raw = s.getBytes();
        DatagramPacket datagramPacket = new DatagramPacket(raw, raw.length, inetAddress, port);
        System.out.println(port);
        DatagramSocket datagramSocket = new DatagramSocket();
        datagramSocket.send(datagramPacket);
    }
    void requestChunk(InetAddress inetAddress,int sid, int chunkNumber) throws IOException {
    String transferMessage = "HSOSSTP_GETXX;"+sid+";"+chunkNumber+"";
        System.out.println("Requesting chunk number "+chunkNumber+" from server.");
        sendMessage(transferMessage,inetAddress);
    }

     void initTransfer(InetAddress inetAddress, int chunksize, String filename) throws IOException {
        String strInit = "HSOSSTP_INITX;"+ chunksize + ";" + filename + "";
        System.out.printf("Requesting File: %s\n", filename);
        sendMessage(strInit,inetAddress);

    }

    void sendSid(InetAddress inetAddress, int sid) throws IOException {
        String transferMessage = "HSOSSTP_SIDXX;"+sid+"";
        System.out.println("Sending SID to client.");
        sendMessage(transferMessage,inetAddress);
    }

    void sendError(InetAddress inetAddress, String error) throws IOException {
        String transferMessage = "HSOSSTP_ERROR;"+error;
        System.out.println("Sending error to client.");
        sendMessage(transferMessage,inetAddress);
    }

    void sendData(InetAddress inetAddress, int chunkNumber, int chunkSize, byte[] data) throws IOException {

        String dataString = new String(data,0,data.length);
        String transferMessage = "HSOSSTP_DATAX;"+chunkNumber+";"+chunkSize+";"+ dataString +"";
        System.out.println("Sending Data to client.");
        sendMessage(transferMessage,inetAddress);
    }
}
