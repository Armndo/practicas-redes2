
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

public class Server {
  public static void main(String[] args) { 
    int PORT = 5555;

    System.out.println("Servidor de chat iniciado...");
    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      while (true) {
        new ClientHandler(serverSocket.accept()).start();
      }
    } catch (IOException e) {
      e.printStackTrace();
    } 
  }

  private static class ClientHandler extends Thread {
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private ArrayList<PrintWriter> clientWriters;

    public ClientHandler(Socket socket) {
      this.socket = socket;
      this.clientWriters = new ArrayList<>();
    }

    @Override
    public void run() {
      try {
        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        out = new PrintWriter(socket.getOutputStream(), true);
        synchronized (clientWriters) {
          clientWriters.add(out);
        }

        String message;
        while ((message = in.readLine()) != null) {
          System.out.println("Mensaje recibido: " + message);
          synchronized (clientWriters) {
            for (PrintWriter writer : clientWriters) {
              writer.println(message);
            }
          }
        }
      } catch (Exception e) {
        e.printStackTrace();
      }
    }
  }
}

