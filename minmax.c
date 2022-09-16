#include <stdio.h>
//only include alquerque.c once
#ifndef MAX_SIZE
	#include "alquerque.c"
#endif

#define NUMLINES 28


/********************
* Heuristic
********************/

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

void minMaxInit(){
	lines = generateLines();
}

/****************************
*	Minmax
****************************/
typedef struct MMNode MMNode;
struct MMNode{
	Play lastPlay; 
	int **board;
	int player;
	int value;	
	int numChildren;
	MMNode *children;
};

void expandNode(MMNode *n){
	int nextPlayer = -1*n->player;
	
	int nPlays = -1; ////////////%FIXME%
	Play* nextPlays = get_possible_plays(n->board, nextPlayer, &nPlays);
	n->children = malloc(nPlays*sizeof(MMNode));
	n->numChildren = nPlays;
	printf("%d\n", n->numChildren);
	for(int i = 0; i < nPlays; i++){
		Play p = nextPlays[i];
		int **b = apply_play(n->board, p);
		n->children[i].lastPlay = p;
		n->children[i].board = b;
		n->children[i].player = nextPlayer;
		n->children[i].value = heuristic(b);
		n->children[i].numChildren = 0;
		n->children[i].children = NULL;		
	}
}

void printChildren(MMNode n){
	printf("_|F (Player:%d Children:%d)\n", n.player, n.numChildren);
	for(int i = 0; i < n.numChildren; i++){
		MMNode node = n.children[i];
		Play p = node.lastPlay;
		printf("| [(%d, %d) -> (%d, %d)] (Player:%d Value:%d Children:%d)\n", p.x1, p.y1, p.x2, p.y2, node.player, node.value, node.numChildren);
	}
}

int main(){
	minMaxInit();
	
	int *board[] = {(int[]){1,-1,1,-1,1},
					   (int[]){0,0,0,0,0},
					   (int[]){0,0,0,0,0},
					   (int[]){0,0,0,0,0},
					   (int[]){-1,1,-1,1,-1}
	};
	show_board(board);
	
	MMNode testNode; testNode.board = board; testNode.player = 1;
	expandNode(&testNode);
	printf("\n");
	printChildren(testNode);
	return 0;
}
