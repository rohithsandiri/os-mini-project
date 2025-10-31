#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> // Added for socket functions
#include <netinet/in.h> // Added for sockaddr_in

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Connect to localhost
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to Bank Server\n");

    // Main loop: Read from server, print, get input, send to server
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        
        int bytes = read(sock, buffer, sizeof(buffer) - 1);
        
        if (bytes <= 0) {
            break; 
        }
        
        buffer[bytes] = '\0'; 
        printf("%s", buffer); // Print the server's prompt

        if (strstr(buffer, "Invalid login") || strstr(buffer, "deactivated")) {
            break;
        }

        char input[BUFFER_SIZE];
        memset(input, 0, sizeof(input));
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // User pressed Ctrl+D
            break; 
        }

        input[strcspn(input, "\n")] = 0;

        write(sock, input, strlen(input));
        
    }

    printf("\nDisconnected from server.\n");
    close(sock);
    return 0;
}
