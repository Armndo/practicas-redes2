import socket

def server(host: str = "127.0.0.1", port: int = 5555):
  # creating socket
  server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  # blocking socket
  server_socket.setblocking(True)
  # binding socket to host:port
  server_socket.bind((host, port))
  # listening
  server_socket.listen(1)
  print(f"Server listening on {host}:{port}")
  # waiting to accept a connection
  conn, (o_host, o_port) = server_socket.accept()
  print(f"Accepted connection from {o_host}:{o_port}")
  # blocking connection sokcet
  conn.setblocking(True)
  # receiving message
  message = conn.recv(1024).decode()
  # show message
  print(f"Received message: '{message}'")
  # closing connection and socket
  conn.close()
  server_socket.close()

server()