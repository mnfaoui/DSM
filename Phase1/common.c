#include "common_impl.h"

int creer_socket(int prop, int *port_num) 
{
   int fd = 0;
   struct sockaddr_in saddr;

   
   /* fonction de creation et d'attachement */
   /* d'une nouvelle socket */
   fd = socket(AF_INET, prop, IPPROTO_TCP);
    if (fd == -1) {
        printf("Erreur à la création de la socket\n");
        return 1;
}
   //Socket initialisation
   memset(&saddr,0, sizeof(saddr));
   saddr.sin_family = AF_INET;
   saddr.sin_addr.s_addr = htonl(INADDR_ANY);
   saddr.sin_port = htons(atoi(0));
   
   do_bind(fd, saddr); 
   do_listen(fd);
   /* renvoie le numero de descripteur */
   /* et modifie le parametre port_num */
   *port_num = saddr.sin_port;
   return fd;
}

/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */

void do_bind(int fd, struct sockaddr_in saddr){ 
  if ( bind( fd,(struct sockaddr*)&saddr,sizeof(saddr))==-1)
    perror ("bind");
}

void do_listen(int fd){  // L'ecoute se fait pour un maximum de clients
  if (listen(fd, SOMAXCONN) == -1 ) {
    perror( "listen: ");
  }
}

int do_accept(int fd, struct sockaddr_in *caddr){
  struct sockaddr_in caddr2 = *caddr;
  int c_sock;
  socklen_t len = sizeof(caddr2);

  //accept connection from client
  c_sock=accept(fd,(struct sockaddr *) &caddr2, &len );
  caddr->sin_addr=caddr2.sin_addr;
  caddr->sin_port=caddr2.sin_port;
  if (c_sock == -1){
    perror("accept:");
  }

  return c_sock;
}

void do_connect(int fd, struct sockaddr_in sock_host, int size_host){
	if (connect(fd, (struct sockaddr *) & sock_host, size_host) == -1)
			perror("probleme de connect");
}
