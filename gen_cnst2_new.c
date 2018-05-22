#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "contraint.h"
#include "resoudre.h"
//#include "variable.h"

FILE *File;
FILE *File2 = NULL;

char Modele = 'n';

int nombre_carbone =0,nbr_lien=0, Q=0,a;
int **data;

/** RECUPERER LE NOMBRE DE VARIABLE A PARTIR DES DONNÉES */

int get_nombre_var(){
	nombre_carbone =-1 ;
	//recuperer la charge initiale 
	fscanf(File2,"%d",&Q);	
	while (!feof(File2)){
		fscanf(File2,"%d",&a);
		nombre_carbone++;
	}
	nombre_carbone = sqrt(nombre_carbone);
	return nombre_carbone;
}

void prologue(){
	fprintf(File,"<instance format=\"XCSP3\" type=\"CSP\">\n");
	}

/** RECUPERER LA MATRICE D'ADJACENCE DEPUIS LE FICHIER ET LA CHARGER DANS LA MATRICE DATA */ 

int **get_data(){
    int **data, i,j;
    data =malloc(nombre_carbone * sizeof(int*));
    for (i=1; i <= nombre_carbone; i++)
        data[i]=malloc(nombre_carbone * sizeof(int));
	File2 = fopen("data","r");
	fscanf (File2,"%*[^\n]");      	
	for(i=1;i<=nombre_carbone;i++){
		for(j=1;j<=nombre_carbone;j++){
			fscanf(File2,"%d ",&data[i][j]);
			if ((data[i][j]==1)&& i>j)
				nbr_lien++;
		}
	}
	return data;
}	

/************** GENERER LES VARIABLERS ************************************************
 ********** TEL QUE Lij REPRESENTE UN LIEN ENTRE LE CARBONE i ET LE CARBONE j *********
 ********** Ri = 1 SI LE CARBONE EST RADICALAIRE , 0 SINON ****************************
 ********** Qi = {1, 0, 1} QUI REPRESENTE LA CHARGE DE CHAQUE CARBONNE ****************
 ********** C REPRESENTE LA CHARGE TOTAL DE LA MOLECULE *******************************
 ********** n REPRESENTE LE NOMBRE DES ATOMES CHARGÉES {1, 2, 3} *********************/   

void get_variable(int** data){
	int i,j;
	File2 = fopen("data","r");
	fscanf (File2,"%*[^\n]");      		
	fprintf(File,"  <variables>\n");
	for(i=1;i<=nombre_carbone;i++){
		for(j=1;j<=nombre_carbone;j++){
			if ((data[i][j]== 1)&&(j>i))
				fprintf(File,"    <var id=\"L%d_%d\"> 0 1</var>\n",i,j);
		}
	}
	
    fprintf(File,"    <array id=\"R\" size=\"[%d]\"> 0..1</array>\n",nombre_carbone);
    fprintf(File,"    <array id=\"Q\" size=\"[%d]\"> -1..1</array>\n",nombre_carbone);
    fprintf(File,"    <array id=\"absq\" size=\"[%d]\"> 0..1</array>\n",nombre_carbone); 
    fprintf(File,"    <var id=\"n\">0 1 2</var >\n");    
    fprintf(File,"    <var id=\"C\"> %d </var >\n",Q);       
    fprintf(File,"    <var id=\"un\"> %d </var >\n",1); 
    fprintf(File,"    <var id=\"deux\"> %d </var >\n",2);     
    fprintf(File,"  </variables>\n\n");
}







int main(int argc ,char * argv[]){

    if(argc < 3){
        printf("Usage ./gen_cnst3 A file_Matrice_Adjacence or Usage ./gen_cnst3 B file_Matrice_Adjacence \n");
        exit(0);    
    }
    if(argv[1][0] != 'A' && argv[1][0] != 'B'){
        printf("Usage ./gen_cnst3 A file_Matrice_Adjacence or Usage ./gen_cnst3 B file_Matrice_Adjacence \n");
        exit(0);
    }
    Modele = argv[1][0];	
    File = fopen("carbone_new.xcsp","w");    
    File2 = fopen(argv[2],"r");
    prologue();
	
	// recuperer le nombre des variables
    get_nombre_var(nombre_carbone);
	
	// recuperer la matrices d'adjacence
	data=get_data(&nbr_lien);		
	
	// generer les variables
	get_variable(data);
	
	// generer les contraintes
	get_contrainte_1(File, data, nombre_carbone,Modele);
	get_contrainte_2(File, data, nombre_carbone);
	nouvelle_contrainte_2(File, nombre_carbone);
	if(Modele == 'B'){
	get_contrainte_3(File, nombre_carbone);
	get_contrainte_5(File, nombre_carbone);
	//get_contrainte_6(File, nombre_carbone);
	get_contrainte_7(File, data, nombre_carbone);
	nouvelle_contrainte_1(File, data, nombre_carbone, nbr_lien);
        }
	
	    fprintf(File,"  </constraints>\n</instance>\n"); 
	fclose(File);
	fclose(File2);
	int nombreSOlutions = resoudre(data, nombre_carbone, nbr_lien,Modele);
	printf("le nombre de solutions est %d \n",nombreSOlutions);
	//libre_data(&data);
}
