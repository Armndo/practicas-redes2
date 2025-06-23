#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 5555
#define BUFFER_SIZE 1024

int main() {
  int sockfd;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in servaddr, cliaddr;
  socklen_t len;
  ssize_t n;

  // Crear el socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error al crear el socket");
    exit(EXIT_FAILURE);
  }

  // Inicializar la dirección del servidor
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Enlazar el socket con la dirección del servidor
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Error en el bind");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  len = sizeof(cliaddr);

  while (1) {
    // Recibir datos del cliente (bloqueante)
    n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    // Añadir un terminador de cadena al final del mensaje recibido
    buffer[n] = '\0';
    printf("Cliente : %s\n", buffer);
    // Enviar una respuesta al cliente (bloqueante)
    sendto(sockfd, buffer, n, 0, (struct sockaddr *)&cliaddr, len);
  }

  close(sockfd);
  return 0;
}
