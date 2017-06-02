// tcp_client.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#define PORT 80

int main(void) {

  int sock;
  int nb_read;

  char hostname[32]="localhost";
  char port[8];
  char path[256]="ballon.txt";
  char request[256] = "GET /";
  char read[25];
  char *pos;
  char rep[3] = "nil";

  FILE* site = NULL;

  struct sockaddr_in sin;
  struct hostent *hostinfo;



  printf("Entrez le port : ");
  fgets(port, 8, stdin);

  /* Correction des fins de lignes */
  if ((pos=strchr(hostname, '\n')) != NULL) *pos = '\0';
  if ((pos=strchr(port, '\n')) != NULL) *pos = '\0';

  /* Définition du pourt de référence */
  if(strlen(port)==0){
     sin.sin_port = htons(PORT);
  }else{
     sin.sin_port = htons(atoi(port));
  }

  /* Configuration de la connexion */
  hostinfo = gethostbyname(hostname);

  sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
  sin.sin_family = AF_INET;


  /* Creation de la socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  /* Tentative de connexion au serveur */
  connect(sock, (struct sockaddr*)&sin, sizeof(sin));
  printf("Connexion a %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));

  /* Demande de rentrer un message */
  printf("Chemin : ");
  fgets(path, 256, stdin);
  if ((pos=strchr(path, '\n')) != NULL) *pos = '\0';

  /* Creation de la requête */
  strcat(request, path);
  strcat(request, " HTTP/1.1\r\n");
  strcat(request, "Host: ");
  strcat(request, hostname);
  strcat(request, "\r\n\r\n");
  printf("REQUETE : \n%s\n*",request);

  /* Envoie du message au serveur */
  send(sock, request, strlen(request),0);

  /* Attent le message du serveur */
  recv(sock, read, sizeof(read)-1, 0);

  /* Verification de la demande*/
  char *tok = strtok(read, " ");
  tok = strtok(NULL, " ");

  printf("CODE DE RETOUR : %s - ", tok);

  if(strcmp(tok, "200") == 0){
      printf("SUCCES\n\n");
  }else if(strcmp(tok, "404") == 0){
     printf("PAGE NOT FOUND\n\n");
  }else{
     printf("ERREUR NON RECONNUE\n\n");
  }

  site = fopen("ballon.txt", "w");

  if(site != NULL){

    /* Option d'affichage dans le terminal */

    while(strcmp(rep, "yes")!=0 && strcmp(rep, "no") != 0){
        printf("Afficher dans le terminal ? (yes/no)\n");
        fgets(rep, 4, stdin);
        if ((pos=strchr(rep, '\n')) != NULL) *pos = '\0';
    }


    nb_read = recv(sock, read, sizeof(read)-1, 0);
    while (nb_read != 0){
      fputs(read, site);
      read[nb_read] = '\0';
      if(strcmp(rep, "yes") == 0){
          printf("%s",read);
      }
      nb_read = recv(sock, read, sizeof(read)-1, 0);
    }
    printf("\n");

    fclose(site);

  close(sock);
  return(EXIT_SUCCESS);


}//main
