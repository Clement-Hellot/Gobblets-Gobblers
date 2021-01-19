#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

/**
 * \file board.c
 *
 * 
 *
 * \author Clement Hellot
 */

/**
 * @brief The board of the game
 */
typedef struct{
	player memory[4];			// definition d'une structure case contenant un tableau pour le stockage des joueurs en fonction de la taille de la piece possédé sur la case 
}cases;							// memory[taille de la piece -1 pour partir du début du tableau] = numero du joueur
								// long de 4 car erreur pour une longueur de 3

struct board_s {
	cases plateau[DIMENSIONS][DIMENSIONS];		// Creation d'un plateau de case 3*3
};

board new_game(){ 
	board new_board = malloc(sizeof(struct board_s));		// allocation dynamique
	for(int i =0;i<DIMENSIONS;i++){
		for(int k =0; k<DIMENSIONS;k++){
			for(int j = 0;j<4;j++){
				new_board->plateau[i][k].memory[j]=0;
}
}
}
	return(new_board);
}
board copy_game(board game){
	board copy = new_game();
	for(int i =0;i<DIMENSIONS;i++){
		for(int k =0; k<DIMENSIONS;k++){
			for(int j = 0;j<4;j++){
				copy->plateau[i][k].memory[j]=game->plateau[i][k].memory[j];
			}
		}
	}
	
	return(copy);						// copie du jeu	
}
void destroy_game(board game){
	// freeing memory, necessary for memory allocated with malloc (leave as is)
	free(game);
}

player get_place_holder(board game,int line, int column){
	int i =2;
	while(game->plateau[line][column].memory[i] ==  0 && i >= 0){		// parcours la case du tableau de la plus grosse piece a la plus petite tant qu'il n'y a pas de joueur
		i--;															// retourne le joueur trouvé
	}
	return(game->plateau[line][column].memory[i]);
}
size get_piece_size(board game,int line,int column){
	int i =2;
	while(game->plateau[line][column].memory[i] ==  0 && i >= 0){	// parcours la case du tableau de la plus grosse piece a la plus petite tant qu'il n'y a pas de joueur 
		i--;														// retourne la taille de la piece 
	}
	return(i+1);
}

player next_player(player current_player){
    if(current_player !=0){					//change le joueur par XOR
		current_player ^= 0b11; 			//sinon defni le joueur sur 1
	}else{
		current_player = 1;
	}              // XOR
    return(current_player);
}



player get_winner(board game){
	//defini le winner 
	int winner=0;
	//parcours le tableau 
	for(int i =0; i<DIMENSIONS;i++){
		//Cherche si la ligne est gagnante 
		if(get_place_holder(game,i,0) != 0 && get_place_holder(game,i,0)==get_place_holder(game,i,1)  && get_place_holder(game,i,1)==get_place_holder(game,i,2)){
			if(winner==0 || winner == get_place_holder(game,i,0)){
				winner = get_place_holder(game,i,0);
			}
			// defini le gagnant sur 0 si les 2 joueurs sont gagnants
			else{
				winner =0;
			}
		}
		//cherche si la colonne est gagnante
		if(get_place_holder(game,0,i) != 0 && get_place_holder(game,0,i)==get_place_holder(game,1,i) && get_place_holder(game,1,i)==get_place_holder(game,2,i)){
			if(winner==0 || winner == get_place_holder(game,0,i)){
				winner = get_place_holder(game,0,i);
			}
			// defini le gagnant sur 0 si les 2 joueurs sont gagnants
			else{
				winner =0;
			}
			
		}
	}
	//cherche si la diagonal est gagnante
	if (get_place_holder(game, 0, 0) == get_place_holder(game, 1, 1) && get_place_holder(game, 2, 2) == get_place_holder(game, 1, 1) && get_place_holder(game, 1, 1) != 0){
		if (winner == 0 || winner == get_place_holder(game,1,1)){
            winner = get_place_holder(game, 1, 1);
        }
		// defini le gagnant sur 0 si les 2 joueurs sont gagnants
		else
        {
            winner = 0;
        }
	}
    if (get_place_holder(game, 0, 2) == get_place_holder(game, 1,1) && get_place_holder(game, 1,1) == get_place_holder(game, 2,0) && get_place_holder(game, 1, 1) != 0){
		if (winner == 0 || winner == get_place_holder(game,1,1)){
            winner = get_place_holder(game, 1, 1);
        }
		// defini le gagnant sur 0 si les 2 joueurs sont gagnants
		else
        {
            winner = 0;
        }
    }
		
	return(winner);
}

int get_nb_piece_in_house(board game, player checked_player, size piece_size){
	int out = NB_INITIAL_PIECES;
	if(checked_player == NO_PLAYER){
		out =0;
	}else{													//parcours les cases du tableau et soustrait 1 au nombre maximum de pieces quand une piece est trouvé
	for(int i = 0;i<DIMENSIONS;i++){
		for(int k = 0;k<DIMENSIONS;k++){
			if(game->plateau[i][k].memory[piece_size-1] == checked_player){
				out--;
			}
		}
	}
	}
	return(out);																	// retourne le nombre de pieces restantes
}

int place_piece(board game, player current_player, size piece_size, int line, int column){
	int out = 0;
	if(piece_size < get_piece_size(game,line,column)+1){	//retourne l'erreur 2 si la piece a placé n'est pas superieur a celle deja en place
		out =2;
	}else if(line >2 || column > 2 ){						//retourne l'erreur 3 si les coordonnées sont incorrects
		out =3;
	}else if(get_nb_piece_in_house(game,current_player,piece_size) == 0){		//retourne l'erreur 1 si il n'y a plus de piece disponible
		out =1;
	}else{
		game->plateau[line][column].memory[piece_size-1] = current_player;		//place la piece si il n'y a  pas d'erreur
	}
	return(out);
}


int move_piece(board game, int source_line, int source_column, int target_line, int target_column){
	int out = 0;
	if(get_piece_size(game,source_line,source_column)== 0){		//retourne l'erreur 1 si la case source est vide
		out=1;
		if(source_line >2 || source_column > 2 || target_column > 2 || target_line >2  ){ //retourne l'erreur 3 si les coordonnées sont incorrects
		out =3;
		}
	}else if(get_piece_size(game,source_line,source_column) <= get_piece_size(game,target_line,target_column)){	//retourne l'erreur 2 si la piece a déplacé n'est pas superieur a celle de la case cible
		out =2;
	}else{			//deplace la piece
		game->plateau[target_line][target_column].memory[get_piece_size(game,source_line,source_column)-1] = game->plateau[source_line][source_column].memory[get_piece_size(game,source_line,source_column)-1];
		game->plateau[source_line][source_column].memory[get_piece_size(game,source_line,source_column)-1] = 0;
		
	}
	return(out);
}


