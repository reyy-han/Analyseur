#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"Analyseur.h"

void cree_NomFichier(NomFichier** nf, char* nom){
	(*nf)->nom=nom;
	(*nf)->taille=0;
	(*nf)->suiv = NULL;
}

NomFichier* cree_NomFichier2(char* nom, int taille){
	NomFichier* nf = (NomFichier*) malloc(sizeof(NomFichier));
	nf->nom=nom;
	nf->taille=taille;
	nf->suiv = NULL;
	return nf;
}

void ajoute_liste_NomFichier(NomFichier ** nf, char* nv){
	NomFichier *t= (NomFichier*)malloc(sizeof(NomFichier));
	if(t==NULL) {
		printf("erreur allocation\n");
		return;
	}
	t->nom=nv;
	t->taille=0;
	t->suiv=(*nf);
	(*nf) = t;
}

void ajoute_liste_NomFichier2(NomFichier ** nf,NomFichier *n){
	NomFichier *t = n;
	if(t==NULL) {
		printf("erreur allocation\n");
		return;
	}
	t->suiv=(*nf);
	(*nf) = t;
}

int taille_NomFichier(NomFichier** nf){
	NomFichier *t = *nf;
	int taille = 0;
	while(t){
		taille++;
		t=t->suiv;
	}
	return taille;
}

void affiche_liste_NomFichier(NomFichier* nf){
	NomFichier *t= nf;
	while(t){
		t=t->suiv;
	}
}

void detruit_NomFichier(NomFichier** nf){
	NomFichier *t;
	while(*nf!=NULL) {
		t=*nf;
		(*nf)=(*nf)->suiv;
		free(t);
	}
}



int hexa(char c){
	if (c>47 && c<58){ // on utilise le langage ascii pour savoir si ce sont des chiffre décimaux
		return (int)c-48;
	}
	else{
		if (c == 'A' || c == 'a'){
			return 10;
		}
		if (c == 'B' || c == 'b'){
			return 11;
		}
		if (c == 'C' || c == 'c'){
			return 12;
		}
		if (c == 'D' || c == 'd'){
			return 13;
		}
		if (c == 'E' || c == 'e'){
			return 14;
		}
		if (c == 'F' || c == 'f'){
			return 15;
		}
	}
	return -1;
}

int mot_hexa_to_deci(char* mothexa){
	int longueur = strlen(mothexa);
	int res = 0;
	int c;
	int pos = 0;
	for(int i = longueur-1; i >= 0; i--){
		c = hexa(mothexa[i]);
		if(c == -1){
			return -1;
		}
		res = res + (c * pow(16,pos)); 
		pos++;
	}
	return res;
}

int hexa_to_dec(char * hex){
	return hexa(hex[0])*16+hexa(hex[1]);
}

NomFichier* fichier_compatible(char* nomFichier){
	NomFichier* nf = (NomFichier*) malloc(sizeof(NomFichier));
	char* nom = (char*) malloc(sizeof(char) *100);
	FILE*  f;
	FILE* src = fopen(nomFichier,"r");
	char* sep = " ";	/* Separateur */
	char* sepr = "\n";
	int cpt, taille;
	int npb = 0; /* detail*/				
	/* Considerons la taille maximale de la ligne 1000 */
	char* line = (char*) malloc(sizeof(char) * 1000);
	cpt = 0;		/* initialise le compteur de trame */
	taille = 0;		/* initialise le compteur d'octet enregistr'e par trame */
	if(src != NULL){
		while(fgets(line, 1000, src) != NULL){
			/* Si la ligne ne contient qu'un retour chariot et/ou un retour a la ligne on passe a la ligne suivante */
			if(strlen(line) <= 2) continue;
			
			char* motr = strtok(line, sepr); //on enleve le retour à la ligne
			char* mot = strtok(motr, sep);

			if(mot_hexa_to_deci(mot) == -1) continue;
			
			if(mot_hexa_to_deci(mot) == 0) // si premier offset : ouverture d'un nouveau fichier d'ecriture
			{
				printf("Enregistrement de la trame n°:%d\n",cpt);
				/* on considere qu'il n'y a pas de faute dans le nom donné */
				printf("veillez donner un nom à ce nouveau fichier (avec .txt):\n");
				scanf("%s", nom);
				
				if(cpt == 0){
					char* s0 = (char*) malloc(sizeof(char) *100);
					strcpy(s0, nom);
					cree_NomFichier(&nf, s0);
				}
				else{
					/* On ferme le fichier de la trame precedente */
					nf -> taille = taille;
					fclose(f);
					taille = 0;
					char* s = (char*) malloc(sizeof(char) *100);
					strcpy(s, nom);
					ajoute_liste_NomFichier(&nf, s);
				}
				f = fopen(nom, "w+");
				if(f != NULL){
					cpt++;
				}else{
					printf("Impossible d'ouvrir un des fichiers!\n");
					exit(0);
				}
			}
			/*Si verifie la coherence des offsets entre elle(0000, 0010, 0020, etc) */
			if(strlen(mot)>= 2 && mot_hexa_to_deci(mot) == taille)
			{
				mot = strtok(NULL, sep);
				npb = 0;
				while(mot != NULL)
				{
					
					/* 13 : retour chariot et 10 : retour a la ligne */
					if(strlen(mot) == 2 || (mot[2] == 13))
					{
						int c1 = mot[0];
						int c2 = mot[1];
						
						if(hexa(c1) >= 0 && hexa(c1) < 16 && hexa(c2) >= 0 && hexa(c2) < 16 && (npb == 0)){
							int e = ' ';
							taille ++;
							fputc(c1, f);
							fputc(c2, f);
							fputc(e, f);
						}else{
							npb = npb + 1;
						}
					}else{
						npb = npb + 1;
					}
					/* On passe au mot suivant */
					mot = strtok(NULL, sep);
				}
			}else{
				if(mot_hexa_to_deci(mot) > taille){
					printf("Erreur dans la trame vers le nombre hexadecimal n°:%d, il a l'air de manquer %d octets\n", taille, mot_hexa_to_deci(mot)-taille);
					continue;
				}
				if(mot_hexa_to_deci(mot) < taille) continue;
			}
		}
		nf->taille = taille;
		fclose(f);
		fclose(src);
		free(line);
		free(nom);
		return nf;
	}
	else{
		printf("Impossible d'ouvrir un des fichiers!\n");
		exit(0);
	}
}


/* prend un fichier contenant seulement la trame (exemple trame1.txt) */
char** fichier_trame_to_tableau(char* nomFichier, int taille){
	FILE* f = fopen(nomFichier,"r");
	char ** trame = (char**) malloc(sizeof(char*) * taille);
	for(int i=0; i< taille; i++){
		trame[i] = (char*) malloc(sizeof(char*) * 2);
	}
	char* line = (char*) malloc(sizeof(char) * 1000);
	if(f != NULL){
		int i = 0;
		while(fgets(line, 1000, f) != NULL){			
			char d[] = " ";
			/* strtok recupere un tableau de char depuis son premier parametre en le
			separant à chaque fois par le delimiteur du deuxieme parmatre ici un espace
			Attention strtok altere la chaine d'origine (ici line) (ici pas tres grave 				car on utilise plus line apres) */
			char* mot = strtok(line, d);
			while(mot != NULL){
				trame[i] = mot;
				mot = strtok(NULL, d);
				i++;
			}
		}
		fclose(f);	/* il faut fermer le fichier précedement ouvert */
	}else{
		printf("Impossible d'ouvrir le fichier %s\n!", nomFichier);
	}
	return trame;
}


void arp(char ** trame, FILE *fichier, int taille){
	int i=0;
	fprintf(fichier, " Champ ARP");
	
	if(taille < 41){
		printf(">> Erreur ARP: Trame incomplete <<\n");
		fprintf(fichier, " <!> ARP: Trame incomplete !\n");
		return;
	}
	
	if (strcmp(trame[14],"00")==0 && strcmp(trame[15],"01")==0){
		fprintf(fichier, " de type Hardware Ethernet (0x0001) \n");
	}
	else{
		if (strcmp(trame[14],"00")==0 && strcmp(trame[15],"11")==0){
			fprintf(fichier, " de type Hardware HDLC (0x0011) \n");
		}
		else{
			fprintf(fichier, "de type Hardware inconnu pour l'analyseur \n");
		}
	}
	
	fprintf(fichier, "		le type Hardware est de taille %s\n",trame[18]);
	if (strcmp(trame[16],"08")==0 && strcmp(trame[17],"00")==0){
		fprintf(fichier, "	de type Protocol ipv4 (0x0800) \n");
	}
	else{
		fprintf(fichier, "	de type Protocol inconnu pour l'analyseur \n");
	}
	fprintf(fichier, "		le type Protocol est de taille %s\n",trame[19]);
	if (strcmp(trame[20],"00")==0 && strcmp(trame[21],"01")==0){
		fprintf(fichier, "	d'opcode 01 (requête) \n");
	}
	else{
		if (strcmp(trame[20],"00")==0 && strcmp(trame[21],"02")==0){
			fprintf(fichier, "	d'opcode 02 (réponse) \n");
		}
		else{
			fprintf(fichier, "	d'opcode inconnu pour l'analyseur \n");
		}
	}
	fprintf(fichier, "	Emetteur : \n");
	fprintf(fichier, "		Adresse MAC : ");
	for(i=22; i<27; i++){
		fprintf(fichier,"%s:",trame[i]);
	}
	fprintf(fichier, "%s\n",trame[i]);
	fprintf(fichier, "		Adresse IP : ");
	for(i=28; i<31; i++){
		fprintf(fichier,"%d.",hexa_to_dec(trame[i]));
	}
	fprintf(fichier,"%d\n",hexa_to_dec(trame[i]));
	fprintf(fichier, "	Destinataire : \n");
	fprintf(fichier, "		Adresse MAC : ");
	for(i=32; i<37; i++){
		fprintf(fichier,"%s:",trame[i]);
	}
	fprintf(fichier, "%s\n",trame[i]);
	fprintf(fichier, "		Adresse IP : ");
	for(i=38; i<41; i++){
		fprintf(fichier,"%d.",hexa_to_dec(trame[i]));
	}
	fprintf(fichier,"%d\n",hexa_to_dec(trame[i]));
}

void tcp(char **trame,  FILE *fichier, int i, int taille){
	int j, k, l=0;
	char *c;
	
	if(taille < (i + 19)){
		printf(">> Erreur TCP: Trame incomplete <<\n");
		fprintf(fichier, " <!> TCP: Trame incomplete !\n");
		return;
	}
	
	fprintf(fichier, " Champ Transmission Control Protocol (TCP)");
	fprintf(fichier, " Longueur totale : %d octets \n",taille-i);
	j = hexa_to_dec(trame[i])*16*16+ hexa_to_dec(trame[i+1]);
	fprintf(fichier, "	Port source : Ox%s%s (%d)\n", trame[i], trame[i+1],j);
	j = hexa_to_dec(trame[i+2])*16*16+ hexa_to_dec(trame[i+3]);
	fprintf(fichier, "	Port destination : Ox%s%s (%d)\n", trame[i+2], trame[i+3],j);
	fprintf(fichier, "	Numéro de séquence : Ox%s%s%s%s \n", trame[i+4], trame[i+5],trame[i+6], trame[i+7]);
	fprintf(fichier, "	Numéro d'ACK : Ox%s%s%s%s \n", trame[i+8], trame[i+9],trame[i+10], trame[i+11]);
	c=trame[i+12];
	j = hexa(c[0])*4;
	fprintf(fichier, "	Taille de l'entête : %d octets (%c)\n", j, c[0]);
	fprintf(fichier, "	Drapeaux : 0x%c%s\n", c[1], trame[i+13]);
	j = hexa(c[1]);
	c=trame[i+13];
	k = hexa(c[0]);
	fprintf(fichier, "		Reserved : ");
	if (k>7){
		l=8;
	}
	if ((k>3 && k<8)||(k>12)){
		l=l+4;
	}
	fprintf(fichier, "%d ",j+l);
	if(j>7){
		fprintf(fichier,"( 1");
	}
	else{
		fprintf(fichier,"( 0");
	}
	if(((j>4)&&(j<8))||(j>11)){
		fprintf(fichier,"1");
	}
	else{
		fprintf(fichier,"0");
	}
	if(((j>1)&&(j<4))||((j>6)&&(j<8))||((j>9)&&(j<11))||(j>13)){
		fprintf(fichier,"1");
	}
	else{
		fprintf(fichier,"0");
	}
	if(j%2==1){
		fprintf(fichier, "1");
	}
	else{
		fprintf(fichier, "0 ");
	}
	if(k>7){
		fprintf(fichier, " 1");
	}
	else{
		fprintf(fichier, " 0");
	}
	if(((k>3)&&(k<8))||(k>11)){
		fprintf(fichier, "1");
	}
	else{
		fprintf(fichier, "0");
	}
	fprintf(fichier, ".. .... )\n");
	
	fprintf(fichier, "		URG : ");
	if(((k>1)&&(k<4))||((j>6)&&(k<8))||((k>9)&&(k<11))||(k>13)){
		fprintf(fichier,"1 ( .... ..1. .... )\n");
	}
	else{
		fprintf(fichier,"0 ( .... ..0. .... )\n");
	}
	fprintf(fichier, "		ACK : ");
	if(k%2==1){
		fprintf(fichier, "1 ( .... ...1 .... )\n");
	}
	else{
		fprintf(fichier, "0 ( .... ...0 .... )\n");
	}
	j = hexa(c[1]);
	fprintf(fichier, "		PSH : ");
	if(j>7){
		fprintf(fichier, "1 ( .... .... 1... )\n");
	}
	else{
		fprintf(fichier, "0 ( .... .... 0... )\n");
	}
	fprintf(fichier, "		RST : ");
	if(((j>3)&&(j<8))||(j>11)){
		fprintf(fichier, "1 ( .... .... .1.. )\n");
	}
	else{
		fprintf(fichier, "0 ( .... .... .0.. )\n");
	}
	fprintf(fichier, "		SYN : ");
	if(((j>1)&&(j<4))||((j>6)&&(j<8))||((j>9)&&(j<11))||(j>13)){
		fprintf(fichier,"1 ( .... .... ..1. )\n");
	}
	else{
		fprintf(fichier,"0 ( .... .... ..0. )\n");
	}
	fprintf(fichier, "		FIN : ");
	if(j%2==1){
		fprintf(fichier, "1 ( .... .... ...1 )\n");
	}
	else{
		fprintf(fichier, "0 ( .... .... ...0 )\n");
	}
	j = hexa_to_dec(trame[i+14])*16*16+ hexa_to_dec(trame[i+15]);
	fprintf(fichier, "	Fenêtre : 0x%s%s (%d)\n", trame[i+14], trame[i+15],j);
	fprintf(fichier, "	Cheksum : 0x%s%s \n", trame[i+16], trame[i+17]);
	j = hexa_to_dec(trame[i+18])*16*16+ hexa_to_dec(trame[i+19]);
	fprintf(fichier, "	Pointeur de donnée urgente : 0x%s%s (%d)\n", trame[i+18], trame[i+19],j);
	c=trame[i+12];
	l=i+20;
	if(hexa(c[0])>5){
		fprintf(fichier, "	Options et Padding : \n");
		int k= ((hexa(c[0])*4)-20)+l;
		while(l<k){
			j=hexa_to_dec(trame[l]);
			if((j==0)||(j==1)||(j==2)||(j==3)||(j==4)||(j==8)){
				if(j==0){
					fprintf(fichier, "		Option End of List (EOL) \n");
				}
				if(j==1){
					fprintf(fichier, "		Option No Opération (NOP) \n");
				}
				if(j==2){
					fprintf(fichier, "		Option Maximum segment size (MMS) \n");
				}
				if(j==3){
					fprintf(fichier, "		Option Window Scale (WSOPT) \n");
				}
				if(j==4){
					fprintf(fichier, "		Option SACK permitted \n");
				}
				if(j==8){
					fprintf(fichier, "		Option Time Stamp (TS) \n");
				}
			}
			else{
				fprintf(fichier, "		Option inconnu pour l'analyseur \n");
			}
			if((j==0)||(j==1)){
				l++;
			}
			else{
				int m=hexa_to_dec(trame[1+l]);
				int n=2;
				fprintf(fichier, "			Taille : 0x%s  (%d)\n",trame[1+l],m);
				if(n<m){
					fprintf(fichier, "			Valeur : 0x");
					while (n<m){
						fprintf(fichier, "%s",trame[n+l]);
						n++;
					}
					fprintf(fichier, "\n");
				}
				l=l+hexa_to_dec(trame[1+l]);
			}
		}
	}
	fprintf(fichier, "\n");
	k = hexa_to_dec(trame[i])*16*16+ hexa_to_dec(trame[i+1]);
	j = hexa_to_dec(trame[i+2])*16*16+ hexa_to_dec(trame[i+3]);
	if(((k==80)||(k==8080)||(j==80)||(j==8080))&&(taille>l)){
		http(trame, fichier, l, taille);
	}
}


void IPv4(char ** trame, FILE *fichier, int taille){
	int i;
	int j;
	char *c=trame[14];
	
	if(taille < 33){
		printf(">> Erreur IPv4: Trame incomplete <<\n");
		fprintf(fichier, " <!> IPv4: Trame incomplete !\n");
		return;
	}

	if(c[0]!='4'){
		fprintf(fichier, " Champ Internet Protocol (version inconnue pour l'analyseur)\n ");
		fprintf(fichier, " => erreur de correspondance avec le champ Ethernet (version ipv4 attendue  ) \n ");
		return;
	}
	fprintf(fichier, " Champ Internet Protocol (IPv4) ");
	fprintf(fichier, " Longueur totale : 0x%s%s ",trame[16],trame[17] ); 
	i = hexa_to_dec(trame[16])*16*16+ hexa_to_dec(trame[17]);
	fprintf(fichier, "(%d octets)\n", i);
	if(c[1]<53){
		fprintf(fichier, " =>erreur de la taille de l'entête IPv4 (taille supérieure ou égale à 5 attendue) \n");
		return;
	}
	i = hexa(c[1])*4;
	fprintf(fichier, "	Taille de l'entête : %d octets (%c)\n", i, c[1]);
	fprintf(fichier, "	TOS : 0x%s\n", trame[15]);
	i = hexa_to_dec(trame[18])*16*16+ hexa_to_dec(trame[19]);
	fprintf(fichier, "	Identifiant du paquet : Ox%s%s (%d)\n", trame[18], trame[19],i);
	fprintf(fichier, "	Drapeaux : 0x%s\n", trame [20]);
	c=trame[20];
	if(hexa(c[0])%2 == 0){
		j=0;
	}
	else{
		j=1;
	}
	i= hexa(c[0])-j;
	if (i==8 || i==10 || i==12|| i==14){
		fprintf(fichier, "		RB : 1\n"); 
	}
	else{
		fprintf(fichier, "		RB : 0\n"); 
	}
	if (i==4 || i==6 || i==12 || i==14){
		fprintf(fichier, "		DF : 1\n"); 
	}
	else{
		fprintf(fichier, "		DF : 0\n"); 
	}
	if (i==2 || i==6 || i==10 || i==14){
		fprintf(fichier, "		MO : 1\n"); 
	}
	else{
		fprintf(fichier, "		MO : 0\n"); 
	}
	i=(j*16 + hexa(c[1]))*16*16 + hexa_to_dec(trame[21]);
	fprintf(fichier, "	Fragment offset : %d\n",i);
	fprintf(fichier, "	Time To Live (TTL) : %d\n", hexa_to_dec(trame[22]));

	if(strcmp(trame[23],"01")==0){
		fprintf(fichier, "	Protocol : ICMP (0x%s)\n", trame[23]);
	}
	else{
		if(strcmp(trame[23],"11")==0){
			fprintf(fichier, "	Protocol : UDP (0x%s)\n", trame[23]);
		}
		else{
			if(strcmp(trame[23],"06")==0){
				fprintf(fichier, "	Protocol : TCP (0x%s)\n", trame[23]);
			}
			else{
				fprintf(fichier, "	Protocol inconnu pour l'analyseur \n");
			}
		}
	}
	fprintf(fichier, "	Header cheksum :  0x%s%s\n", trame[24], trame[25]);
	fprintf(fichier, "	Source_Adresse IP : ");
	for(i=26; i<29; i++){
		fprintf(fichier,"%d.",hexa_to_dec(trame[i]));
	}
	fprintf(fichier,"%d\n",hexa_to_dec(trame[i]));
	fprintf(fichier, "	Destination_Adresse IP : ");
	for(i=30; i<33; i++){
		fprintf(fichier,"%d.",hexa_to_dec(trame[i]));
	}
	fprintf(fichier,"%d\n",hexa_to_dec(trame[i]));
	
	c=trame[14];
	i=34;
	if(hexa(c[1])>5){
		if(taille < 73){
			printf(">> Erreur Option IPv4: Trame incomplete <<\n");
			fprintf(fichier, " <!> Option IPv4: Trame incomplete !\n");
			return;
		}
		
		fprintf(fichier, "	Options et Padding : \n");
		int k= ((hexa(c[1])*4)-20)+i;
		while(i<k){
			c=trame[i];
			if(hexa(c[0])%2 == 0){
				j=0;
			}
			else{
				j=1;
			}
			j=j*16 + hexa(c[1]);
			
			if((j==7)||(j==1)||(j==68)||(j==131)||(j==137)||(j==0)){
				if(j==7){
					fprintf(fichier, "		Option RR (Record Route) \n");
				}
				if(j==1){
					fprintf(fichier, "		Option NO (No Opération) \n");
				}
				if(j==68){
					fprintf(fichier, "		Option TS (Time Stamp) \n");
				}
				if(j==131){
					fprintf(fichier, "		Option LR (Loose Routing) \n");
				}
				if(j==137){
					fprintf(fichier, "		Option SR (Strict Routing) \n");
				}
				if(j==0){
					fprintf(fichier, "		Option EOL (End of Option List) \n");
				}
			}
			else{
				fprintf(fichier, "		Option inconnu pour l'analyseur \n");
			}
			if(hexa(c[0])>7){
				fprintf(fichier, "			Copy on fragmentation : yes (1... ....)\n");
			}
			else{
				fprintf(fichier, "			Copy on fragmentation : no (0... ....)\n");
			}
			if((hexa(c[0])==0)||(hexa(c[0])==1)||(hexa(c[0])==8)||(hexa(c[0])==9)){
				fprintf(fichier, "			Classe : 0  (.00. ....)\n");
			}
			if((hexa(c[0])==2)||(hexa(c[0])==3)||(hexa(c[0])==10)||(hexa(c[0])==11)){
				fprintf(fichier, "			Classe : 1  (.01. ....)\n");
			}
			if((hexa(c[0])==4)||(hexa(c[0])==5)||(hexa(c[0])==12)||(hexa(c[0])==13)){
				fprintf(fichier, "			Classe : 2  (.10. ....)\n");
			}
			if((hexa(c[0])==6)||(hexa(c[0])==7)||(hexa(c[0])==14)||(hexa(c[0])==15)){
				fprintf(fichier, "			Classe : 3  (.11. ....)\n");
			}
			fprintf(fichier, "			Numéro : %d  (...",j);
			if(j>15){
				fprintf(fichier,"1 ");
			}
			else{
				fprintf(fichier,"0 ");
			}
			if((j>7 && j<16)||(j>23)){
				fprintf(fichier,"1");
			}
			else{
				fprintf(fichier,"0");
			}
			if((j>3 && j<8)||(j>11 && j<16)||(j>27)){
				fprintf(fichier,"1");
			}
			else{
				fprintf(fichier,"0");
			}
			if((j>1 && j<4)||(j>5 && j<8)||(j>9 && j<12)||(j>13 && j<16)||(j>17 && j<20)||(j>29)){
				fprintf(fichier,"1");
			}
			else{
				fprintf(fichier,"0");
			}
			if(j%2 == 0){
				fprintf(fichier,"0)\n");
			}
			else{
				fprintf(fichier,"1)\n");
			}
			if(j==0){
				i++;
			}
			else{
				fprintf(fichier, "			Taille : 0x%s  (%d)\n",trame[1+i],hexa_to_dec(trame[1+i]));
				fprintf(fichier, "			Pointeur : 0x%s  (%d)\n",trame[2+i],hexa_to_dec(trame[2+i]));
				i=i+hexa_to_dec(trame[1+i]);
			}
		}
	}

	
	if(strcmp(trame[23],"06")==0){
		fprintf(fichier, "\n");
		tcp(trame, fichier, i, taille);
	}
}




void analyseur(char ** trame, char * nomFichier, int taille){
	/* on s'intéresse à la couche 2, cad au champ Ethernet */
	int i=0;
	FILE *fichier;
	fichier = fopen(nomFichier,"w");
	
	if(taille < 13){
		printf(">> Erreur Ethernet: Trame incomplete <<\n");
		fprintf(fichier, " <!> Ethernet: Trame incomplete !\n");
		return;
	}

	fprintf(fichier, " Champ Ethernet");
	if (strcmp(trame[12],"08")==0){
		/*seules les types ARP => 08 06 et ipv4 => 08 00 sont acceptés par l'analyseur
		  on ne prend pas en compte les types ipv6, RARP, DEC, XNS, ... */
		if (strcmp(trame[13],"00")==0){
			fprintf(fichier, " de type ipv4 (0x0800) ");
			fprintf(fichier, " Longueur totale : %d octets \n",taille);
			fprintf(fichier, "	Adresse MAC source : ");
			for(i=6; i<11; i++){
				fprintf(fichier,"%s:",trame[i]);
			}
			fprintf(fichier, "%s\n",trame[i]);
			fprintf(fichier, "	Adresse MAC destination : ");
			for(i=0; i<5; i++){
				fprintf(fichier, "%s:",trame[i]);
			}
			fprintf(fichier, "%s\n\n",trame[i]);
			IPv4(trame, fichier, taille);
		}
		else {
			if (strcmp(trame[13],"06")==0){
				fprintf(fichier, " de type ARP (0x0806) \n");
				fprintf(fichier, "	Adresse MAC source : ");
				for(i=6; i<11; i++){
					fprintf(fichier, "%s:",trame[i]);
				}
				fprintf(fichier, "%s\n",trame[i]);
				fprintf(fichier, "	Adresse MAC destination : ");
				for(i=0; i<5; i++){
					fprintf(fichier, "%s:",trame[i]);
				}
				fprintf(fichier, "%s\n\n",trame[i]);
				arp(trame, fichier, taille);
			}
			else {
				fprintf(fichier, " de type ethernet inconnu pour l'analyseur \n");
			}
		}
	}
	else{
		fprintf(fichier, " de type ethernet inconnu pour l'analyseur \n");
	}
	fclose(fichier);
}

char ascii_to_char(char* ascii){
	int tot;
	tot = hexa_to_dec(ascii);
	char c = tot;
	return c;
}

void http(char ** trame, FILE *fichier, int casecourante, int taille){
	int odoa = 0;			/* Correspond au separateur cr lf */
	int i = casecourante;	/* position i debut du protocole HTTP dans la trame */
	int v = 0;				/* si v = 0 alors HTTP si v = 1 alors GET */
	int sl = 0;				/* sl = 0 si status line a ete ecrite completement sl = 1 sinon */
	int j = 0;				/* Va nous permettre de mettre les titres qu'une seule fois */
	int sp = 0;				/* On compte les separateur sp(20 en hexa) dans status line */

	/* On regarde s'il s'agit d'une requete ou d'une reponse */	
	if(strcmp(trame[i],"47")==0 && strcmp(trame[i+1],"45")==0 && strcmp(trame[i+2],"54")==0) {
		v++;
	}
	
	fprintf(fichier, " Champ Hypertext Transfer Protocole (HTTP)");
	if( v == 0 && (strcmp(trame[i],"48")!=0 || strcmp(trame[i+1],"54")!=0 || strcmp(trame[i+2],"54")!=0 || strcmp(trame[i+3],"50")!=0) ){
		fprintf(fichier, " de type HTTP inconnu pour l'analyseur");
		return;
	}
	fprintf(fichier, " Longueur totale : %d octets ",taille-casecourante);
	fprintf(fichier, "\n");
	fprintf(fichier, "	");
	while(odoa != 1){
		if((strcmp(trame[i], "0d") == 0 || strcmp(trame[i], "0D") == 0)
		&& (strcmp(trame[i+1], "0a") == 0 || strcmp(trame[i+1], "0A") == 0)){
			odoa++;
		}
		fprintf(fichier, "%c",ascii_to_char(trame[i]));
		i++;
	}
	/* On remet a zero odoa et i la case courante du debut */
	odoa = 0;
	i =  casecourante;
	while(odoa != 2){
		if((strcmp(trame[i], "0d") == 0 || strcmp(trame[i], "0D") == 0)
		&& (strcmp(trame[i+1], "0a") == 0 || strcmp(trame[i+1], "0A") == 0)){
			fprintf(fichier, "\n");
			odoa++;
			i = i + 2;
			j = 0;
		}
		else{
			odoa = 0;
			if(sl == 0 && strcmp(trame[i],"20")==0){
				fprintf(fichier, "\n");
				j = 0;
				sp++;
				}
			if(sp == 0 && j == 0) {
				if(v == 1)fprintf(fichier, "		Request Method: ");
				if(v == 0)fprintf(fichier, "		Response Version: ");
				j++;
			}if(sp == 1 && j == 0){
				if(v == 1)fprintf(fichier, "		Request URI: ");
				if(v == 0)fprintf(fichier, "		Status Code: ");
				j++;
			}if(sp == 2 && j == 0){
				if(v == 1)fprintf(fichier, "		Request Version: ");
				if(v == 0)fprintf(fichier, "		Response Phrase: ");
				j++;
				sp++;
				sl++;
			}
			if(sl != 0 && j == 0){
				fprintf(fichier, "\t");
				j ++;
			}
			fprintf(fichier, "%c",ascii_to_char(trame[i]));
			i++;
		}
	}
}

/* Le main doit etre lancé avec pour parametre le nom du fichier contenant la trame
*/
int main(int argc, char **argv){
	if(argv[1] == NULL){
		printf("Il faut le nom du fichier!\n");
		exit(1);
	}
	else{
		printf("Analyse de la trame du fichier : %s\n", argv[1]);
		NomFichier* nf = fichier_compatible(argv[1]);
		affiche_liste_NomFichier(nf);
		
		int nbFichier = taille_NomFichier(&nf);
		char *** trames = (char***) malloc(sizeof(char**) * nbFichier);
		int i = 0;
		NomFichier* t = nf;
		while(t){
			trames[i] = fichier_trame_to_tableau(t->nom, t->taille);
			analyseur(trames[i], t->nom, t->taille);
			t = t -> suiv;
			i++;
		}
		NomFichier* t2 = nf;
		char* gedit = "gedit";
		char et = '&';
		while(t2){
			char commande[100];
			sprintf(commande, "%s %s%c", gedit, t2->nom, et);
			system(commande);
			t2 = t2 -> suiv;
		}
		detruit_NomFichier(&nf);
		
	}
	return 0;
}
