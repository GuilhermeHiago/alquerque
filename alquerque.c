#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5

// typedef struct
// {
//     int x, y;
// }Pos;

typedef struct
{
    int x1, y1, x2, y2;
}Play;

typedef enum { false, true } bool;

int *clean_board[MAX_SIZE][MAX_SIZE];


/**
 * Find maximum between two numbers.
 */
int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

/**
 * Find minimum between two numbers.
 */
int min(int num1, int num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}

/**
 * Receives:
 *  int m (number of columms)
 *  int n (number of rows)
 * Returns:
 *  int** (matrix of int filled with 0)
*/
int** createArray(int m, int n)
{
    int* values = calloc(m*n, sizeof(int));
    int** rows = malloc(m*sizeof(int*));
    for (int i=0; i<m; ++i)
    {
        rows[i] = values + i*n;
    }
    return rows;
}

void clone_matrix(int **orig, int **dest){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            dest[i][j] = orig[i][j];
        }
    }
}

int destroyArray(int** arr)
{
    free(*arr);
    free(arr);

    return 0;
}

int show_board(int **board){
    char board2[MAX_SIZE][MAX_SIZE];

    for(int i =0; i < MAX_SIZE; i++){
        for(int j =0; j < MAX_SIZE; j++){
            switch (board[i][j])
            {
            case 1:
                board2[i][j] = 'X';
                break;
            case -1:
                board2[i][j] = 'O';
                break;
            default:
                board2[i][j] = '*';
                break;
            }
        }
    }

    printf("%c - %c - %c - %c - %c\n", board2[0][0], board2[0][1], board2[0][2], board2[0][3], board2[0][4]);
    printf("| \\ | / | \\ | / |\n");
    printf("%c - %c - %c - %c - %c\n", board2[1][0], board2[1][1], board2[1][2], board2[1][3], board2[1][4]);
    printf("| / | \\ | / | \\ |\n");
    printf("%c - %c - %c - %c - %c\n", board2[2][0], board2[2][1], board2[2][2], board2[2][3], board2[2][4]);
    printf("| \\ | / | \\ | / |\n");
    printf("%c - %c - %c - %c - %c\n", board2[3][0], board2[3][1], board2[3][2], board2[3][3], board2[3][4]);
    printf("| / | \\ | / | \\ |\n");
    printf("%c - %c - %c - %c - %c\n", board2[4][0], board2[4][1], board2[4][2], board2[4][3], board2[4][4]);
}

/**
 * Receives:
    int **state (board to verify)
    int player (player value on board)
    int *n_plays (addres of int var to save number of plays)
 * Return:
    Play * (Play[] with list of possible plays for a player)
*/
Play* get_possible_plays(int **state, int player, int *n_plays){
    // 5 pieces * max_of_8_plays = 40
    Play *possibles = malloc(sizeof(Play) * 40);
    int count = 0;

    for(int i = 0; i < MAX_SIZE; i++){
        for(int j = 0; j < MAX_SIZE; j++){
            
            if(state[i][j] != player) continue;

            // // in pair lines odd columms has diagonal moviment
            bool diagonal = i % 2 == j % 2;
    
            for(int k = max(0, i-1); k <= min(i+1, MAX_SIZE-1); k++){
                for(int l = max(0, j-1); l <= min(j+1, MAX_SIZE-1); l++){
                    if(state[k][l] != 0){
                        continue;
                    }

                    if(!diagonal && (k == i-1 || k == i+1) && (l == j-1 || l == j+1)){
                        continue;
                    }

            

                    Play move;
                    move.x1 = i;
                    move.y1 = j;
                    move.x2 = k;
                    move.y2 = l;
                    possibles[count] = move;
                    count++;
                }
            }
        }
    }

    *n_plays += count + 1;

    return possibles;
}

/*
Receives:
    int **board (the board to verify)
    int player (player value)
Return:
    bool (if exist line of four pieces)
*/
bool is_final_state(int **board, int player){

    for(int i = 0; i < MAX_SIZE; i++){
        for(int j = 0; j < MAX_SIZE; j++){

            if(board[i][j] != player) continue;


            // down front diagonal
            if(i + 3 < MAX_SIZE && j + 3 < MAX_SIZE){
                if(board[i][j] && board[i+1][j+1] && board[i+2][j+2] && board[i+3][j+3])
                    return true;
            }
            // front row
            else if(i + 3 < MAX_SIZE){
                if(board[i][j] && board[i+1][j] && board[i+2][j] && board[i+3][j])
                    return true;
            }
            // down back diagonal
            else if(i - 3 >= 0 && j + 3 < MAX_SIZE){
                if(board[i][j] && board[i-1][j+1] && board[i-2][j+2] && board[i-3][j+3])
                    return true;
            }
            // columm
            else if(j + 3 < MAX_SIZE){
                if(board[i][j] && board[i][j+1] && board[i][j+2] && board[i][j+3])
                    return true;
            }

        }
    }

    return false;
}

/*
Receives: 
    int matrix board 
    Play play : play to apply
Returns:
    int **new_board (new pointer)
*/
int** apply_play(int **board, Play play){

    int **new_board = createArray(MAX_SIZE, MAX_SIZE);
    clone_matrix(board, new_board);

    new_board[play.x2][play.y2] = new_board[play.x1][play.y1];
    new_board[play.x1][play.y1] = 0;

    return new_board;
}

int main (int argc, const char * argv[]) 
{
    // board[1][4] = board[2][3] = board[3][2] = board[4][1] = 1;
    int *board[] = {(int[]){1,-1,1,-1,1},
                          (int[]){0,0,0,0,0},
                          (int[]){0,0,0,0,0},
                          (int[]){0,0,0,0,0},
                          (int[]){-1,1,-1,1,-1}
    };

    Play p;
    p.x1 = 0;
    p.y1 = 0;
    p.x2 = 1;
    p.y2 = 1;

    int **board2 = apply_play(board, p);
    // show_board(board2);
    show_board(board);
    printf("%d\n", is_final_state(board2, 1));

    int n_plays = 0;
    Play* poss = get_possible_plays(board, 1, &n_plays);

    printf("number of plays: %d\n", n_plays);

    for(int i = 0; i < n_plays-1; i++) {
        printf(" (%d,%d) -> (%d,%d), ", poss[i].x1, poss[i].y1, poss[i].x2, poss[i].y2);
    }

    destroyArray(board);
    destroyArray(board2);
}