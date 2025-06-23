import java.io.*;
import java.net.*;

public class ServidorMultithread {

  private static final int PORT = 1234; // Puerto en el que el servidor escucha

  public static void main(String[] args) {
    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      System.out.println("Servidor en espera de conexiones...");
      while (true) {
        // Aceptar la conexión del cliente
        Socket clienteSocket = serverSocket.accept();
        System.out.println("Cliente conectado: " + clienteSocket.getInetAddress());
        // Crear un nuevo hilo para manejar al cliente
        ClientHandler handler = new ClientHandler(clienteSocket);
        new Thread(handler).start();
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}

class ClientHandler implements Runnable {
  private final Socket clientSocket;

  public ClientHandler(Socket socket) {
      this.clientSocket = socket;
  }

  // @Override
  // public void run() {
  //   try (BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {
  //     String inputLine;
  //     while ((inputLine = in.readLine()) != null) {
  //       System.out.println("Cliente dice: " + inputLine);
  //     }
  //   } catch (IOException e) {
  //     e.printStackTrace();
  //   }
  // }

  @Override
  public void run() {
    try (
      // Input stream to read from the client
      BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
      // Output stream to write back to the client
      PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
    ) {
      String inputLine;
      while ((inputLine = in.readLine()) != null) {
        System.out.println("Cliente dice: " + inputLine);
        
        // Respond to the client (echo example)
        String response = "Servidor recibió: " + inputLine;
        out.println(response); // Send response back
      }
    } catch (IOException e) {
      e.printStackTrace();
    } finally {
      try {
        clientSocket.close(); // Close the socket when done
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
  }
}