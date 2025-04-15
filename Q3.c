#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

char message[100];

void* handle_client(void* arg) {
    int clientSocket = *(int*)arg;
    free(arg);
    printf("Client ID: %s, Served by Thread ID: %lu\n", message, pthread_self());
    send(clientSocket, message, sizeof(message), 0);
    close(clientSocket);
    return NULL;
}

int main() {
    srand(time(NULL));
    int num = rand() % 9;
    if (num > 8)
        strcpy(message, "lover Tenzing 2022ITB064");
    else
        strcpy(message, "hater Tenzing 2022ITB064");

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    inet_pton(AF_INET, "10.24.36.74", &serverAddress.sin_addr);
    bind(mySocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    socklen_t addrlen = sizeof(serverAddress);
    getsockname(mySocket, (struct sockaddr*)&serverAddress, &addrlen);

    printf("%s\n", message);
    printf("Server IP: 10.24.36.74\n");

    listen(mySocket, 100);

    while (1) {
        int* clientSocket = malloc(sizeof(int));
        *clientSocket = accept(mySocket, NULL, NULL);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, clientSocket);
        pthread_detach(tid);
    }
    return 0;
}
