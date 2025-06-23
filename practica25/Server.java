
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements Calculadora {
  public Server() throws RemoteException {
    super(); // Calls UnicastRemoteObject constructor which may throw RemoteException
  }
  public static void main(String[] args) {
    try {
      Server servidor = new Server();
      Registry registry = LocateRegistry.createRegistry(5555);
      registry.rebind("ServicioCalculadora", servidor);
      System.out.println("Servidor de Calculadora en funcionamiento...");
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  @Override
  public int sumar(int a, int b) throws RemoteException {
    return a + b;
  }

  @Override
  public int restar(int a, int b) throws RemoteException {
    return a - b;
  }

  @Override
  public int multiplicar(int a, int b) throws RemoteException {
    return a * b;
  }

  @Override
  public double dividir(int a, int b) throws RemoteException {
    if (b == 0) {
      throw new RemoteException("No se puede dividir entre cero");
    }
    return (double) a / b;
  }
}