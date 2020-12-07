import de.hsos.morais.nelson.*;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.HashMap;

@WebServlet(name = "ClientServlet")
public class ClientServlet extends HttpServlet {

    Registry registry = null;
    ChatServer chatServer;
    ChatProxy chatProxy = null;
    public static ArrayList<String> clients;
    private static HashMap<String, String> sessionUserMap;

    @Override
    public void init() throws ServletException {
        clients = new ArrayList<>();
        sessionUserMap = new HashMap<>();
        try {
            registry = LocateRegistry.getRegistry();
            chatServer = (ChatServer) registry.lookup("ChatServer");
        } catch (NotBoundException | RemoteException ex) {
            ex.printStackTrace();
        }
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        if (request.getParameterValues("name") != null) {
            HttpSession session = request.getSession(false);
            if(sessionUserMap.containsKey(session.getId()))
                error(request,response,"Session already active. Try to open another session with another browser.", false);
            else if (request.getParameterValues("name")[0].isEmpty())
                error(request, response, "no username found", true);
            else if (!clients.contains(request.getParameterValues("name")[0]))
                subscribe(request, response);
            else
                error(request, response, "username already exists", true);
        } else if (request.getParameterValues("message") != null) {
            if (!request.getParameterValues("message")[0].isEmpty())
                send(request, response);
            else
                error(request, response, "message is empty", true);
        } else
            error(request, response, "unknown request!", true);
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        if (request.getSession(false) == null)
            response.sendRedirect("index.jsp");
        else if (request.getParameterValues("logout") != null)
            unsubscribe(request, response);
        else
            refresh(request, response);
    }

    private void send(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        HttpSession session = request.getSession(false); //Open Session
        ArrayList<String> messages = (ArrayList<String>) session.getAttribute("messages");

        String message = request.getParameterValues("message")[0];
        String user = (String) session.getAttribute("name");
        chatProxy.sendMessage(user, message);
        message = user + ": " + message;

        if (messages == null) {
            ArrayList<String> temp = new ArrayList<>();
            temp.add(message);
            session.setAttribute("messages", temp);
        } else {
            session.setAttribute("messages", messages);
        }
        System.out.println("Sending message from " + message);
        refresh(request, response);
    }

    private void error(HttpServletRequest request, HttpServletResponse response, String error, boolean redirect) throws IOException, ServletException {
        try (PrintWriter out = response.getWriter()) {
            out.println("<!DOCTYPE html>");
            out.println("<html>");
            out.println("<head>");
            if(redirect)
                out.println("<meta http-equiv=\"refresh\" content=\"5;\"URL=ClientServlet\">");
            out.println("<title>An Error occurred</title>");
            out.println("</head>");
            out.println("<body style=\"color:red;\">");
            out.println("<h1>An Error occurred: </h1>");
            out.println("<h2>" + error + "</h2>");
            out.println("</body>");
            out.println("</html>");
        } catch (IOException e) {
            System.err.println("Error occured while printing");
        }
        System.err.println(error);
    }

    private void unsubscribe(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        HttpSession session = request.getSession(false);
        String username = (String) session.getAttribute("name");
        if (chatServer.unsubscribeUser(username)) {
            clients.remove(username);
            sessionUserMap.remove(session.getId());
        }
        response.setContentType("text/html;charset=UTF-8");
        try (PrintWriter out = response.getWriter()) {
            out.println("<!DOCTYPE html>");
            out.println("<html>");
            out.println("<meta http-equiv=\"refresh\" content=\"2;\"URL=index.jsp\">");
            out.println("<head>");
            out.println("<title>Unsubscribing</title>");
            out.println("</head>");
            out.println("<body>");
            out.println("<h2>You have been successfully unsubscribed!</h2>");
            out.println("</body>");
            out.println("</html>");
        } catch (IOException e) {
            System.err.println("Error occured while printing");
        }
        System.out.println("Unsubscribing " + username);
    }

    private void subscribe(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        response.setContentType("text/html;charset=UTF-8");
        String username="";
        try {
            HttpSession session = request.getSession(true);
            ClientProxyImpl clientProxy = new ClientProxyImpl();
            ClientProxy handle = (ClientProxy) UnicastRemoteObject.exportObject((Remote) clientProxy, 0);
            username = request.getParameterValues("name")[0];
            session.setAttribute("name", username);
            chatProxy = chatServer.subscribeUser(username, handle);
            session.setAttribute("proxy", handle);
            if(sessionUserMap.putIfAbsent(session.getId(), username)!=null)
                error(request,response,"User already exists", true);
            clients.add(username);
        } catch (RemoteException e) {
            System.err.println("Error occured while subscribing. Please try again.");
            return;
        }
        try (PrintWriter out = response.getWriter()) {
            out.println("<!DOCTYPE html>");
            out.println("<html>");
            out.println("<meta http-equiv=\"refresh\" content=\"2;\"URL=index.html\">");
            out.println("<head>");
            out.println("<title>Subscribing...</title>");
            out.println("</head>");
            out.println("<body>");
            out.println("<h2>Setting up connection</h2>");
            out.println("</body>");
            out.println("</html>");
        } catch (IOException e) {
            System.err.println("Error occured while printing");
        }
        System.out.println("Subscribing " + username);
    }

    private void refresh(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        HttpSession session = request.getSession(false);

        ArrayList<String> messages = new ArrayList<>();
        ArrayList<String> sessionMessages = (ArrayList<String>) session.getAttribute("messages");

        ClientProxy clientProxy = (ClientProxy) session.getAttribute("proxy");
        ArrayList<String> newMessages = clientProxy.getMessages();

        if (newMessages != null) {
            messages.addAll(newMessages);
            clientProxy.clearMessages();
        }

        if (sessionMessages != null) {
            messages.addAll(sessionMessages);
        }

        ArrayList<String> output = new ArrayList<>(messages);

        String username = (String) session.getAttribute("name");
        session.setAttribute("messages", messages);

        response.setContentType("text/html;charset=UTF-8");
        try (PrintWriter out = response.getWriter()) {
            out.println("<!DOCTYPE html>");
            out.println("<html>");
            out.println("<meta http-equiv=\"refresh\" content=\"8;\"URL=ClientServlet\">");

            out.println("<head>");
            out.println("<title>Chat</title>");
            out.println("</head>");

            out.println("<body>");

            out.println("<h1>Logged in as " + username + "</h1>");

            out.println("<form id=\"eingabefeld\" action=\"ClientServlet\" method=\"POST\" >");
            out.println("<input type=\"text\" name=\"message\" size=\"30\" maxlength=\"50\"><br><br>");
            out.println("<input type=submit value=\"Send\"><br><br><br>");
            out.println("</form>");

            out.println("<textarea name=\"chatoutput\" cols=\"50\" rows=\"10\"readonly>");
            output.forEach(out::println);
            out.println("</textarea><br><br>");

            out.println("<form action=\"ClientServlet\" method=\"GET\">");
            out.println("<input style=\"display:none\"type=\"text\"name=\"logout\"value=\"true\">");
            out.println("<input type=submit value=\"Unsubscribe\">");
            out.println("</form>");

            out.println("</body>");
            out.println("</html>");
        } catch (IOException e) {
            System.err.println("Error occured while printing");
        }
    }

}
