/*
 * This is sample code generated by rpcgen.
 * hese are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include "string.h"
#include "calcul.h"
CLIENT *clnt;

/** Fonction RPC de recuperation de ballon**/
reponse *getBallon (char* idRobot) {
	reponse *resultat;
	data parametre;
	parametre.arg1 = idRobot;
	resultat = calcul_my_strcat_1 (&parametre, clnt); 
	return resultat;
}

/** Fonction RPC de verification de validité du ballon**/
reponse *verifBallon (char* ballon) {
    reponse *resultat;
    data parametre;
    parametre.arg1 = ballon;
    resultat = valid_but_1 (&parametre, clnt);
    return resultat;
}

void
calcul_1(char *host)
{
	CLIENT *clnt;
	reponse  *result_1;
	reponse  *result_2;
	data  calcul_my_strcat_1_arg;
	data  valid_but_1_arg;
#ifndef	DEBUG
	clnt = clnt_create (host, CALCUL, VERSION_UN, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	result_1 = valid_but_1(&valid_but_1_arg, clnt);
	if (result_1 == (reponse *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = calcul_my_strcat_1(&calcul_my_strcat_1_arg, clnt);
	if (result_2 == (reponse *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
    char idRobot[30];
    char ballon[100];
	char *pos;
    reponse *resultat;

    if (argc < 3) {
        if(argc == 1){
            printf ("Il manque le nom d'hote\n", argv[0]);
        }else if(argc == 2){
            printf ("Il manque la fonction a executer (1 ou 2)\n", argv[0]);
        }
		exit (1);
	}
	host = argv[1];
	clnt = clnt_create (host, CALCUL, VERSION_UN, "tcp");
    if(atoi(argv[2])==1){//Demander un ballon
        printf("Numéro du robot : ");
        fgets(idRobot,30,stdin);
        if ((pos=strchr(idRobot, '\n')) != NULL) *pos = '\0';
        resultat = getBallon(idRobot);
        if(resultat->errno ==-1){
            printf("Ballon indisponible");
        }else{
            printf("Ballon généré = %s\n", resultat->ballon);
        }
    }else if(atoi(argv[2])==2){//Demander si un ballon est valide
        printf("Ballon : ");
        fgets(ballon,100,stdin);
        if ((pos=strchr(ballon, '\n')) != NULL) *pos = '\0';
        resultat = verifBallon(ballon);
        if(resultat->errno == -1){
            printf("Ballon invalide\n");
        }else{
            printf("Ballon valide\n");
        }
    }else{
        printf("La fonction exécutée n'existe pas (1 ou 2)\n");
        exit (1);
    }
	exit (0);
}
