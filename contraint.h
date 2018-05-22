#ifndef CONTRAINT_H
#define CONTRAINT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/************************** GENERER LES CONTRAINTES ***************************************/


int **data;



/******** CONTRAINTE C1 : POUR CHAQUE CARBONE Si LA SOMME DES (|Qi| + Lij + Ri ) = 1  ************
 ******** TRAITEMENT DE TOUS LES CAS POSSIBLES : UN LIEN, DEUX LIENS , TROIS LIENS ********/ 

void get_contrainte_1(FILE *File, int** data, int nombre_carbone,char Modele);


/*** CONTRAINTE C2 : POUR CHAQUE CARBONE Si LA SOMME DES Ri Rj (SI Si ET Sj SONT ADJACENTS)  <=1 */

void get_contrainte_2(FILE *File, int** data, int nombre_carbone);


/*** CONTRAINTE C3 : LA SOMME DES CHARGES Qi = LA CHARGE INITIAL C */

void get_contrainte_3(FILE *File, int nombre_carbone);



/*** CONTRAINTE C5 : LA SOMME DES ATOMES CHARGÉES = n */

void get_contrainte_5(FILE *File, int nombre_carbone);


/*** CONTRAINTE C6 : SI (n >=2) ==>  LA SOMME DES ATOMES CHARGÉES NEGATIVES >0 ET  LA SOMME DES ATOMES CHARGÉES POSITIVES >0*/

void get_contrainte_6(FILE *File, int nombre_carbone);


/*** CONTRAINTE C7 : LES ATOMES CHARGÉES NE SONT PAS ADJACENTES ***/

void get_contrainte_7(FILE *File, int** data, int nombre_carbone);

void nouvelle_contrainte_1(FILE *File, int **data, int nombre_carbone, int nbr_lien);

void nouvelle_contrainte_2(FILE *File, int nombre_carbone);

#endif
