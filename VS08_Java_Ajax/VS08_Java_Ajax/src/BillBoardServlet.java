import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Implementierung des BillBoard-Servers.
 * In dieser Version unterstützt er asynchrone Aufrufe.
 * Damit wird die Implementierung von Long Polling möglich:
 * Anfragen (HTTP GET) werden nicht sofort wie bei zyklischem
 * Polling beantwortet sondern verbleiben so lange im System,
 * bis eine Änderung an den Client gemeldet werden kann.
 *
 * @author heikerli
 */
@WebServlet(asyncSupported = true, urlPatterns = {"/BillBoardServer"})
public class BillBoardServlet extends HttpServlet {
    private final BillBoardHtmlAdapter bb = new BillBoardHtmlAdapter("BillBoardServer");
    Map<String, Long> time= new HashMap<>();

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">

    /**
     * Handles the HTTP <code>GET</code> method.
     *
     * @param request  servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException      if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String caller_ip = request.getRemoteAddr();
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();
        System.out.println("BillBoardServer - GET (" + caller_ip + "): full output");
        String answer ="";
        answer= request.getParameter("rtype");
        if (answer.equals("JSON")) {
            Long oldTime = time.put(caller_ip, System.currentTimeMillis());
                answer = bb.readEntriesJSON(caller_ip, oldTime);
        } else if(answer.equals("newJSON")) {
            answer = bb.readEntriesJSON(caller_ip, null);
        }else {
            answer = bb.readEntries(caller_ip);
        }
        try {
            out.println(answer);
        } finally {
            out.close();
        }
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request  servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException      if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String caller_ip = request.getRemoteAddr();

        JSONObject jsonObjectM;
        String content = "";
        System.out.println("BillBoardServer - POST (" + caller_ip + ")");
        response.setContentType("text/html;charset=UTF-8");
        StringBuilder builder = new StringBuilder();
        BufferedReader buff = request.getReader();
        try {
            String line;
            while ((line = buff.readLine()) != null) {
                builder.append(line).append('\n');
                System.out.println("JSON: " + builder);
            }
        } finally {
            buff.close();
        }

        // create JSON object with value "input" from JSON object
        try {
            jsonObjectM = new JSONObject(builder.toString());
            content = jsonObjectM.getString("input");
        } catch (JSONException ex) {
            System.out.println("Exception: JSONException: doPost()");
        }
        response.getWriter().close();
        bb.createEntry(content, caller_ip);   //dem Billboard einen Eintrag hinzufügen
    }

    /**
     * Handles the HTTP <code>DELETE</code> method.
     *
     * @param request  servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException      if an I/O error occurs
     */
    @Override
    protected void doDelete(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String caller_ip = request.getRemoteAddr();
        System.out.println("BillBoardServer - DELETE (" + caller_ip + ")");

        JSONObject jsonObjectM;
        int id = 0;
        StringBuilder builder = new StringBuilder();
        BufferedReader buff = request.getReader();

        try {
            String line;
            while ((line = buff.readLine()) != null) {
                builder.append(line).append('\n');
                System.out.println("JSON: " + builder);
            }
        } finally {
            buff.close();
        }

        // create JSON object with value "id" from JSON object
        try {
            jsonObjectM = new JSONObject(builder.toString());
            id = Integer.parseInt(jsonObjectM.getString("id"));
        } catch (JSONException ex) {
            System.out.println("Exception: JSONException: doDelete()");
        }

        System.out.println("Deleting ID: " + id);

        bb.deleteEntry(id);   //dem Billboard einen Eintrag hinzufügen
    }

    /**
     * Handles the HTTP <code>PUT</code> method.
     *
     * @param request  servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException      if an I/O error occurs
     */
    @Override
    protected void doPut(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String caller_ip = request.getRemoteAddr();
        System.out.println("BillBoardServer - PUT (" + caller_ip + ")");

        JSONObject jsonObjectM;
        StringBuilder builder = new StringBuilder();
        BufferedReader requestReader = request.getReader();
        String text = "";
        int idx = 0;
        try {
            String line;
            while ((line = requestReader.readLine()) != null) {
                builder.append(line).append('\n');
                System.out.println("JSON: " + builder);
            }
        } finally {
            requestReader.close();
        }
        try {
            jsonObjectM = new JSONObject(builder.toString());
            text = jsonObjectM.getString("text");
            idx = Integer.parseInt(jsonObjectM.getString("id"));
        } catch (JSONException ex) {
            System.out.println("Exception: JSONException: doPut()");
        }
        bb.updateEntry(idx, text, caller_ip);
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "BillBoard Servlet";
    }// </editor-fold>
}
