#include "http-server.h"
#include <string.h>
#include <stdio.h>

struct Reaction {
    char *user;
    char *message;
};
struct Chat {
    uint32_t id;
    char *user;
    char *message;
    char *timestamp;
    uint32_t num_reactions;
    struct Reaction *reactions;
};

struct Chat all_chats[100000];
size_t chat_count = 0;


char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, response_buff, strlen(response_buff));

}

uint8_t add_chat(char* username, char* message){
    if (chat_count >= 100000) {
        printf("Chat array is full\n");
        return 1;  // Error: Array is full
    }

    struct Chat new_chat;
    new_chat.user = username;
    new_chat.message = message;
    new_chat.id = chat_count;  // Assign current count as ID
    new_chat.num_reactions = 0;
    new_chat.reactions = NULL;

    all_chats[chat_count] = new_chat;  // Add new chat to array
    chat_count++;                      // Increment chat count

    return 0;
}


uint8_t add_reaction(uint32_t id, char* username, char* message){
    if (id >= chat_count || id < 0) {
        return 1;  // Error: Invalid chat ID
    }
    
    struct Reaction new_reaction = {username, message};
    struct Chat *chat = &all_chats[id];
    chat->num_reactions++;
    
    return 0;
}

char* extract_user(char* path){
    const char *userpath = "user=";
    char *start = strstr(path, userpath);

    if (start) {
        start += strlen(userpath); 
        char *end = strchr(start, '&');

        if (end) {
            *end = '\0';
        }
        return start;
    }
    return NULL;
}


char* extract_message(char* path){
    const char *messagepath = "&message=";
    char *start = strstr(path, messagepath);

    if (start) {
        start += strlen(messagepath);
        char *end = strchr(start, '&');

        if (end) {
            *end = '\0';
        }
        return start;
    }
    return NULL;
}



void handle_response(char *request, int client_sock) {

	char path[256];
	
    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }
    else if(strncmp(path, "/chats", strlen("/chats"))){
	//respond_with_chats(client_sock);	
    }

    else if(strncmp(path, "/post", strlen("/post"))){
	char* username = extract_user(path);
	char* message = extract_message(path);
	add_chat(username, message);
	//respond_with_chats(client_sock);
    }

    else if(strncmp(path, "/react", strlen("/react"))){
	char* username = extract_user(path);
	char* message = extract_message(path);

	//extract_id(path);
	//add_reaction(username, message, id);
	////respond_with_chats(client_sock);

    }

    handle_404(client_sock, path);
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    //printf("%s %s\n", username, message);

    start_server(&handle_response, port);
    
    return 0;
}
