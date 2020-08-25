#include "httpRequests.h"
#include "strlib.h"

#define YEL "\033[1;33m"
#define RES "\033[1;33m"

int
createSockets(char host[], char *port){
    struct addrinfo hints, *cli;
    int s = 0;
    memSet(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(host, port, &hints, &cli) != 0){
        printf("[!] erro: este hostname: \"%s\", nao pode ser resolvido.\n", host);
        return 1;
    }
    s = socket(cli->ai_family, cli->ai_socktype, cli->ai_protocol);
    if(s == -1){
        printf("[!] erro ao criar socket.\n");
        return 1;
    }
    if(connect(s, cli->ai_addr, cli->ai_addrlen) == -1){
        printf("[!] erro ao tentar conexão com o servidor.\n");
        close(s);
        return 1;
    }
    struct sockaddr_in *addr;
    addr = (struct sockaddr_in *)cli->ai_addr;
    char *ip = inet_ntoa((struct in_addr)addr->sin_addr);
    printf("[*] socket criado com sucesso. %s\n", ip);
    return s;
}

int
sendRequest(char host[], char *port, char urlPath[]){
    int cliente = createSockets(host, port);
    int rLen = 256 + slen(urlPath);
    char request[rLen];

    addstr(request, (char*)"GET ");
    addstr(request, (char*)urlPath);
    addstr(request, (char*)" HTTP/1.1\r\nHost: ");
    addstr(request, (char*)host);
    addstr(request, (char*)"\r\nUser-Agent: Navegador\r\n");
    addstr(request, (char*)"Connection: keep-alive\r\n");
    addstr(request, (char*)"Content-Type: text/html\r\n\r\n");
    send(cliente, request, slen(request), 0);

    /* ----------- */

    char buffer[1024*1024];
    memSet(buffer, 0, sizeof(buffer));
    recv(cliente, buffer, sizeof(buffer),0);
    printf(YEL "[*] Resposta do servidor abaixo:\n\n%s" RES, buffer);
    return 0;
}

int
main(int argc, char **argv){
    if(argc < 4){
        printf("[!] argumentos insuficientes para o programa.\nex: %s example.com 80 /urlpath\n", argv[0]);
        return 1;
    }
    sendRequest(argv[1], argv[2], argv[3]);
    //você pode testar com: www.example.com, 80
    return 0;
}
