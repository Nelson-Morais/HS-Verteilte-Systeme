//
// Created by Nelson Morais on 24.10.20.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <dirent.h>
#include <math.h>


//define HTTP/1.1 Header
#define HTTP_PROTOCOL_V "HTTP/1.1 "
#define HTTP_OK "200 OK\r\n"
#define HTTP_CONTENTLANGUAGE "Content-Language: de\r\n"
#define HTTP_CONTENTTYPETEXTHTML "Content-Type: text/html; charset=utf-8\r\n"
#define HTTP_CONTENTTYPEIMGJPEG "Content-Type: image/jpeg\r\n"
#define HTTP_NOT_FOUND "404 Not Found\r\n"
#define HTTP_SPACE "\r\n\r\n"


//define sizes
#define MAXLINE 1024
#define CHUNKSIZE 100

#define INDEXPAGE "funktioniert.html"

// Vorwaertsdeklarationen
void err_abort(char *str);

void input_parser(char *input, char **out);

void sendMessage(char **parsedStr, int newsockfd);

DIR *dir;
char *SRV_DIR;
int SRV_PORT;

int main(int argc, char *argv[]) {

    //Argumente zuweisung und Prüfung
    if (argc != 3) {
        err_abort(
                "Arguments missing use: - ./startServer <dir> <port> - dir: Home Directory with HTML files - Port: 80/8080/8008");
    }
    //DIR
    dir = opendir(argv[1]);
    if (dir == NULL) {
        err_abort("Directory kann nicht geöffnet werden.");
    } else {
        closedir(dir);
        SRV_DIR = argv[1];
    }

    //PORT
    char *testConversion;
    long convertToInt = strtol(argv[2], &testConversion, 10);
    if (*testConversion != '\0' || convertToInt > INT_MAX) {
        err_abort("Port parameter nicht zulässig.");
    } else {
        SRV_PORT = (int) convertToInt;
    }


    // Deskriptoren, Adresslaenge, Prozess-ID
    int sockfd, newsockfd, pid;

    // Socket Adressen
    //Structs definiert in netinet/in.h
    struct sockaddr_in srv_addr, cli_addr;
    int srv_addrlen = sizeof(srv_addr);
    int cli_addrlen = sizeof(cli_addr);

    // TCP-Socket erzeugen
    // AF_INET = 2	/* IP protocol family.  */
    //SOCK_STREAM = "SOCK_DGRAM = 2",		/* Connectionless, unreliable datagrams of fixed maximum length.  */
    //protocol = 0 /* nur eine art von SOCK_STREAM , daher protocol = 0 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_abort("Kann Stream-Socket nicht oeffnen!");
    }


    // Binden der lokalen Adresse damit Clients uns erreichen
    // struct sockaddr_in erwartet "family", "Ip" und "port"
    // htonl konvertiert long integer zu netzwerk repräsentation. ( zB address ) (32 bits)
    // htons htonl konzept mit short integer (16 bits)
    memset((void *) &srv_addr, '\0', sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRV_PORT);
    if (bind(sockfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
        err_abort("Kann lokale Adresse nicht binden, laeuft fremder Server?");
    }

    // Warteschlange fuer TCP-Socket einrichten
    // Zweiten parameter bestimmt max anzahl in der warteschlange, alle connections über n werden abgelehnt.
    if (listen(sockfd, 5) < 0) {
        err_abort("Fehler beim Listener.");
    }
    for (;;) {

        printf("TCP Echo-Server: bereit ...\n");
        // Verbindung aufbauen
        //erste parameter ist der socket der in listen für die annahme der verbindung vorgesehen wurde.

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &cli_addrlen);
        if (newsockfd < 0) {
            err_abort("Fehler beim Verbindungsaufbau!");
        }

        // fuer jede Verbindung einen Kindprozess erzeugen
        if ((pid = fork()) < 0) {
            err_abort("Fehler beim Erzeugen eines Kindprozesses!");
        } else if (pid == 0) {
            close(sockfd);
            exit(0);
        }

        char in[MAXLINE];
        char *parsedStr[3];
        memset(in, '\0', sizeof(in));
        read(newsockfd, in, sizeof(in));

        input_parser(in, parsedStr);
        //Ignore favicon anfragen für die ausgabe.
        if (strcmp(parsedStr[1], "favicon.ico") != 0) {
            printf("%s", in);
        }
        sendMessage(parsedStr, newsockfd);

        close(newsockfd);
    }
    return 0;
}

//String Parser, returns Array of pointers to char[]
void input_parser(char *input, char **out) {
    char *flag = malloc(sizeof(char) * 7);
    char *dirInServer = malloc(sizeof(char) * MAXLINE);
    char *protocol = malloc(sizeof(char) * 16);

    sscanf(input, "%s %s %s", flag, dirInServer, protocol);

    //Check directory
    dirInServer = strtok(dirInServer, "/");
    if (dirInServer == NULL) {
        dirInServer = malloc(sizeof(INDEXPAGE));
        memset(dirInServer, '\0', strlen(dirInServer));
        strcpy(dirInServer, INDEXPAGE);
    }
    //CheckFlag
    if (strcmp(flag, "POST") == 0 && strcmp(dirInServer, "ServerHTTPPost") == 0) {
        char *s;
        char *zahl1;
        char *zahl2;
        s = strstr(input, "zahl1=");
        zahl1 = strtok(s, "&");
        zahl2 = strtok(NULL, "&");

        zahl1 = strtok(zahl1, "=");
        zahl1 = strtok(NULL, "=");

        zahl2 = strtok(zahl2, "=");
        zahl2 = strtok(NULL, "=");

        out[0] = flag;
        out[1] = zahl1;
        out[2] = zahl2;
    } else {

        out[0] = flag;
        out[1] = dirInServer;
        out[2] = protocol;
    }
}

void sendMessage(char **parsedStr, int newsockfd) {
    //Header 1024 chars MAX - size 1024bytes (char 1byte * 1024)
    char *httpHeader = malloc(sizeof(char) * MAXLINE);
    memset(httpHeader, '\0', sizeof(*httpHeader));
    FILE *file;

    //httpheader Protocol in ausgabe array speichern.
    strcat(httpHeader, HTTP_PROTOCOL_V);

    if (strcmp(parsedStr[0], "GET") == 0) {
        //set working DIR
        chdir(SRV_DIR);
        //ignore Icon requests vorerst.
        if (strcmp(parsedStr[1], "favicon.ico") == 0) {
            return;
        }

        file = fopen(parsedStr[1], "r");

        if (file == NULL) { //response 404 File not found
            //Http Header
            strcat(httpHeader, HTTP_NOT_FOUND);
            strcat(httpHeader, HTTP_CONTENTTYPETEXTHTML);

            write(newsockfd, httpHeader, sizeof(httpHeader) - 1);
            write(newsockfd, HTTP_SPACE, sizeof(HTTP_SPACE) - 1);

            //Filesize ermitteln.
            file = fopen("404.html", "r");
            fseek(file, 0, SEEK_END);
            long int fileSize = ftell(file);

            char data[CHUNKSIZE];
            memset(data, '\0', sizeof(data));
            rewind(file);


            //chunks
            int bool = 1;
            size_t r = 0;
            while (bool) {
                r += fread(data, 1, CHUNKSIZE, file);
                send(newsockfd, data, strlen(data), 0);
                memset(data, '\0', sizeof(data));
                if (r == fileSize) {
                    bool = 0;
                }
            }


        } else { /*response 200 OK*/
            strcat(httpHeader, HTTP_OK);
            strcat(httpHeader, HTTP_CONTENTLANGUAGE);

            //filesize ermitteln
            fseek(file, 0, SEEK_END);
            long int fileSize = ftell(file);

            //file einlesen in bytes
            char data[CHUNKSIZE];
            memset(data, '\0', sizeof(data));
            rewind(file);

            char *delimed;
            delimed = strtok(parsedStr[1], ".");
            delimed = strtok(NULL, ".");
            //pointer auf delimed übergeben
            if (strcmp(delimed, "html") == 0) {
                strcat(httpHeader, HTTP_CONTENTTYPETEXTHTML);
            } else if (strcmp(delimed, "jpg") == 0) {
                strcat(httpHeader, HTTP_CONTENTTYPEIMGJPEG); }

            write(newsockfd, httpHeader, sizeof(httpHeader) - 1);
            write(newsockfd, HTTP_SPACE, sizeof(HTTP_SPACE) - 1);
            printf("%s\n", httpHeader);
            //chunks
            int bool = 1;
            size_t r = 0;
            while (1) {
                r += fread(data, 1, CHUNKSIZE, file);
                if (strcmp(delimed, "html") == 0)
                    send(newsockfd, data, strlen(data), 0);
                else
                    send(newsockfd, data, sizeof(data), 0);
                memset(data, '\0', sizeof(data));
                if (r == fileSize) {
                    break;
                }
            }

        }

    } else if ((strcmp(parsedStr[0], "POST")) == 0) {

        strcat(httpHeader, HTTP_OK);
        strcat(httpHeader, HTTP_CONTENTLANGUAGE);
        strcat(httpHeader, HTTP_CONTENTTYPETEXTHTML);

        char post_answer[MAXLINE];
        char post_part1[] = "<!DOCTYPE html><HTML><BODY><center><h1> Ergebnis: ";
        char post_part2[] = "</h1></center></BODY></HTML>";
        int zahl1;
        int zahl2;

        //parse ints, calculate and revert to char[]
        sscanf(parsedStr[1], "%d", &zahl1);
        sscanf(parsedStr[2], "%d", &zahl2);
        int length = snprintf(NULL, 0, "%d", zahl1 * zahl2);
        char *product = malloc(length + 1);
        sprintf(product, "%d", zahl1 * zahl2);

        strcat(post_answer, post_part1);
        strcat(post_answer, product);
        strcat(post_answer, post_part2);

        file = fopen("post_return.html", "w");
        fwrite(post_answer, strlen(post_answer), 1, file);
        fclose(file);

        //filesize ermitteln
        file = fopen("post_return.html","r");
        fseek(file, 0, SEEK_END);
        long int fileSize = ftell(file);

        char data[CHUNKSIZE];
        memset(data, '\0', sizeof(data));
        rewind(file);

        write(newsockfd, httpHeader, strlen(httpHeader)-1);
        write(newsockfd, HTTP_SPACE, sizeof(HTTP_SPACE)-1);
        printf("%s\n", httpHeader);

        //chunks
        int bool = 1;
        size_t r = 0;
        while (bool) {
            r += fread(data, 1, CHUNKSIZE, file);
            send(newsockfd, data, strlen(data), 0);
            memset(data, '\0', sizeof(data));
            if (r == fileSize) {
                bool = 0;
            }
        }

    }


    fclose(file);

}

/*
Ausgabe von Fehlermeldungen
*/
void err_abort(char *str) {
    fprintf(stderr, " TCP Echo-Server: %s\n", str);
    fflush(stdout);
    fflush(stderr);
    exit(1);
}
