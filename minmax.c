#include <stdio.h>
//only include alquerque.c once
#ifndef MAX_SIZE
	#include "alquerque.c"
#endif

#define NUMLINES 28
#define MAXVALUE 100
#define MAXWIDTH 1500


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
			if(count == 4) return cur*MAXVALUE;
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
	bool isFinal;
	MMNode *children;
};

//expand current node
void expandNode(MMNode *n){
	if(n->isFinal) return;
	int nextPlayer = -1*n->player;
	
	int nPlays = -1; ////////////%FIXME%
	Play* nextPlays = get_possible_plays(n->board, n->player, &nPlays);
	n->children = malloc(nPlays*sizeof(MMNode));
	n->numChildren = nPlays;
	for(int i = 0; i < nPlays; i++){
		Play p = nextPlays[i];
		int **b = apply_play(n->board, p);
		n->children[i].lastPlay = p;
		n->children[i].board = b;
		n->children[i].player = nextPlayer;
		int value = heuristic(b);
		n->children[i].value = value;
		//final state
		if(value == MAXVALUE || value == MAXVALUE*-1){
			n->children[i].isFinal = true;
		}else{
			n->children[i].isFinal = false;
		}
		n->children[i].numChildren = 0;
		n->children[i].children = NULL;		
	}
}

//expand until depth
void expandTo(int depth, MMNode *root){
	//nodes to expand on cur depth
	MMNode **curD = calloc(MAXWIDTH, sizeof(MMNode*));
	//nodes on next depth 
	MMNode **nextD = calloc(MAXWIDTH, sizeof(MMNode*));
	//number of nodes on cur depth
	int curN = 1;
	
	curD[0] = root;	
	for(int d = 0; d < depth; d++){
		//expand nodes on current depth
		for(int i = 0; i < curN; i++){
			if(curD[i]->isFinal) continue;
			expandNode(curD[i]);
		}
		//add children to nextD
		int nextN = 0;
		for(int i = 0; i < curN; i++){
			for(int j = 0; j < curD[i]->numChildren; j++){
				nextD[nextN] = &(curD[i]->children[j]);
				nextN++;
			}	
		}
		//next to cur
		MMNode **aux = curD;
		curD = nextD;
		nextD = aux;
		//nextN is now curN
		curN = nextN;
	}
	free(curD);
	free(nextD);
	printf("[NEXT DEPTH REQUIRES MAXWIDTH>=%d]\n", curN);
}

//send values up the tree
int recalculate(MMNode *root){
	//final node
	if(root->numChildren == 0) return root->value;
	int values[root->numChildren];
	//recalculate children
	for(int i = 0; i < root->numChildren; i++){
		values[i] = recalculate(&(root->children[i]));
	}
	int value;
	value = values[0];
	//get max 
	if(root->player == 1){
		for(int i = 1; i < root->numChildren; i++){
			if(values[i] > value) value = values[i];
		}
	}
	//get min
	else{
		for(int i = 1; i < root->numChildren; i++){
			if(values[i] < value) value = values[i];
		}
	}
	root->value = value;
	return value;
}

//cleanup
void freeTree(MMNode *root){
	if(root->numChildren == 0) return;
	//free children of children
	for(int i = 0; i < root->numChildren; i++){
		freeTree(&(root->children[i]));
	}
	//free children
	free(root->children);
}

//get next play
Play minmax(int **board, int player, int depth){
	//root node
	MMNode root; 
	root.board = board; root.value = 0; root.player=1; root.numChildren=0; root.isFinal = false;
	
	//expand tree
	expandTo(depth, &root);
	//recalculate values and get the max/min
	int value = recalculate(&root);
	
	//find first max/min and get next play
	Play play; 
	for(int i = 0; i < root.numChildren; i++){
		if(root.children[i].value == value){
			play = root.children[i].lastPlay;
			break;
		}
	}
	
	//free tree
	freeTree(&root);
	
	return play;
}


/*****************************
*	TEST
*****************************/
void printChildren(MMNode n){
	printf("_|F (Player:%d Children:%d Value:%d)\n", n.player, n.numChildren, n.value);
	for(int i = 0; i < n.numChildren; i++){
		MMNode node = n.children[i];
		Play p = node.lastPlay;
		printf("| [(%d, %d) -> (%d, %d)] (Player:%d Value:%d Children:%d)\n", p.x1, p.y1, p.x2, p.y2, node.player, node.value, node.numChildren);
	}
}

void printPos(Pos p){
	printf("x=%d y=%d | ", p.x, p.y);
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
	
	/*MMNode testNode; testNode.board = board; testNode.value = 0; testNode.player = 1; testNode.numChildren = 0; testNode.isFinal = false;
	
	
	printf("\nDEPTH=0\n");
	printChildren(testNode);
	
	printf("\nDEPTH=1\n");
	expandTo(1, &testNode);	
	printChildren(testNode);
	printChildren(testNode.children[0]);
	
	printf("\nDEPTH=2\n");
	expandTo(2, &testNode);
	printChildren(testNode);
	printChildren(testNode.children[0]);
	printChildren(testNode.children[0].children[0]);
	
	printf("\nDEPTH=3\n");
	expandTo(3, &testNode);
	printChildren(testNode);
	printChildren(testNode.children[0]);
	printChildren(testNode.children[0].children[0]);
	
	printf("\nRECALCULATE\n");
	recalculate(&testNode);
	printChildren(testNode);
	freeTree(&testNode);
	printf("\n");*/
	
	int player = 1;
	int depth = 4;
	Play next = minmax(board, player, depth);
	printf("Next play: (%d,%d)->(%d,%d)", next.x1, next.y1, next.x2, next.y2);
	
	return 0;
}
