
import java.io.*;
import java.util.HashMap;


public class UDPServer {

    static final int clientPort = 4710;
    static final int serverPort =4803;
    static int sid = 0;
    static HashMap<Integer, MessageValues> valuesMap = new HashMap<Integer, MessageValues>();
    private static int actualChunkSize = 0;



    public static void main(String[] args) throws IOException {

        MessageReceiver messageReceiver = new MessageReceiver(serverPort);
        MessageSender messageSender = new MessageSender(clientPort);

        while (true) {
            messageReceiver.receivePacket();

            switch (messageReceiver.getFlag()) {
                case "INITX":
                    sid++;
                    File f = new File(messageReceiver.getFilename());
                    if (f.exists() && !f.isDirectory()) {
                        valuesMap.put(sid, new MessageValues(messageReceiver.getAddress(), messageReceiver.getPort(), f, messageReceiver.getChunkSize()));
                        messageSender.sendSid(valuesMap.get(sid).getConnectionIp(), sid);
                    } else {
                        messageSender.sendError(messageReceiver.getAddress(), "FNF");
                    }
                    break;
                case "ERROR":
                    switch (messageReceiver.getError()) {
                        case "NOS":
                            break;
                        default:
                            break;
                    }
                case "GETXX":
                    int sid = messageReceiver.getSessionKey();
                    if (!valuesMap.containsKey(sid)) {
                        messageSender.sendError(messageReceiver.getAddress(), "NOS");
                    }

                    else {
                        MessageValues mv = valuesMap.get(sid);
                        mv.setChunkNumber(messageReceiver.getChunkNumber());

                        byte[] data= fileFunction(mv.getFile(),mv.getChunkNumber(), mv.getChunkSize());
                        if(mv.isCnfFlag()){
                            messageSender.sendError(mv.getConnectionIp(),"CNF");
                        }
                   else {
                            messageSender.sendData(mv.getConnectionIp(), mv.getChunkNumber(), actualChunkSize, data);
                        }
                        if(data.length < mv.getChunkSize()){
                            mv.setCnfFlag(true);
                        }
                    }
                    break;
                default:
                    break;
            }

        }
    }

    static private byte[] fileFunction(File file, int chunkNumber, int chunkSize) throws IOException {

        byte[] data = new byte[chunkSize];

        InputStream in = new FileInputStream(file);
        in.skip(chunkNumber*chunkSize);
         actualChunkSize = in.readNBytes(data,0,chunkSize);
            if(actualChunkSize < chunkSize) {
                byte[] temp = new byte[actualChunkSize];
                for(int i =0 ; i < actualChunkSize; i++ ){
                temp[i] = data[i];
                }
                in.close();
                return temp;
            }


        in.close();
        return data;
    }

}