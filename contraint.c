#include "contraint.h"
//#include "variable.h"

/************************** GENERER LES CONTRAINTES ***************************************/

/******** CONTRAINTE C1 : POUR CHAQUE CARBONE Si LA SOMME DES (|Qi| + Lij + Ri ) = 1  ************
 ******** TRAITEMENT DE TOUS LES CAS POSSIBLES : UN LIEN, DEUX LIENS , TROIS LIENS ********/ 


void get_contrainte_1(FILE *File, int** data, int nombre_carbone,char Modele){
	int i,j,l;
fprintf(File,"    <constraints>\n    <!-- remplissage des valeurs absolues -->\n"); 

       if(Modele == 'B'){
           fprintf(File,"%s","    <group class=\"valeurs_abs\">\n      <intension>  eq(%0,abs(%1)) </intension>\n");
           for(i = 1 ; i<=nombre_carbone ; i++)
               fprintf(File,"        <args> absq[%d] Q[%d] </args>\n",i-1,i-1);
           fprintf(File,"    </group>\n");
           }


	fprintf(File,"    <!-- contrainte c1:Pour chaque atome de carbone la somme des |Qi|+ Lij + Ri = 1 -->\n\n"); 

   for(i = 1 ; i<=nombre_carbone ; i++){
       fprintf(File,"\n    <sum>\n      <list> ");
       if(Modele == 'B')
          fprintf(File,"absq[%d] ",i-1); 
       fprintf(File,"R[%d]",i-1);
       for(j =1 ;j <= nombre_carbone; j++){
           if(data[i][j] == 1 && i != j ){
               if(i<j)
                   fprintf(File," L%d_%d",i,j);
               else
                   fprintf(File," L%d_%d",j,i);
               }
       }
       fprintf(File,"</list>\n      <coeffs> ");        
       if(Modele == 'B')
           fprintf(File,"1 ");          
       fprintf(File,"1 ");
       for(l =1 ;l <= nombre_carbone; l++){
           if(data[i][l] == 1 && i != l )
               fprintf(File," 1");
       }
   fprintf(File," </coeffs>\n      <condition> (eq,1) </condition>\n    </sum>\n");   

   }
   

}




/*** CONTRAINTE C2 : POUR CHAQUE CARBONE Si LA SOMME DES Ri Rj (SI Si ET Sj SONT ADJACENTS)  <=1 */

void get_contrainte_2(FILE *File, int** data, int nombre_carbone){
	int i,j;
   fprintf(File,"    <!--contrainte c2:Pour chaque atome de carbone i la somme des Rij tq j est un carbone adjacent <=1  -->\n");
   fprintf(File,"    <group class=\"radicalaires\">\n");
   fprintf(File,"%s","        <intension> le(add(%1,%2),%0) </intension>\n");
   for(i = 1 ; i<=nombre_carbone ; i++){
       for(j =1 ;j <= nombre_carbone; j++){
           if(data[i][j] == 1 && i < j )
               fprintf(File,"        <args> 1 R[%d] R[%d] </args>\n",i-1,j-1);
       }
   }

   fprintf(File,"    </group>\n\n"); 
}


/*** CONTRAINTE C3 : LA SOMME DES CHARGES Qi = LA CHARGE INITIAL C */

void get_contrainte_3(FILE *File, int nombre_carbone){
        int i;
	fprintf(File,"    <!--contrainte 3 c3:la somme des charges est egale à la charge Q initial-->\n ");  
	
	fprintf(File,"    <sum>\n      <list> Q[] </list>\n      <coeffs>");
	for (i=0;i< nombre_carbone;i++)
		fprintf(File,"1 ");	
	fprintf(File,"</coeffs>\n      <condition> (eq,C) </condition>\n    </sum>\n");
}



/*** CONTRAINTE C5 : LA SOMME DES ATOMES CHARGÉES = n */

void get_contrainte_5(FILE *File, int nombre_carbone){

        int i;
	fprintf(File,"    <!-- contrainte5 C5 :au plus trois atomes chargées -->\n");
	
	fprintf(File,"    <sum>\n      <list> absq[] </list>\n      <coeffs>");
	for (i=0;i< nombre_carbone;i++)
		fprintf(File,"1 ");	
	fprintf(File,"</coeffs>\n      <condition> (eq,n) </condition>\n    </sum>\n");

}


/*** CONTRAINTE C6 : SI (n >=2) ==>  LA SOMME DES ATOMES CHARGÉES NEGATIVES >0 ET  LA SOMME DES ATOMES CHARGÉES POSITIVES >0*/
/*
void get_contrainte_6(FILE *File, int nombre_carbone){
	int i;
	fprintf(File,"    <!-- contrainte 6 c6:si n >=2 la somme des qi positives > 0 et somme des qi négatives > 0-->\n");
    fprintf(File,"    <group class=\"C6: n>2\">\n");
    fprintf(File,"%s","    <intension> imp(gt(%2,%1),and(gt(add(eq(%3,-1)"); 
    for (i=4;i<nombre_carbone+3;i++)
		    fprintf(File,",eq(%s%d,-1)","%",i);	
	fprintf(File,"%s","),%0),gt(add(eq(%3,1)");
	for (i=4;i<nombre_carbone+3;i++)
		    fprintf(File,",eq(%s%d,1)","%",i);
	fprintf(File,"%s","),%0))) </intension>\n         <args> 0 un n ");
	for (i=0;i<nombre_carbone;i++)
	    fprintf(File,"Q[%d] ",i);
        fprintf(File,"</args>\n");
        fprintf(File,"    </group>\n\n");	    

}

*/
/*** CONTRAINTE C7 : LES ATOMES CHARGÉES NE SONT PAS ADJACENTES ***/

void get_contrainte_7(FILE *File, int** data, int nombre_carbone){
	int i,j;
	fprintf(File,"    <!-- contrainte 7 C7: les atomes chargées ne sont pas adjacents  -->\n");
    fprintf(File,"    <group class=\"c7\">\n");
    fprintf(File,"%s","        <intension> eq(%0,mul(%1,%2)) </intension>\n");
    for(i = 1 ; i<=nombre_carbone ; i++){
       for(j =1 ;j <=nombre_carbone; j++){
           if(data[i][j] == 1 && i < j )
               fprintf(File,"        <args> 0 Q[%d] Q[%d] </args>\n",i-1,j-1);
       }
    }
    fprintf(File,"    </group>\n");

}

void nouvelle_contrainte_1(FILE *File, int **data, int nombre_carbone, int nbr_lien){
	fprintf(File,"    <!-- contrainte new: la somme des Lij >= 1 -->\n");
        int i,j;
	fprintf(File,"    <sum>\n      <list> ");
    for (i=1;i<=nombre_carbone;i++){
		for (j=1;j<=nombre_carbone;j++){
				if (data[i][j]==1 && (j>i))
					fprintf(File,"L%d_%d ",i,j);
		}
	}	
    fprintf(File,"</list>\n      <coeffs>");
	for (i=0;i< nbr_lien;i++)
		fprintf(File,"1 ");	
    fprintf(File,"</coeffs>\n      <condition> (ge,un) </condition>\n    </sum>\n");
	
}

void nouvelle_contrainte_2(FILE *File, int nombre_carbone){
    int i;
    fprintf(File,"    <!-- contrainte new 2:nombre des Ri <= 2 -->\n");

	
	fprintf(File,"    <sum>\n      <list> R[] </list>\n      <coeffs>");
	for (i=0;i< nombre_carbone;i++)
		fprintf(File,"1 ");	
	fprintf(File,"</coeffs>\n      <condition> (le,deux) </condition>\n    </sum>\n");
     
}
