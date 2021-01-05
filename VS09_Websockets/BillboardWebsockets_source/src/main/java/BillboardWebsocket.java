import org.json.JSONException;
import org.json.JSONObject;

import javax.websocket.*;
import javax.websocket.server.ServerEndpoint;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

@ServerEndpoint(value = "/billboard")
public class BillboardWebsocket{
    ArrayList<Session> sessions= new ArrayList<>();
    private final BillBoardHtmlAdapter bb = new BillBoardHtmlAdapter("BillBoardServer");
    @OnOpen
    public void onOpen(Session session){
        System.out.println("Open session: " + session.getId());
        sessions.add(session);
        newBillboard(session);
    }

    @OnClose
    public void onClose(Session session){
        System.out.println("Close session: " + session.getId());
        sessions.remove(session);
    }

    @OnMessage
    public void onMessage(Session session, String message){
        String caller_id = session.getId();
        int id =0;
        if(message.startsWith("*P*")) {
            String input = message.substring(3);
            System.out.println("BillBoardServer - POST (" + input +" from ID: " + caller_id + ")");
            System.out.println("Message: " + input);
            id = bb.createEntry(input, caller_id);
        } else if(message.startsWith("*U*")) {
            String input = message.substring(3);
            System.out.println("BillBoardServer - Update (" + input + " from ID: " + caller_id + ")");
            try {
                JSONObject jsonObjectM = new JSONObject(input);
                String text = jsonObjectM.getString("text");
                id = Integer.parseInt(jsonObjectM.getString("id"));
                bb.updateEntry(id, text, caller_id);
            } catch (JSONException ex) {
                System.out.println("JSONException: Update");
            }
        } else if(message.startsWith("*D*")) {
            String input = message.substring(3);
            id = Integer.parseInt(input);
            System.out.println("BillBoardServer - DELETE (EntryNo. " + id + "from ID: " + caller_id + ")");

            bb.deleteEntry(id);
        } else {
            System.err.println("Message couldn't be evaluated");
        }

        updateBillboard(id);
    }

    @OnError
    public void onError(Session session, Throwable error){
        error.printStackTrace();
    }

    private void updateBillboard(int entryId){
        ArrayList<Session> failedSessions = new ArrayList<>();
            sessions.forEach((session) -> {
                try {
                    session.getBasicRemote().sendText(bb.readEntryJSON(entryId, session.getId()).toString());
                } catch (IOException e) {
                    e.printStackTrace();
                    System.err.println("Session not found. Deleting Session.");
                    failedSessions.add(session);
                }
                System.out.println("Session: " + session.getId() + " updated");
        });
            sessions.removeAll(failedSessions);
    }
    private void newBillboard(Session session) {
        try {
            session.getBasicRemote().sendText(bb.readEntriesJSON(session.getId()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
