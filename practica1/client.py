import socket

def client(message: str, host: str = "127.0.0.1", port: int = 5555):
  # creating socket
  client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  # blocking socket
  client_socket.setblocking(True)
  # connecting to server
  client_socket.connect((host, port))
  print(f"Connecting to server @ {host}:{port}")
  # sending message to server
  client_socket.sendall(message.encode())
  print(f"Message '{message}' sent to server")
  # closing socket
  client_socket.close()

client("hola mundo!")