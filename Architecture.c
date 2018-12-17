/*
* -------------------------- TP du module Archi1 -------------------------
*
* ATTENTION : un outil de détection de plagiat logiciel sera utilisé lors de la correction,
* vous avez donc tout intérêt à effectuer un travail PERSONNEL
*
* Un mot/registre de 16 bits est représenté par un tableau d'entiers égaux à 0 ou 1.
* Une ALSU est représentée par une structure ALSU, avec registre accumulateur
* et registre d'état.
* Un CPU (très très simplifié) est représenté par une ALSU et quelques registres
* nécessaires pour stocker les résultats intermédiaires.
*
* Certaines fonctions vous sont fournies, d'autres sont à implanter ou à compléter,
* de préférence dans l'ordre où eles sont indiquées.
* Il vous est fortement conseillé de lire attentivement l'ensemble des commentaires.
*
* Parmi les opérations arithmétiques et logiques, seules 4 opérations de base sont
* directement fournies par l'ALSU, les autres doivent être décrites comme des algorithmes
* travaillant à l'aide des opérateurs de base de l'ALSU simplifiée et pouvant utiliser les
* registres du CPU.
*
* La fonction main() vous permet de tester au fur et à mesure les fonctions que vous implantez.
*
* ----------------------------------------------------------------------------------------------
*

* version: 2018-2019
*/
#include <stdio.h>
#include <stdlib.h>

#define NBITS 16
#define LOWBITS 0x0000FFFF
//LOWBITS 0x0000FFFF = 11111111
/////////////////////////////////////////////////////////
// définition de types
/////////////////////////////////////////////////////////

typedef struct {
	int* accu;
	int* flags; // indicateurs Z C O N	
} ALSU;		//z-zero;c-carry（进位）；o-overfloat；N：negative

typedef struct {
	ALSU alsu;
	int* R0;
	int* R1;
	int* R2;
} CPU;

/////////////////////////////////////////////////////////
// fonctions d'initialisation
/////////////////////////////////////////////////////////

/*
* allocation d'un mot entier de 16 bits initialisé à 0
*/
int* word() {
	int* tab;
	int i;
	tab = (int*)malloc(NBITS*sizeof(int));
	for (i = 0; i < NBITS; i++) tab[i] = 0;
	return tab;
}

/*
* Initialisation du mot (mot de 16 bits, codant un entier en Cà2) avec une valeur entière.
*/
void setValue(int* word, int n) {		//word est sortie, et n est nb entière
	int word16 = n & LOWBITS;	//(n=n0---nNB-1 &  LOWBITS=1-----1) ==>  int word16=n0----nNB-1 (est binaire Cà2)
	// à compléter : utiliser la variable word16 et non le paramètre n
	int i = NBITS - 1;	//i=15
	int j;
	while (word16 != 0)
	{
		word[i] = word16 % 2;
		word16 = word16 / 2;
		--i;
	}

	for (j = i; i >= 0; i--){ //i la pos qui a la reste jusqu'à 0(début)
		word[j] = 0;
	}

	for (j = 0; j <= NBITS - 1; j++){	//afficher tous
		printf("%d", word[j]);
	}
}

/*
* instanciation d'un mot de 16 bits initialisé avec la valeur n
*/
int* initWord(int n) {
	int* tab = word();
	setValue(tab, n);
	return tab;
}

/*
* Initialisation du mot (mot de 16 bits) par recopie des bits du mot en paramètre.
*/
void copyValue(int* word, int* src) {		//Aujourd
	// à implanter
	int i;
	for (i = 0; i <= NBITS - 1; i++){
		src[i] = word[i];
	}
}

/*
* instanciation d'un mot de 16 bits initialisé par recopie d'un mot
*/
int* copyWord(int* src) {
	int* tab = word();
	copyValue(tab, src);
	return tab;
}

/*
* initialise l'ALSU
*/
ALSU initALSU() {
	ALSU res;
	res.accu = word();
	res.flags = (int*)malloc(4 * sizeof(int));
	return res;
}

/*
* initialise le CPU
*/
CPU initCPU() {
	CPU res;
	res.alsu = initALSU();
	res.R0 = (int*)malloc(NBITS*sizeof(int));
	res.R1 = (int*)malloc(NBITS*sizeof(int));
	res.R2 = (int*)malloc(NBITS*sizeof(int));
	return res;
}

/////////////////////////////////////////////////////////
// fonctions de lecture
/////////////////////////////////////////////////////////

/*
* Retourne la valeur entière signée représentée par le mot (complément à 2).
*/
int intValue(int* word) {			//Ajourd'hui
	// à implanter

	int res = 0;
	int puiss = 1;
	int i = NBITS - 1;

	while (i >= 0){
		if (word[i] == 1)
			res += puiss;
		puiss = puiss * 2;
		i -= 1;
	}
	return res - (puiss * word[0]);	//La méthode qu'on doit souvenir
}

/*
* Retourne une chaîne de caractères décrivant les 16 bits
*/
char* toString(int* word) {
	int i, j = 0;
	char* s = (char*)malloc((10 + NBITS)*sizeof(char));
	for (i = 15; i >= 0; i--) {
		if (word[i] == 1) s[j] = '1';
		else s[j] = '0';
		j++;
	}
	return s;
}

/*
* Retourne l'écriture des indicateurs associés à l'ALSU.
*/
char* flagsToString(ALSU alsu) {
	char *string = (char*)malloc(10 * sizeof(char));
	sprintf(string, "z%dc%do%dn%d", alsu.flags[0], alsu.flags[1], alsu.flags[2], alsu.flags[3]);
	return string;
}

/*
* affiche à l'écran le contenu d'une ALSU
*/
void printing(ALSU alsu) {			//Aujourd
	// à implanter
	printf_s("Alsu: \n Binaire: alsu.accu: %s \n", toString(alsu.accu));
	printf_s("Chiffre: alsu.accu : %d\n", intValue(alsu.accu));
	printf_s("Alsu.Flags: %s", flagsToString(alsu));
}

/////////////////////////////////////////////////////////
// fonctions de manipulations élémentaires
/////////////////////////////////////////////////////////

/*
* Mise à la valeur b du bit spécifié dans le mot
*/
void set(int* word, int bitIndex, int b) {
	if ((bitIndex > NBITS - 1) || (bitIndex < 0))
		printf("valeur d'index incorrecte\n");
	word[bitIndex] = b;
}

/*
* Retourne la valeur du bit spécifié dans le mot
*/
int get(int* word, int bitIndex) {
	if ((bitIndex > NBITS - 1) || (bitIndex < 0))
		printf_s("valeur d'index incorrecte\n");

	return word[bitIndex];
}

/*
* Positionne l'indicateur Z en fonction de l'état de l'accumulateur
*/
void setZ(ALSU alsu) {		//Z pour zero	
	// à compléter	tp2
	alsu.flags[0] = 1;
	int i = 0;	//
	while ((i < NBITS) && (alsu.flags[0] == 1))	//Quand on rencontre 1 dans Tableau accu, ce n'est pas de nombre ZERO
	{
		if (alsu.accu[i] == 1)
		{
			alsu.flags[0] = 0;
		}
		++i;
	}
}

/////////////////////////////////////////////////////////
// opérateurs de base de l'ALSU
/////////////////////////////////////////////////////////

/*
* Stocke le paramètre dans le registre accumulateur
*/
void pass(ALSU alsu, int* B) {		//Aujourd	2
	// à compléter
	int i;
	for (i = 0; i < NBITS; i++){
		alsu.accu[i] = B[i];
	}


	setZ(alsu);			//Nb est zero?
	alsu.flags[1] = 0;	//C carry
	alsu.flags[2] = 0;	//O Overfloat
	alsu.flags[3] = 0;	//N négative
}

/*
* Effectue un NAND (NON-ET) entre le contenu de l'accumulateur et le paramètre.
*/
void nand(ALSU alsu, int* B) {	//2
	// à compléter
	for (int i = 0; i < NBITS; i++){
		alsu.accu[i] = ~(B[i] & alsu.accu[i]);	//non-et
	}

	setZ(alsu);	//Zero
	alsu.flags[3] = alsu.accu[0];	//Négative ?
}

/*
* Décale le contenu de l'accumulateur de 1 bit vers la droite
*/
void shift(ALSU alsu) {	//Il est certainement positif  1000...01--->01000...0
	// à compléter
	for (int i = NBITS - 1; i > 0; i--)		//15 -> 1
	{
		alsu.accu[i - 1] = alsu.accu[i];
	}
	alsu.accu[0] = 0;	//
}

/*
* module Full Adder : a+b+cin = s + 2 cout
* retourne un tableau contenant s et cout
*/
int* fullAdder(int a, int b, int cin) {	//2
	int* res = (int*)malloc(2 * sizeof(int));
	// à compléter
	//res[0] sort  et res[1] carry
	int c = a + b;
	if (cin == 0)
	{
		if (c < 2)	//a + b = 1 ou 0
		{
			res[0] = 1;//sort
			res[1] = 0;//carry
			return res;
		}
		else //a + b = 2
		{
			res[0] = 0;
			res[1] = 1;
			return res;
		}

	}
	else if (cin == 1)
	{
		if (c == 0)	//a + b = 0
		{
			res[0] = 1;//sort
			res[1] = 0;//carry
			return res;
		}
		else if (c == 1) // a+b == 1
		{
			res[0] = 0;
			res[1] = 1;
			return res;
		}
		else if (c == 2)
		{
			res[0] = 1;
			res[1] = 1;
			return res;
		}
	}

}

/*
* Additionne le paramètre au contenu de l'accumulateur (addition entière Cà2).
* Les indicateurs sont positionnés conformément au résultat de l'opération.
*/
void add(ALSU alsu, int* B) {	//2
	// à compléter
	// 	int cin;
	// 	for (int i = NBITS - 1; i >= 0;i++)
	// 	{
	// 		
	// 		
	// 	}
}

////////////////////////////////////////////////////////////////////
// Opérations logiques :
////////////////////////////////////////////////////////////////////

/*
* Négation.
*/
void not(CPU cpu){
	// à compléter
	int i = NBITS - 1;
	while (cpu.alsu.accu[i] != -1)
	{
		if (cpu.alsu.accu[i] == 1)
			cpu.alsu.accu[i] = 0;
		else if (cpu.alsu.accu[i] == 0)
		{
			cpu.alsu.accu[i] = 1;
		}
		i--;
	}
	setZ(cpu.alsu);
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[1] = cpu.alsu.accu[0];
}

/*
* Et.
*/
void and(CPU cpu, int* B) {
	int i = NBITS - 1;
	while (cpu.alsu.accu[i] != -1){
		if (cpu.alsu.accu[i] & B[i] == 0)
			cpu.alsu.accu[i] = 0
		else
		cpu.alsu.accu[i] = 1
		i--;
	}
	setZ(cpu.alsu);
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[1] = cpu.alsu.accu[0];


}


/*
* Ou.
*/
void or(CPU cpu, int* B) {
	// à compléter
	int i = NBITS - 1;
	while (cpu.alsu.accu[i] != -1){
		if (cpu.alsu.accu[i] | B[i] == 1)
			cpu.alsu.accu[i] = 1;
		else
			cpu.alsu.accu[i] = 0;
	}
	setZ(cpu.alsu);
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[1] = cpu.alsu.accu[0];
}

/*
* Xor.
*/
void xor(CPU cpu, int* B) {
	// à compléter
	int i = NBITS - 1;
	while (cpu.alsu.accu[i] != -1){
		cpu.alsu.accu[i] = cpu.alsu.accu[i] ^ B[i];
		i--;
	}
	setZ(cpu.alsu);
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[2] = 0;
	cpu.alsu.flags[3] = cpu.alsu.accu[0];
}

/*
* Décale le receveur de |n| positions.
* Le décalage s'effectue vers la gauche si n>0 vers la droite dans le cas contraire.
* C'est un décalage logique (pas de report du bit de signe dans les positions
* libérées en cas de décalage à droite).
* Les indicateurs sont positionnés avec le dernier bit "perdu".
*/
void logicalShift(CPU cpu, int n) {
	// à compléter
	int i, j;
	while (n > 16 || n < 1){
		printf("Entrez un nombre entre 1 et 16%d\n", n);
		for (i = 0; i < NBITS; i++){
			cpu.R0[i] = cpu.alsu.accu[i];
		}
		for (i = 0; i < n; i++){
			cpu.alsu.accu[i] = 0;
		}
		for (j = n; j < NBITS; j++){
			cpu.alsu.accu[j] = cpu.R0[j - n];
		}
	}
	setZ(cpu.alsu);
	cpu.alsu.flags[1] = 0;
	cpu.alsu.flags[2] = 0;
	cpu.alsu.flags[3] = 0;

}

/////////////////////////////////////////////////////////
// Opérations arithmétiques entières
/////////////////////////////////////////////////////////

/*
* Opposé.
*/
void opp(CPU cpu) {
	// à compléter
}

/*
* Soustraction.
*/
void sub(CPU cpu, int* B) {
	// à compléter
}

/*
* Multiplication.
*/
void mul(CPU cpu, int* B) {
	// à compléter
}

/////////////////////////////////////////////////////////
// Programme de test
/////////////////////////////////////////////////////////

int main(char *argv[]) {

	// Champs privés

	int* operand;
	ALSU alsu;
	CPU cpu;
	int * res;
	int chosenInt, integer;
	int go_on = 1;

	char* menu =
		"              Programme de test\n\n0  Quitter\n1  setValue(operande,int)\n2  pass(alsu,operande)\n3  printing(alsu)\n4  afficher toString(operande)\n5  afficher intValue(operande)\n6  afficher intValue(accu)\n8  accu=add(accu,operande)\n9  accu=sub(accu,operande)\n10  accu=and(accu,operande)\n11 accu=or(accu,operande)\n12 accu=xor(accu,operande)\n13 accu=not(accu)\n14 accu=opp(accu)\n15 accu=logicalShift(accu,int)\n17 accu=mul(accu,operande)\n\n";

	char* invite = "--> Quel est votre choix  ? ";

	printf_s("%s", menu);

	operand = word();
	cpu = initCPU();
	alsu = cpu.alsu;

	while (go_on == 1) {
		printf("%s", invite);
		scanf("%d", &chosenInt);
		switch (chosenInt) {
		case 0:
			go_on = 0;
			break;
		case 1:
			printf("Entrez un nombre :	\n");
			scanf("%d", &integer);
			setValue(operand, integer);
			break;
		case 2:
			pass(alsu, operand);
			break;
		case 3:
			printing(alsu);
			break;
		case 4:
			printf("%s\n", toString(operand));
			break;
		case 5:
			printf("intValue(operand)=%d\n", intValue(operand));
			break;
		case 6:
			printf("intValue(accu)=%d\n", intValue(alsu.accu));
			break;
		case 8:
			add(alsu, operand);
			printf("apres add(), accu = \n");
			printing(alsu);
			break;
		case 9:
			sub(cpu, operand);
			printf("apres sub(), A = ");
			printing(alsu);
			break;
		case 10:
			and(cpu, operand);
			printf("apres and(), A = ");
			printing(alsu);
			break;
		case 11:
			or(cpu, operand);
			printf("apres or(), A = ");
			printing(alsu);
			break;
		case 12:
			xor(cpu, operand);
			printf("apres xor(), A = ");
			printing(alsu);
			break;
		case 13:
			not(cpu);
			printf("apres not(), A = ");
			printing(alsu);
			break;
		case 14:
			opp(cpu);
			printf("apres opp(), A = ");
			printing(alsu);
			break;
		case 15:
			printf("Entrez un entier :");
			scanf("%d", &integer);
			logicalShift(cpu, integer);
			printf("apres logicalshift(), A = ");
			printing(alsu);
			break;
		case 17:
			mul(cpu, operand);
			printf("apres mul(), A = ");
			printing(alsu);
			break;
		case 18:
			res = fullAdder(1, 1, 1);
			printf_s("%d%d", res[0], res[1]);
			break;
		default:
			printf("Choix inexistant !!!!\n");
			printf("%s", menu);
		}
	}
	printf("Au revoir et a bientot\n");
}



