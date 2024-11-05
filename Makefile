all: chat-server
  
chat-server: chat-server.c
	gcc -std=c11 -Wall -Wno-unused-variable -fsanitize=address -g chat-server.c -g http-server.c -o chat-server

clean:
	rm -f chat-server

