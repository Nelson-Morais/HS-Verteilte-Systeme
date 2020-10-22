
import java.io.File;
import java.net.InetAddress;

public class MessageValues {

    private int chunkSize;
    private int chunkNumber;
    private InetAddress connectionIp;
    private File file;
    private int port;
    private boolean cnfFlag;


    MessageValues(InetAddress connectionIp, int port, File file, int chunkSize){
        this.connectionIp = connectionIp;
        this.port = port;
        this.file = file;
        this.chunkSize = chunkSize;
        chunkNumber = 0;
        cnfFlag = false;

    }

    public void setChunkNumber(int chunkNumber) {
        this.chunkNumber = chunkNumber;
    }

    public int getChunkNumber() {
        return chunkNumber;
    }

    public int getChunkSize() {
        return chunkSize;
    }

    public int getPort() {
        return port;
    }

    public InetAddress getConnectionIp() {
        return connectionIp;
    }

    public File getFile() {
        return file;
    }

    public void setCnfFlag(boolean cnfFlag) {
        this.cnfFlag = cnfFlag;
    }
    public boolean isCnfFlag() {
        return cnfFlag;
    }

}
