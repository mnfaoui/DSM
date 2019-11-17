#include "common_impl.h"

/* variables globales */
#define MSG_SIZE 1024
/* un tableau gerant les infos d'identification */
/* des processus dsm */
dsm_proc_t *proc_array = NULL; 

/* le nombre de processus effectivement crees */
volatile int num_procs_creat = 0;

void usage(void)
{
  fprintf(stdout,"Usage : dsmexec machine_file executable arg1 arg2 ...\n");
  fflush(stdout);
  exit(EXIT_FAILURE);
}

void sigchld_handler(int sig)
{
   pid_t pid;

    while ((pid = waitpid(-1, NULL, WNOHANG)) != -1) // -1 pour attendre n'importe quel fils
    {
      if (pid > 0)
      {
        num_procs_creat--;
      }
   }
}


int main(int argc, char *argv[])
{
  if (argc < 3){
    usage();
  } else {       
     pid_t pid;
     int num_procs = 0;
     int nb_machine;
     int num_process;
     int i;
     int sock_listen;
     char nom_machine;
     int port_num=0;
     int** stdout_tube;
     int** stderr_tube;
     
     /* Mise en place d'un traitant pour recuperer les fils zombies*/      
     /* XXX.sa_handler = sigchld_handler; */
     struct sigaction action;
     memset(&action, 0, sizeof(action));
     action.sa_handler = &sigchld_handler;
     action.sa_flags = 0;
     sigaction(SIGALRM, &action, NULL);
     
     /* lecture du fichier de machines */
     /* 1- on recupere le nombre de processus a lancer */
     num_process=nb_process(argv[1]);
     /* 2- on recupere les noms des machines : le nom de */
     /* la machine est un des elements d'identification */
     nom_machine=lecture_machine(argv[1]);
     /* creation de la socket d'ecoute */
     sock_listen=creer_socket(SOCK_STREAM,&port_num);
     /* + ecoute effective */ 
     do_listen(sock_listen);
     /* creation des fils */
     for(i = 0; i < num_procs ; i++) {
	
	   /* creation du tube pour rediriger stdout */
      if(pipe(stdout_tube)==-1)
         ERROR_EXIT("Erreur création du tube stdout du fils\n");

	   /* creation du tube pour rediriger stderr */
      if(pipe(stderr_tube)==-1)
         ERROR_EXIT("Erreur création du tube stderr du fils\n");

	   pid = fork();
	   if(pid == -1) ERROR_EXIT("fork");
	
	   if (pid == 0) { /* fils */	
	   
	      /* redirection stdout */	      
	      close(stdout_tube[0]);
         dup2(stdout_tube[1],STDOUT_FILENO);
	      /* redirection stderr */	      	      
	      close(stderr_tube[0]);
         dup2(stderr_tube[1],STDERR_FILENO);
	      /* Creation du tableau d'arguments pour le ssh */ 
         char ** newargv = malloc((argc+4)*sizeof(char *));
         newargv[0]="ssh";
         newargv[1]=nom_machine[i];
         /* jump to new prog : */
	      execvp("ssh",newargv); 

	   } else  if(pid > 0) { /* pere */		      
	      /* fermeture des extremites des tubes non utiles */
	      num_procs_creat++;	      
	   }
     }
     
   
     for(i = 0; i < num_procs ; i++){
	
	/* on accepte les connexions des processus dsm */
	
	/*  On recupere le nom de la machine distante */
	/* 1- d'abord la taille de la chaine */
	/* 2- puis la chaine elle-meme */
	
	/* On recupere le pid du processus distant  */
	
	/* On recupere le numero de port de la socket */
	/* d'ecoute des processus distants */
     }
     
     /* envoi du nombre de processus aux processus dsm*/
     
     /* envoi des rangs aux processus dsm */
     
     /* envoi des infos de connexion aux processus */
     
     /* gestion des E/S : on recupere les caracteres */
     /* sur les tubes de redirection de stdout/stderr */     
     /* while(1)
         {
            je recupere les infos sur les tubes de redirection
            jusqu'à ce qu'ils soient inactifs (ie fermes par les
            processus dsm ecrivains de l'autre cote ...)
       
         };
      */
     
     /* on attend les processus fils */
     
     /* on ferme les descripteurs proprement */
     
     /* on ferme la socket d'ecoute */
  }   
   exit(EXIT_SUCCESS);  
}

