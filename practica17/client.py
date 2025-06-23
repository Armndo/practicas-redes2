import socket
# Configuración del cliente
HOST = '127.0.0.1' # Reemplaza con la IP de tu servidor Ubuntu
PORT = 5555 # Debe coincidir con el puerto configurado en el servidor
# Conectar al servidor
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
# Recibir el mensaje de bienvenida
print(client_socket.recv(1024).decode('utf-8'))
# Enviar nombre de usuario
username = input("Ingrese su nombre de usuario: ")
client_socket.sendall(username.encode('utf-8') + b"\n")
# Recibir la solicitud de contraseña
print(client_socket.recv(1024).decode('utf-8'))
# Enviar la contraseña
password = input("Ingrese su contraseña: ")
client_socket.sendall(password.encode('utf-8') + b"\n")
# Recibir el resultado de la autenticación
print(client_socket.recv(1024).decode('utf-8'))
client_socket.close()