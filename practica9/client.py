import socket

# Configuración del grupo de multidifusión y el puerto
group = '224.1.1.1'
port = 5004

# Restricción de 2 saltos en la red
ttl = 2

# Crear el socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)
print("Escribe tus mensajes. Escribe 'salir' para terminar.")

while True:
  # Pedir al usuario que ingrese un mensaje
  message = input("Ingrese el mensaje a enviar: ")

  # Salir del bucle si el usuario escribe 'salir'
  if message.lower() == 'salir':
    break

  # Enviar el mensaje al grupo de multidifusión
  sock.sendto(message.encode('utf-8'), (group, port))

print("Cliente cerrado.")