#include "http-server.h"
#include <string.h>
#include <stdio.h>

struct Reaction {
    char *user;
    char *message;
}
struct Chat {
    uint32_t id;
    char *user;
    char *message;
    char *timestamp;
    uint32_t num_reactions;
    Reaction *reactions;
}

struct Chat all_chats[];

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, response_buff, strlen(response_buff));

}


void handle_response(char *request, int client_sock) {
    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }
    else if(strncmp(path, "/chats", strlen("/chats"))){
	//how to print all chats
    }

    else if(strncmp(path, "/post", strlen("/post"))){
	extract_user(path);
	extract_message(path);
	//add_chat(username, message);
    }

    else if(strncmp(path, "/react", strlen("/react"))){
	
    }


    handle_404(client_sock, path);
}

uint8_t add_chat(char* username, char* message){
	id++;
	char buffer[100];
    	time_t now = time(NULL);
    	struct tm *tm_info = localtime(&now);

}


void extract_user(char* path){
	const char *userpath = "user=";
    	char *start = strstr(path, userpath);

    if (start) {
        
        start += strlen(userpath); 
        char *end = strchr(start, '&');

        if (end) {
            // Terminate the username string at the '/'
            *end = '\0';
        }
	username = start;
	
    }
}

void extract_message(char* path){
	const char *messagepath = "&";
    	char *start = strstr(path, messagepath);

    if (start) {
        
        start += strlen(messagepath);
        char *end = strchr(start, '&');

        if (end) {
            // Terminate the username string at the '/'
            *end = '\0';
        }
	message = start;
	
    }
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }
	
    printf("%s %s\n", username, message);

    start_server(&handle_response, port);
    
    return 0;
}
