
import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class FTPServer {
  private static final int PORT = 21; // Puerto FTP estándar
  private static final int MAX_THREADS = 10;

  public static void main(String[] args) throws IOException {
    ServerSocket serverSocket = new ServerSocket(PORT);
    ExecutorService threadPool = Executors.newFixedThreadPool(MAX_THREADS);
    System.out.println("Servidor FTP iniciado en el puerto " + PORT);
    while (true) {
      Socket clientSocket = serverSocket.accept();
      threadPool.execute(new FTPHandler(clientSocket));
    }
  }
}

class FTPHandler implements Runnable {
  private Socket clientSocket;

  public FTPHandler(Socket socket) {
    this.clientSocket = socket;
  }

  @Override
  public void run() {
    try {
      InputStream input = clientSocket.getInputStream();
      OutputStream output = clientSocket.getOutputStream();
      BufferedReader reader = new BufferedReader(new InputStreamReader(input));
      PrintWriter writer = new PrintWriter(output, true);
      String command = reader.readLine();
      System.out.println("Comando recibido: " + command);
      if (command.startsWith("PUT")) {
        String fileName = command.split(" ")[1];
        // Confirmación de que el servidor está listo para recibir el archivo
        writer.println("READY");
        // Recibir el archivo
        receiveFile(fileName, input, reader, writer);
      }
      clientSocket.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  private void receiveFile(String fileName, InputStream input, BufferedReader reader, PrintWriter writer) {
    try {
      File file = new File("servidor_" + fileName);
      FileOutputStream fileOutput = new FileOutputStream(file);
      byte[] buffer = new byte[4096];
      int bytesRead;
      // Recibir archivo hasta que se detecte el comando "END"
      while ((bytesRead = input.read(buffer)) != -1) {
          fileOutput.write(buffer, 0, bytesRead);
          // Verificar si el cliente envió "END"
          if (reader.ready() && "END".equals(reader.readLine())) {
              break;
          }
      }
      fileOutput.close();
      System.out.println("Archivo " + fileName + " recibido con éxito.");
      // Enviar respuesta al cliente
      writer.println("Archivo " + fileName + " subido correctamente.");
    } catch (IOException e) {
      e.printStackTrace();
      writer.println("Error al recibir el archivo " + fileName);
    }
  }
}
