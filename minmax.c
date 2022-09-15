#include <stdio.h>
#include "alquerque.c"

typedef struct {
     int x, y;
} Pos;

//possible win lines
Pos** generateLines(){
	//10 horiz, 10 vert, 4 diag, 4 positions per line
	Pos *aux = malloc((24*4)*sizeof(Pos));
	Pos **lines = malloc(24*sizeof(Pos*));
	for(int i = 0; i < 24; i++){
		lines[i] = aux + i*4;
	}
	
	int count = 0;
	//horiz
	for(int i = 0; i < MAX_SIZE; i++){
		for(int j0 = 0; j0 < 2; j0++){
			for(int j = 0; j < 4; j++){
				lines[count][j].x = i;
				lines[count][j].y = j+j0;
			}
			count += 1;
		}
	}
	//vert
	for(int i = 0; i < MAX_SIZE; i++){
		for(int j0 = 0; j0 < 2; j0++){
			for(int j = 0; j < 4; j++){
				lines[count][j].x = j+j0;
				lines[count][j].y = i;
			}
			count += 1;
		}
	}
	/*diag \ */
	for(int i0 = 0; i0 < 2; i0++){
		for(int j0 = 0; j0 < 2; j0++){
			for(int desloc = 0; desloc < 4; desloc++){
				lines[count][desloc].x = i0+desloc;
				lines[count][desloc].y = j0+desloc;
			}
			count += 1;
		}
	}
	
	return lines;
}

void printPos(Pos p){
	printf("x=%d y=%d | ", p.x, p.y);
}

int heuristic(int** board){
	int sum = 0;
	for(int i = 0; i < MAX_SIZE; i++){
		for(int j = 0; j < MAX_SIZE; j++){
			if(board[i][j] == 0) continue;
			
			// down front diagonal
			if(i + 3 < MAX_SIZE && j + 3 < MAX_SIZE){
				if(board[i][j] && board[i+1][j+1] && board[i+2][j+2] && board[i+3][j+3])
					return 1;
			}
			// front row
			else if(i + 3 < MAX_SIZE){
				if(board[i][j] && board[i+1][j] && board[i+2][j] && board[i+3][j])
					return 1;
			}
			// down back diagonal
			else if(i - 3 >= 0 && j + 3 < MAX_SIZE){
				if(board[i][j] && board[i-1][j+1] && board[i-2][j+2] && board[i-3][j+3])
					return 1;
			}
			// columm
			else if(j + 3 < MAX_SIZE){
				if(board[i][j] && board[i][j+1] && board[i][j+2] && board[i][j+3])
					return 1;
			}
		}
	}
	return false;
}

int main(){
	Pos** lines = generateLines();
	for(int i = 0; i < 4; i++){
		printPos(lines[21][i]);
	}
	printf("\n");

/*	int **board = createArray(MAX_SIZE, MAX_SIZE);
	board[0][0] = 1;
	board[1][0] = 1;
	board[2][0] = 1;
	board[3][0] = 1;
	board[4][0] = 1;
	show_board(board);
	printf("%d\n", heuristic(board));
	destroyArray(board);
	printf("test\n");*/
	return 0;
}
