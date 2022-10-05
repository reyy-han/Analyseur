typedef struct NomFichier NomFichier;
struct NomFichier{
	char* nom;
	int taille;
	NomFichier* suiv;
};

void cree_NomFichier(NomFichier** nf, char* nom);

NomFichier* cree_NomFichier2(char* nom, int taille);

void ajoute_liste_NomFichier(NomFichier ** nf, char* nv);

void ajoute_liste_NomFichier2(NomFichier ** nf,NomFichier *n);

void affiche_liste_NomFichier(NomFichier* nf);

void detruit_NomFichier(NomFichier** nf);

int hexa(char c);

/* rend le nombre decimal correspondant au caractere hexadecimal donn'e */
int decimal_hexa(char h);

/* retourne le nombre decimale correspondant au mot hexadecimal donn'e (pour les offsets) */
int mot_hexa_to_deci(char* mothexa);

int hexa_to_dec(char * hex);

/* Prend le nom du fichier donné par le "client" et l'analyse en creant de nouveaux fichiers contenant seulement des trames (le fichier source pouvant contenier plusieurs trames à la suite
Cette fonction retourne une structure NomFichier qui rend une liste chainees de nom de fichiers des trames retenues par la fonction (on y retrouve aussi la taille des trames en fonction du nombre de chiffres hexadecimaux 
On demande au client le nom du fichier que l'on crée à chaque fois (lui fesant confiance pour l a conformité du nom donn'e */
NomFichier* fichier_compatible(char* nomFichier);

/* prend un fichier contenant seulement la trame (exemple trame1.txt) */
char** fichier_trame_to_tableau(char* nomFichier, int taille);

void arp(char ** trame, FILE *fichier, int taille);

void tcp(char **trame,  FILE *fichier, int i, int taille);

void IPv4(char ** trame, FILE *fichier, int taille);

/* Prend le resultat de fichier_trame_to_tableau (char**) et qui commence à analyser la trame case par case (par exemple : char[0] à char[7] correspond à l'@ MAC de la destination, les 8 suivants à celle source, etc), le nom de fichier donn'e en entrée est le fichier de resultat de fin ou l'on affiche les details de la trame */
void analyseur(char ** trame, char * nomFichier, int taille);

char ascii_to_char(char* ascii);

void http(char ** trame, FILE *fichier, int casecourante, int taille);
