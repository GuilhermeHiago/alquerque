#include <stdio.h>
#include "alquerque.c"
#define NUMLINES 28

typedef struct {
     int x, y;
} Pos;

Pos** lines;

//possible win lines
Pos** generateLines(){
	//10 horiz, 10 vert, 8 diag, 4 positions per line
	Pos *aux = malloc((NUMLINES*4)*sizeof(Pos));
	Pos **lines = malloc(NUMLINES*sizeof(Pos*));
	for(int i = 0; i < NUMLINES; i++){
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
	/*diag / */
	for(int i0 = 0; i0 < 2; i0++){
		for(int j0 = 4; j0 > 2; j0--){
			for(int desloc = 0; desloc < 4; desloc++){
				lines[count][desloc].x = i0+desloc;
				lines[count][desloc].y = j0-desloc;
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
	//total
	int sum = 0;
	//go through each possible line
	for(int i = 0; i < NUMLINES; i++){
		int cur = 0; //player
		int count = 0; //player's pieces in a line
		//position in line
		for(int j = 0; j < 4; j++){
			Pos xy = lines[i][j];
			int p = board[xy.x][xy.y];
			if(p == 0) continue;
			//set last found to current player
			if(cur == 0){
				cur = p;
				count = 1;
				continue;
			}
			//two players, neither can form a line, abort
			if(cur != p){
				count = 0;
				break;
			}
			count++;
		}
		//at least two pieces in line
		if(count > 1){
			//won the game, return a value over maximum heuristic 
			if(count == 4) return cur*(NUMLINES*4);
			//otherwise add to sum 
			sum += count*cur;
		}
	}
	return sum;
}

int main(){
	lines = generateLines();

	int **board = createArray(MAX_SIZE, MAX_SIZE);
	board[0][0] = 0;
	board[1][0] = 0;
	board[2][0] = 1;
	board[3][0] = 1;
	board[4][0] = 1;
	show_board(board);
	printf("%d\n", heuristic(board));
	destroyArray(board);
	return 0;
}
