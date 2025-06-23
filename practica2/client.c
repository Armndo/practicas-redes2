#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#define PORT 5555
#define BUFFER_SIZE 1024

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char *hello = "Hola desde el cliente";
  char buffer[BUFFER_SIZE] = {0};

  // Crear socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convertir direcciones IPv4 e IPv6 de texto a binario
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    return -1;
  }

  // Conectarse al servidor
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection Failed");
    return -1;
  }

  // Enviar mensaje al servidor
  send(sock, hello, strlen(hello), 0);
  printf("Mensaje enviado\n");

  // Leer respuesta del servidor
  read(sock, buffer, BUFFER_SIZE);
  printf("Respuesta del servidor: %s\n", buffer);

  // Cerrar el socket
  close(sock);

  return 0;
}