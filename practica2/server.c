#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/select.h>
#define PORT 5555
#define BUFFER_SIZE 1024

int main() {
  int server_fd, new_socket, max_sd, sd, activity, valread;
  int client_socket[30] = {0};
  int max_clients = 30;
  int addrlen;
  struct sockaddr_in address;
  char buffer[BUFFER_SIZE];
  fd_set readfds;

  // Crear socket del servidor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Configurar el socket del servidor como no bloqueante
  fcntl(server_fd, F_SETFL, O_NONBLOCK);

  // Configurar el tipo de socket
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Adjuntar el socket al puerto 5555
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Escuchar en el socket
  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Escuchando en el puerto %d \n", PORT);

  // Bucle principal para aceptar y manejar conexiones
  while (1) {
    // Limpiar el conjunto de descriptores de socket
    FD_ZERO(&readfds);

    // Añadir el socket del servidor al conjunto de descriptores
    FD_SET(server_fd, &readfds);
    max_sd = server_fd;

    // Añadir los sockets de cliente al conjunto de descriptores
    for (int i = 0; i < max_clients; i++) {
      sd = client_socket[i];

      if (sd > 0)
        FD_SET(sd, &readfds);

      if (sd > max_sd)
        max_sd = sd;
    }

    // Esperar a que ocurra alguna actividad en uno de los sockets
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if (activity < 0) {
      perror("select error");
    }

    // Si hay una actividad en el socket del servidor, es una nueva conexión
    if (FD_ISSET(server_fd, &readfds)) {
      addrlen = sizeof(address);

      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
      }

      printf("Nueva conexión, socket fd es %d, ip es : %s, puerto : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      // Añadir el nuevo socket al array de sockets de cliente
      for (int i = 0; i < max_clients; i++) {
        if (client_socket[i] == 0) {
          client_socket[i] = new_socket;
          printf("Añadiendo a la lista de sockets como %d\n", i);
          break;
        }
      }
    }

    // Manejar IO en otros sockets
    for (int i = 0; i < max_clients; i++) {
      sd = client_socket[i];

      if (FD_ISSET(sd, &readfds)) {
        // Revisar si fue por cierre y leer el mensaje
        if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
          // Alguien se desconectó, obtener detalles e imprimir
          getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
          printf("Host desconectado, ip %s, puerto %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

          // Cerrar el socket y marcarlo como 0 en la lista
          close(sd);
          client_socket[i] = 0;
        } else {
          // Poner terminador de cadena en el buffer y enviar mensaje de vuelta al cliente
          buffer[valread] = '\0';
          send(sd, buffer, strlen(buffer), 0);
        }
      }
    }
  }

  return 0;
}