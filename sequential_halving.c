#include "alquerque.c"
#include <time.h>
#include <stdlib.h>

#define BUDGET 100000

typedef struct{
    Play action;
    int** state;
    int n;
    int reward;
}node;

int playout(int **state, int player){
    
    int** playout_state = createArray(MAX_SIZE, MAX_SIZE);
    clone_matrix(state, playout_state);
    int curr_player = 1;
    
    while(is_terminal(playout_state)){
        int n_plays = 0;
        Play *possible_plays = get_possible_plays(playout_state, curr_player, &n_plays);
        curr_player *= -1;

        Play next_action = possible_plays[rand() % n_plays];
        playout_state = apply_play(playout_state, next_action);
        free(possible_plays);
    }
    free(playout_state);
    return is_final_state(playout_state, player)?1:0;
}

void simulate(node n, int budget){
    for(int i = 0; i < budget; i++){
        n.reward+= playout(n.state, 1);
        n.n+= 1;
    }
}

// Play sequential_halving(int **state, int budget){
//     srand(time(NULL));

//     int n_plays = 0;
//     Play *action_children = get_possible_plays(state, 1, &n_plays);
//     node *children = alloc(sizeof(node) * n_plays);

//     for(int i=0; i < n_plays;i++){
//         children[i].action = action_children[i];
//         children[i].state = apply_play(state, children[i].action);
//         children[i].n = 0;
//         children[i].reward = 0;
//     }

//     int layer_size = n_plays;
//     int layer_budget = budget;
//     while(layer_size > 1){
//         layer_budget = layer_budget/layer_size; //TODOlist
//         sort(children, layer_budget);
//     }
//     Play action = children[0].action;
//     free(children);
//     free(action_children);
//     return children[0].action;

// }

// Função de Ordenação por Seleção
// Quick sort function
void quick_sort(node *a, int left, int right) {
    int i, j;
    node x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
    float x_value = x.reward / x.n;
     
    while(i <= j) {
        float node_i_value = a[i].reward / a[i].n;
        float node_j_value = a[j].reward / a[j].n;

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

int main(int argc, char const *argv[])
{
    node n1, n2, n3;

    n1.n = 10;
    n1.reward = 20;

    n2.n = 20;
    n2.reward = 20;

    n3.n = 30;
    n3.reward = 20;

    node list[3] = {n1, n2, n3};

    // list[0] = n1;

    quick_sort(list, 0, 2);

    for(int i = 0; i < 3; i++){
        printf("pos %d n %d\n", i, list[i].n);
    }
    /* code */
    return 0;
}
