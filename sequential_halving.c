#include "alquerque.c"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BUDGET 100000

typedef struct{
    Play action;
    int** state;
    int n;
    int reward;
}node;


// Função de Ordenação por Seleção
// Quick sort function
void quick_sort(node *a, int left, int right) {
    int i, j;
    node x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
    float x_value = (float)x.reward / x.n;
    
    while(i <= j) {
        float node_i_value = (float)a[i].reward / a[i].n;
        float node_j_value = (float)a[j].reward / a[j].n;

        while(node_i_value > x_value && i < right) {
            i++;
        }
        while(node_j_value < x_value && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
     
    if(j > left) {
        quick_sort(a, left, j);
    }
    if(i < right) {
        quick_sort(a, i, right);
    }
}


int playout(int **state){
    
    int** playout_state = createArray(MAX_SIZE, MAX_SIZE);
    clone_matrix(state, playout_state);
    int curr_player = 1;
    int max_depth = 30;
    int curr_depth = 0;
    while(!is_terminal(playout_state)){
        if(curr_depth >= max_depth) break;
        curr_depth+=1;
        int n_plays = 0;
        Play *possible_plays = get_possible_plays(playout_state, curr_player, &n_plays);
        curr_player *= -1;

        Play next_action = possible_plays[rand() % n_plays];
        playout_state = apply_play(playout_state, next_action);
        free(possible_plays);
    }
    int reward = is_final_state(playout_state, curr_player)?1:0;
    //show_board(state);
    //show_board(playout_state);
    free(playout_state);
    return reward;
}

void simulate(node *n, int budget){
    for(int i = 0; i < budget; i++){
        n->reward = n->reward + playout(n->state);
        n->n = n-> n + 1;
    }
}

Play sequential_halving(int **state, int budget){
    srand(time(NULL));
    int n_plays = 0;
    Play *action_children = get_possible_plays(state, 1, &n_plays);
    node *children = malloc(sizeof(node) * n_plays);
    for(int i=0; i < n_plays;i++){
        children[i].action = action_children[i];
        children[i].state = apply_play(state, children[i].action);
        children[i].n = 0;
        children[i].reward = 0;
    }

    int layer_plays = n_plays;
    show_board(children[0].state);
    while(layer_plays > 1){
        int layer_budget = (int)floor(budget/(layer_plays*ceil(log2(n_plays))));
        printf("layer %d size %d\n", layer_budget, layer_plays);
        
        for(int i = 0; i < layer_plays;i++){
            simulate(&children[i], layer_budget);
        }
        layer_plays = (int)layer_plays/2;
        quick_sort(children, 0, layer_budget);
    }
    Play action = children[0].action;
    
    
    /*
    for(int i = 0; i < n_plays; i++){
        Play ac = children[i].action;
        printf("(%d,%d)-->(%d,%d)  %d/%d\n", ac.x1, ac.y1, ac.x2, ac.y2, children[i].reward, children[i].n);
    }
    */


    //show_board(children[0].state);
    free(children);
    free(action_children);
    return children[0].action;

}

// gcc sequential_halving.c -lm -o run
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

    sequential_halving(board, 1000);
}