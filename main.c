#include <stdio.h>
#include <string.h>
#include "board.h"
#define MAX 100

/* Structure stockant les données joueur, se composent uniquement de pseudo mais permet de potentiel extension */
typedef struct{
    char name[MAX];
}pseudo;


/*
Vide le Buffer, obligatoire pour la vérification des saisis
*/
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

/* 
Affiche un pion en fonction de sa taille et du joueur qui le possède
Case 4 permet d'afficher le pseudo du joueur selon sa couleur
*/
void color(int joueur,int size,pseudo e[]){
    if(joueur==1){                                                  //Condition prenant en compte le joueur pour definir une couleur
        printf("\033[34;01m");                                      //Definition de la couleur
                switch(size){                                       //Switch affichant la piece selon la taille, case 4 affichage du pseudo, default si il n'y a pas de piece
                    case 1:
                        printf("x");
                        break;
                    case 2:
                        printf("X");
                        break;
                    case 3: 
                        printf("#");
                        break;
                    case 4:
                        printf("%s",e[joueur-1].name);
                            break;
                    default:
                        printf(" ");    
                            break;
                }
            }else{                                                  //Condition prenant en compte le joueur pour definir une couleur
                printf("\033[33;01m");                              //Definition de la couleur
                switch(size){                                       //Switch affichant la piece selon la taille, case 4 affichage du pseudo, default si il n'y a pas de piece
                    case 1: 
                        printf(".");
                        break;
                    case 2:
                        printf("o");
                        break;
                    case 3: 
                        printf("0");
                        break;
                    case 5:
                        printf("%s",e[joueur-1].name);
                            break;
                    default:
                        printf(" ");   
                            break;
                }
            }
            printf("\033[00m");                                      //Definition de la couleur
}

/* 
Affiche les pieces restantes d'un joueur
*/
void gather_piece_remaining(board game,int joueur){
    for(int i=1; i<4;i++ ){
        int piece_remain = get_nb_piece_in_house(game,joueur,i);    //Collecte le nombre de piece restante selon la taille et le joueur
        for(int k =0 ; k<piece_remain;k++){
            color(joueur,i,NULL);                                   //Affiche toutes les pieces restantes du joueur
            printf(" ");
        }
    }
}

/*
Generateur de ligne qui prend en parametre:
- Row : diferencie les lignes +---+/ |   |
- cptRow : Compteur de ligne du plateau servant de coordonnés
*/
void line(int row,board game,int cptRow,pseudo e[]){
    int cptCol = 0;                                                 //Compteur de colonne servant au coordonnées
    for(int i =0; i<14;i++){                                        //Boucle parcourant les 14 caractères de la ligne
        if (row%2!=0)                                               //Affiche la ligne +---+
        {
            if(i%4!=0){
                printf("-");
            }else{
                printf("+");
            }
        }else{                                                      //Affiche les lignes |   |
            if(i%4!=0 && i%2==0){                                   //affiche les pieces ou les numéros de colonnes si la ligne est 0
                if(row !=0){
                color(get_place_holder(game,cptRow,cptCol),get_piece_size(game,cptRow,cptCol),NULL);    //Affiche la piece via color en recupérant sa taille et son possesseur
                }else{
                    printf("%d",cptCol);            
                }
                cptCol++;                                           //Augmentation de la valeur de coordonnées colonne après affichage de la pièce
            }else if(i==13&& row!=0){                               //Affiche les numéros de lignes
                printf("%d",cptRow);
            }else if(i%4!=0){                                       //Affichage des vides entre | et la pièce
                printf(" ");
            }else{                                                  //Affiche |
                printf("|");
            }
        }
    }
    switch (row)                                                    //Affiche le contenu des maisons
    {
    case 1:
        printf("\t\tContenu des maisons");
        break;
    case 3:
        printf("\t\t");
        color(1,4,e);                                               //Affiche le pseudo du joueur 1
        printf(" : ");
        gather_piece_remaining(game,1);                             //Affiche les pièces restantes via gather_piece_remaining
        break;
    case 5:
        printf("\t\t");
        color(2,5,e);                                               //Affiche le pseudo du joueur 2
        printf(" : ");
        gather_piece_remaining(game,2);                             //Affiche les pièces restantes via gather_piece_remaining
        break; 
    }
}


/*
Genere le plateau 
*/
void plateau(board game,pseudo e[]){
    int cptRow = 0;
    for(int i=0;i<8;i++){
        line(i,game,cptRow,e);                                      //Affiche les lignes du tableau
        printf("\n");
        if(i%2!=0 && i>2){
            cptRow++;                                               //Augmentation de la valeur de coordonnées quand c'est une ligne +--+
        }
    }
}



/* 
Demarre la partie 
*/
void start(board game){
    int current_player = 1;                                         //Defnition du premier joueur
    int choice,x1,y1,x2,y2,possible,taille;                         //Initialisation des variables
    pseudo e[NB_PLAYERS];                                           //Initialisation du nombre de joueur
    
    printf("\nQuel est le pseudo du joueur 1:\n");                  //Choix des pseudos
    scanf("%s",e[0].name);
    printf("\nQuel est le pseudo du joueur 2:\n");
    scanf("%s",e[1].name);


    while(get_winner(game)==0){     //Tourne tant qu'il n'y a pas de gagnant
        choice = 3;
        x1 = 5;             // initialisation des valeurs en erreur pour palier une saisi non int
        x2 =5;
        y1 = 5;
        y2 = 5;                                      
        do{
            printf("\n----------------------------------------\n"); //Affichage du plateau, tour du joueur       
            color(current_player,current_player+3,e);        
            printf(", à vous de jouez:\n\n");
            plateau(game,e);
            do{
            printf("\nVoulez vous deplacer(0) ou placer(1) une piece : "); //Demande du choix 
            scanf("%d",&choice);
            viderBuffer();
            }while(choice != 0 && choice != 1);
            switch(choice){                                         //switch permettant de diffirencier placer/deplacer
                case 0 :                                            // mode deplacer
                    
                        printf("\nCoordonées de la pièce à déplacer:\n");
                        printf("Ligne :");
                        scanf("%d",&x1);
                        printf("Colonne :");
                        scanf("%d",&y1);
                        printf("\nCoordonées de la case cible:\n");
                        printf("Ligne :");
                        scanf("%d",&x2);
                        printf("Colonne :");
                        scanf("%d",&y2);
                        
                        if(get_place_holder(game,x1,y1) == next_player(current_player)){        //Recupere et affiche les cas d'erreurs
                            possible =4;
                        }else{
                            possible =  move_piece(game,x1,y1,x2,y2);
                        }
                        switch (possible)
                        {
                        case 0:
                            move_piece(game,x1,y1,x2,y2);

                            break;
                        case 1: 
                            printf("\n\033[31;01mErreur: il n'y a pas de pièce dans la case source\n\033[00m");
                            break;
                        case 2: 
                            printf("\n\033[31;01mErreur: la pièce est est plus petite que celle de la case cible\n\033[00m");
                            break;
                        case 3:
                            printf("\n\033[31;01mErreur: les coordonnées sont incorrects\n\033[00m");
                            break;  
                        case 4:
                            printf("\033[31;01mErreur: Vous ne pouvez pas bougez des pièces que vous ne possedez pas\n\033[00m");        
                            break;
                        }
                    
                    break;

                case 1 :                                                                //mode placer
                    taille =4;
                        do{
                        printf("\nTaille de la pièce (1:SMALL,2:MEDIUM,3:LARGE):");
                        scanf("%d",&taille);
                        viderBuffer();
                        }while(taille >3 || taille<1);
                        printf("\nCoordonées de la pièce à placer:\n");
                        printf("Ligne :");
                        scanf("%d",&x1);
                        printf("Colonne :");
                        scanf("%d",&y1);
                        possible =  place_piece(game,current_player,taille,x1,y1);          //Recupere et affiche les cas d'erreurs
                        switch (possible)
                        {
                        case 0:
                            place_piece(game,current_player,taille,x1,y1);
                            break;
                        case 1: 
                            printf("\n\033[31;01mErreur: Vous n'avez plus de pièce de ce type\n\033[00m");
                            break;
                        case 2: 
                            printf("\n\033[31;01mErreur: la pièce est est plus petite que celle de la case cible\n\033[00m");
                            break;
                        case 3:
                            printf("\n\033[31;01mErreur: les coordonnées sont incorrects\n\033[00m");
                            break;            
                        }
                    break; 
                default:
                    break;

            }
        viderBuffer();                                          //vide la memoire
        }while(possible!=0);
    current_player = next_player(current_player);               //Change de joueur
    }
    plateau(game,e);                                            //affiche le plateau gagnant
    color(get_winner(game),get_winner(game)+3,e);               //affiche le joueur gagnant
    printf(" gagne\n");
}



int main(void){
    // Demande si l'on veut jouer a globblerts, permet une insertion d'autre jeu
    char play ;
    do{
        printf("\nVoulez jouez à Globblets Globblers ?(Y/N):\n");
        play = getchar();
        viderBuffer();
    }while(play != 'Y' && play != 'N');

    if(play=='Y'){
        board game = new_game();
        start(game);
        
    destroy_game(game);
    printf("\nSuppression du plateau et sortie\n");	
    }


    return 0;
}