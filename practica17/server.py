import socket
# Configuración del servidor
HOST = '0.0.0.0' # Escuchar en todas las interfaces de red disponibles
PORT = 5555 # Puerto arbitrario que no requiere permisos de superusuario
# Crear el socket del servidor
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)
print(f"Servidor Telnet escuchando en {HOST}:{PORT}...")
while True:
  client_socket, client_address = server_socket.accept()
  print(f"Conexión desde {client_address}")
  client_socket.sendall("Bienvenido al servidor Telnet!\nLogin: ".encode('utf-8'))
  # Recibir nombre de usuario
  username = client_socket.recv(1024).strip().decode('utf-8')
  # Pedir contraseña
  client_socket.sendall("Password: ".encode('utf-8'))
  password = client_socket.recv(1024).strip().decode('utf-8')
  # Autenticación simple
  if username == 'escom' and password == 'cisco':
    client_socket.sendall("\nAutenticación exitosa!\n".encode('utf-8'))
  else:
    client_socket.sendall("\nAutenticación fallida!\n".encode('utf-8'))
  client_socket.close()