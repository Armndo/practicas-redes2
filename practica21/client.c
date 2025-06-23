#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER "127.0.0.1" // Dirección IP del servidor SMTP (MailHog)
#define PORT 1025          // Puerto de MailHog (1025 por defecto)
#define BUFSIZE 1024

void send_smtp_command(int sockfd, const char *cmd, const char *expected_response)
{
  char buffer[BUFSIZE] = {0};
  // Enviar el comando SMTP
  send(sockfd, cmd, strlen(cmd), 0);
  printf("Enviado: %s", cmd);
  // Leer la respuesta del servidor
  recv(sockfd, buffer, BUFSIZE, 0);
  printf("Respuesta: %s", buffer);
  // Verificar si la respuesta es la esperada

  if (strstr(buffer, expected_response) == NULL)
  {
    fprintf(stderr, "Error: no se recibió la respuesta esperada.\n");
    exit(EXIT_FAILURE);
  }
}

int main() {
  int sockfd;
  struct sockaddr_in server_addr;
  
  // Create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Socket creation failed");
      exit(EXIT_FAILURE);
  }
  
  // Configure server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  
  // Convert IP address from text to binary form
  if (inet_pton(AF_INET, SERVER, &server_addr.sin_addr) <= 0) {
      perror("Invalid address/Address not supported");
      exit(EXIT_FAILURE);
  }
  
  // Connect to server
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("Connection failed");
      exit(EXIT_FAILURE);
  }
  
  // printf("Connected to SMTP server at %s:%d\n", SERVER, PORT);
  
  // Read initial server greeting
  char buffer[BUFSIZE];
  ssize_t bytes_received = recv(sockfd, buffer, BUFSIZE - 1, 0);
  if (bytes_received < 0) {
      perror("Error reading initial greeting");
      exit(EXIT_FAILURE);
  }
  buffer[bytes_received] = '\0';
  printf("Server greeting: %s\n", buffer);

  // Enviar los comandos SMTP
  send_smtp_command(sockfd, "HELO localhost\r\n", "250");
  send_smtp_command(sockfd, "MAIL FROM:<test@example.com>\r\n", "250");
  send_smtp_command(sockfd, "RCPT TO:<recipient@example.com>\r\n", "250");
  send_smtp_command(sockfd, "DATA\r\n", "354");
  // Enviar el contenido del correo
  send_smtp_command(sockfd, "Subject: Test Mail\r\n", "");
  send_smtp_command(sockfd, "From: test@example.com\r\n", "");
  send_smtp_command(sockfd, "To: recipient@example.com\r\n\r\n", "");
  send_smtp_command(sockfd, "Este es un mensaje de prueba.\r\n.\r\n", "250");

  return 0;
}