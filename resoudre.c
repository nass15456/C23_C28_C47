#include "resoudre.h"
//#include "variable.h"

//appliquer le solveur et ecrire les résultats dans un fichier
int resoudre(int **data, int nombre_carbone, int nbr_lien,char Modele){
	int i,j, nombreSOlutions = 0;
   FILE* fichier3; 
   fichier3 = fopen("sortie", "w");
   system("exe/solver -I vSCPilt -P2G -F xml3 -P ac -PVac ace -S all-sol-mac -SVac ace carbone_new.xcsp >> sortie");
   fclose(fichier3);
   FILE* fichier4;  
   fichier4 = fopen("sortie", "r");
   FILE* fichier5;  
   fichier5 = fopen("solution", "w");   
   fprintf(fichier5,"<structures>\n"); 
   char temp[512];
   int sortieA[nombre_carbone][nombre_carbone];
   int tableRadicalaires[nombre_carbone];
   int tableCharges[nombre_carbone];  

   for(i =0; i<nombre_carbone ;i++){
        tableCharges[i] = 0;
   } 
   while(fgets(temp, 512, fichier4) != NULL) {
        for(i = 1 ;i <= nombre_carbone ; i++){
            for(j = 1 ;j <= nombre_carbone ; j++){
                sortieA[i-1][j-1] = data[i][j];
            }    
        }
        if((strstr(temp, "(")) != NULL) {
            fprintf(fichier5,"  <structure>\n"); 
            nombreSOlutions++;
	    i =0;
	    int negatif = 0 ;
	    int i2 = 0;
	    int nc =0;
	    while(i2 < 512 && temp[i2] != '\0'){
	        if(temp[i2] == 'R'){

	            i2+=2;
	            int indiceActuel = temp[i2] -'0';
	            while(isdigit(temp[i2+1])){
	                indiceActuel=indiceActuel *10 + temp[i2+1] -'0';
	                i2++;
	            }

	            i2+=3;

	            tableRadicalaires[indiceActuel] = temp[i2] -'0';

	        }
                if(Modele == 'B'){
	            if(temp[i2] == 'Q'){
	            
	                i2+=2;
	                int indiceActuel = temp[i2] -'0';
	                while(isdigit(temp[i2+1])){
	                    indiceActuel=indiceActuel *10 +temp[i2+1] -'0';
	                    i2++;
	                }	            

	                i2+=3;
	                if(temp[i2] == '-'){
	                    negatif = 1 ;
	                    i2++;
	                }
	                tableCharges[indiceActuel] = temp[i2] -'0';
	                if(negatif == 1){
	                    tableCharges[indiceActuel] = -tableCharges[indiceActuel] ;
	                    negatif = 0;
	                }
	            
	            }
                }
	        if(temp[i2] == 'C' ){
	            i2+=2;
	            nc = temp[i2] -'0';
	        }
	        
	        if(temp[i2] == 'L'){
	            i2++;
	            int indiceI = temp[i2] -'0';
	            while(isdigit(temp[i2+1])){
	                indiceI= indiceI*10 +temp[i2+1] -'0';
	                i2++;
	            }
	            i2+=2;
	            int indiceJ = temp[i2] -'0';	      	            
	            while(isdigit(temp[i2+1])){
	                indiceJ= indiceJ*10 +temp[i2+1] -'0';
	                i2++;
	            }
	            
                    i2+=2;
                    sortieA[indiceI-1][indiceJ-1] += temp[i2] -'0';
	        }
	        	        
	    i2++; 
	     
		}
		 for(i = 0 ;i <nombre_carbone ; i++){
			  for(j = 0 ;j <nombre_carbone ; j++){
				  if(sortieA[i][j]== 2 ){
					  sortieA[j][i] = 2;
				  }
			  }    
		 }       
		 fprintf(fichier5,"    <molucar_charge> \"%d\" </molucar_charge>\n",nc); 
		 fprintf(fichier5,"    <bonds>\n");
		 for(i = 0 ;i < nombre_carbone ; i++){
			 for(j = 0;j < nombre_carbone ; j++){
				 if(sortieA[i][j] !=0 && j>i)
				 {
					 fprintf(fichier5,"      <bond>\n");   
						 fprintf(fichier5,"        <atom1> \"%d\" </atom1>\n",i+1); 
						 fprintf(fichier5,"        <atom2> \"%d\" </atom2>\n",j+1);                      
							
					 if(sortieA[i][j] ==1)      
						 fprintf(fichier5,"        <type> \"S\" </type>\n");
					 if(sortieA[i][j] ==2)      
						 fprintf(fichier5,"        <type> \"D\" </type>\n");  
					 fprintf(fichier5,"      </bond>\n");                         
				 }
																			
			 }
		 }
		 fprintf(fichier5,"    </bonds>\n");
		 fprintf(fichier5,"    <atomic_charges>\n");  
                 if(Modele == 'B'){
		     for(i = 0 ;i < nombre_carbone ; i++){
			     if(tableCharges[i] != 0){
			     fprintf(fichier5,"      <q>\n");       
			     fprintf(fichier5,"        <atom> \"%d\" </atom> \n",i+1);     
			     fprintf(fichier5,"        <val>\"%d\" </val> \n",tableCharges[i]);              
			     fprintf(fichier5,"      </q>\n");            
			    }        
		     }    
                 }       
		 fprintf(fichier5,"    </atomic_charges>\n"); 
		 fprintf(fichier5,"    <radical>\n");          
		 for(i = 0 ;i < nombre_carbone ; i++){
			 if(tableRadicalaires[i] == 1 || tableCharges[i] == -1){
			 fprintf(fichier5,"      <electrons>\n");       
			 fprintf(fichier5,"        <atom> \"%d\" </atom> \n",i+1); 
			 if(tableRadicalaires[i] == 1)    
				 fprintf(fichier5,"        <val>\"%d\" </val> \n",1);    
			 else
				 fprintf(fichier5,"        <val>\"%d\" </val> \n",2);                         
			 fprintf(fichier5,"      </electrons>\n");            
			}        
		 }        
		 fprintf(fichier5,"    </radical>\n");      
		 fprintf(fichier5,"  </structure>\n");
		}

	}
    fprintf(fichier5,"</structures>");
    return nombreSOlutions;
}
