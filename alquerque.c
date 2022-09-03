#include <stdio.h>

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

int board[MAX_SIZE][MAX_SIZE];

int show_board(){
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


Play get_possible_plays(int state[5][5], int player){
    // 5 pieces * max_of_8_plays = 40
    Play possibles[40];

    for(int i = 0; i < MAX_SIZE; i++){
        for(int j = 0; j < MAX_SIZE; j++){

            if(board[i][j] != player) continue;

            // in pair lines odd columms has diagonal moviment
            bool diagonal_on_odd = j % 2 == 0;

            for(int k = i -1; k < i+1; k++){
                for(int l = j-1; l < l+1; l++){
                    
                }
            }
        }
    }
}


bool is_final_state(int board[5][5], int player){

    for(int i = 0; i < MAX_SIZE; i++){
        for(int j = 0; j < MAX_SIZE; j++){

            if(board[i][j] != player) continue;

            // front row
            if(i + 3 < MAX_SIZE){
                if(board[i][j] && board[i+1][j] && board[i+2][j] && board[i+3][j])
                    return true;
            }

            // down front diagonal
            if(i + 3 < MAX_SIZE && j + 3 < MAX_SIZE){
                if(board[i][j] && board[i+1][j+1] && board[i+2][j+2] && board[i+3][j+3])
                    return true;
            }

            // down back diagonal
            if(i - 3 >= 0 && j + 3 < MAX_SIZE){
                if(board[i][j] && board[i-1][j+1] && board[i-2][j+2] && board[i-3][j+3])
                    return true;
            }

            // columm
            if(j + 3 < MAX_SIZE){
                if(board[i][j] && board[i][j+1] && board[i][j+2] && board[i][j+3])
                    return true;
            }

        }
    }

    return false;
}


int main (int argc, const char * argv[]) 
{
    board[1][4] = board[2][3] = board[3][2] = board[4][1] = 1;
    show_board();
    printf("%d", is_final_state(board, 1));
}