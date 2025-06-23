
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;


public class Client {
  private static final String SERVER_ADDRESS = "127.0.0.1";
  private static final int SERVER_PORT = 5555;

  public static void main(String[] args) {
    try (
      Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
      PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
      BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      Scanner scanner = new Scanner(System.in)
    ) {
      // Hilo para recibir mensajes del servidor
      Thread receiveMessages = new Thread(() -> {
        try {
          String message;
          while ((message = in.readLine()) != null) {
            System.out.println("Mensaje recibido: " + message);
          }
        } catch (IOException e) {
          e.printStackTrace();
        }
      });
      receiveMessages.start();
      // Enviar mensajes al servidor
      System.out.println("Conectado al chat. Escribe tu mensaje:");
      while (true) {
        String messageToSend = scanner.nextLine();
        out.println(messageToSend);
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
