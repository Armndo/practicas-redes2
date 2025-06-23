#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 5555
#define MAX_CLIENTS 10
#define NUM_SERVERS 3

sem_t semaphore;

void *handle_server(void *arg)
{
  int client_socket = *(int *)arg;
  free(arg);
  sem_wait(&semaphore); // Controlar cuántos servidores procesan solicitudes a la vez
  char buffer[1024];
  int read_size = recv(client_socket, buffer, 1024, 0);
  if (read_size > 0)
  {
    buffer[read_size] = '\0';
    printf("Servidor procesando solicitud: %s\n", buffer);
    char *response = "Solicitud procesada exitosamente";
    send(client_socket, response, strlen(response), 0);
  }
  close(client_socket);
  sem_post(&semaphore); // Liberar un servidor
  pthread_exit(NULL);
}

int main()
{
  int server_socket, client_socket,
      *new_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_size = sizeof(struct sockaddr_in);
  sem_init(&semaphore, 0, NUM_SERVERS);
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    printf("No se pudo crear el socket\n");
    return 1;
  }
  printf("Socket creado\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Error en bind");
    return 1;
  }
  printf("Enlace completado\n");
  listen(server_socket, MAX_CLIENTS);
  printf("Esperando conexiones...\n");
  while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size)))
  {
    printf("Conexión aceptada, distribuyendo al servidor disponible...\n");
    pthread_t server_thread;
    new_sock = malloc(1);
    *new_sock = client_socket;
    if (pthread_create(&server_thread, NULL, handle_server, (void *)new_sock) < 0)
    {
      perror("Error al crear el hilo");
      return 1;
    }
    pthread_detach(server_thread);
  }
  if (client_socket < 0)
  {
    perror("Error en accept");
    return 1;
  }
  sem_destroy(&semaphore);
  return 0;
}