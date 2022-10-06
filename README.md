# Analyseur de Protocoles Réseaux Offline
*Projet L3 Réseaux*

KAYA Delphine

LEFEVRE Manon

## Fonctionnement de l’analyseur

Pour pouvoir lancer notre Analyseur, il faut récupérer le fichier `.zip` fournit et le décompresser dans le répertoire de votre choix. 
Celui-ci contient notamment le Makefile et le code. Il faut également que les trames que vous souhaitez analyser avec l’analyseur soient contenues dans ce même répertoire,
à vous donc de les rajouter (on vous fournit tout de même un petit nombre de trames pour commencer).

Lorsque vous souhaitez analyser une trame contenue dans un fichier (ici on prendra trame.txt), vous devez :
- ouvrir un terminal,
- vous placer dans le répertoire contenant l’analyseur et le fichier trame.txt (cd Repetoire)
- lancer la commande `make` puis  `./Analyseur Trames/trame.txt`.
L’analyseur va alors lancer l’analyse. 

Lorsque le programme détecte une trame dans votre fichier, il vous demande d’entrer dans le terminal un nom de fichier (attention aux fautes de frappe, 
vous n’avez qu’une seule chance par trame). A la fin de l’analyse, le(s) fichier(s) auquel(s) vous aurez donné un(des) nom(s) plus tôt, s’ouvrira (s’ouvriront) 
automatiquement. Celui-ci contiendra la description de la trame que vous avez fournie. Si votre fichier **trame.txt** contenait plusieurs trames à la suite, 
plusieurs fichiers s’ouvriront avec à chaque fois un nom que vous aurez donné au départ. Ainsi si votre trame contenait des erreurs, 
le fichier contiendra un message d’erreur.

Les fichiers créés par l’analyseur seront accessibles dans le répertoire de l’analyseur.


## Description

Le but du projet était de créer un analyseur de protocoles réseaux offline qui ressemblerait au logiciel Wireshark.
Ainsi une fois notre code implémenté, on peut l'exécuter avec en paramètre un fichier contenant une ou plusieurs trames.
Il récupère donc ce fichier et l'analyse. Pour chaque trame qu'il trouve, il crée un fichier auquel on donne un nom de fichier dans le terminal.
Il analyse les bits contenus dans la trame considérée et écrit dans son fichier associé les informations correspondantes.

Par exemple, en considérant que la trame est correcte (syntaxiquement), il écrit les informations Ethernet de la trame. S'il s'agit d'un protocole ARP, il décrypte ses informations et les écrit dans le fichier texte.
Sinon s'il s'agit d'un protocole IPv4, il liste les informations qu'il recueille et si celui-ci est suivi d'un protocole tcp il finit par le décrire aussi...

Si on prend une trame:
```
0000   22 69 80 80 7f 64 e8 6f 38 b4 b9 79 08 00 45 00   "i...d.o8..y..E.
0010   00 28 51 09 40 00 80 06 44 52 ac 14 0a 02 d8 3a   .(Q.@...DR.....:
0020   d7 23 d5 6d 01 bb e9 08 60 91 24 6e 61 98 50 10   .#.m....`.$na.P.
0030   01 fe a1 98 00 00                                 ......

```
et qu'on lance `./Analyseur Trames/tcp2.txt`, on obtient:
```
 Champ Ethernet de type ipv4 (0x0800) 
	Adresse MAC source : e8:6f:38:b4:b9:79
	Adresse MAC destination : 22:69:80:80:7f:64

 Champ Internet Protocol (IPv4)  Longueur totale : 0x0028 (40 octets)
	Taille de l'entête : 20 octets (5)
	TOS : 0x00
	Identifiant du paquet : Ox5109 (20745)
	Drapeaux : 0x40
		RB : 0
		DF : 1
		MO : 0
	Fragment offset : 0
	Time To Live (TTL) : 128
	Protocol : TCP (0x06)
	Header cheksum :  0x4452
	Source_Adresse IP : 172.20.10.2
	Destination_Adresse IP : 216.58.215.35

 Champ Transmission Control Protocol (TCP)
	Port source : Oxd56d (54637)
	Port destination : Ox01bb (443)
	Numéro de séquence : Oxe9086091 
	Numéro d'ACK : Ox246e6198 
	Taille de l'entête : 20 octets (5)
	Drapeaux : 0x010
		Reserved : 0 ( 000. .... .... )
		NS : 0 ( ...0 .... .... )
		CWR : 0 ( .... 0... .... )
		ECE : 0 ( .... .0.. .... )
		URG : 0 ( .... ..0. .... )
		ASK : 1 ( .... ...1 .... )
		PSH : 0 ( .... .... 0... )
		RST : 0 ( .... .... .0.. )
		SYN : 0 ( .... .... ..0. )
		FIN : 0 ( .... .... ...0 )
	Fenêtre : 0x01fe (510)
	Cheksum : 0xa198 
	Pointeur de donnée urgente : 0x0000 (0)
```
