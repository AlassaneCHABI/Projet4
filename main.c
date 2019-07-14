//CHABI Alassane

#include <sys/socket.h>
#include <netinet/in.h>
 #include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define GREETING "BONJOUR SERVEUR"

int main(int argc, char *argv[]){
   int cfd; // socket du client
   int success; // etat de la connection
   int port;
   char *element;
   long fileSize;
   FILE* fichier = NULL;
   char *buffer;
   size_t result;
   struct sockaddr_in srv_addr; // socket addr du serveur 

   if(argc!=4){
     printf("USAGE: ./client <serveur_ip_addr> <serveur_port>\n");
     exit(-1);
   }

   cfd = socket(AF_INET, SOCK_STREAM, 0); // création de la socket
   if (cfd < 0){ // Une erreur s'est produite la socket n'a pas pu être créer
      printf("Le SE n'a pas pu créer la socket %d\n", cfd);
      exit(-1);
   }
   // donne une identite a la socket. 
     
   port = atoi(argv[2]);

   srv_addr.sin_family = AF_INET;
   srv_addr.sin_port = htons (port);
   inet_aton(argv[1], (struct in_addr *)&srv_addr.sin_addr.s_addr);

   // connexion au serveur 
     
   success = connect(cfd, (struct sockaddr *) &srv_addr,
                 sizeof(struct sockaddr_in));
   if(success < 0){ 
      printf("Impossible de se connecter au serveur %s:%d error %d\n", 
               argv[1], port, success);
      exit(-1);
   }
   // La connexion au serveur est un succes. r
   
   send(cfd, argv[3], sizeof(argv[1]),0);
   sleep(2);
   fichier = fopen(argv[3],"rb");
   
   // obtain file size:
  fseek (fichier , 0 , SEEK_END);
  fileSize = ftell (fichier);
  rewind (fichier);
  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*fileSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
  // copy the file into the buffer:
  result = fread (buffer,1,fileSize,fichier);
  if (result != fileSize) {
    fputs ("Reading error",stderr);
    exit (3);
   }else{
    send(cfd, buffer, fileSize,0);
   }
  
   // Nous demandons au SE de libérer la socket
      
   fclose(fichier);
   close(cfd);
   return(0);
}

