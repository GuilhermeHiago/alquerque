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

Play sequential_halving(int **state, int budget){
    srand(time(NULL));

    int n_plays = 0;
    Play *action_children = get_possible_plays(state, 1, &n_plays);
    node *children = alloc(sizeof(node) * n_plays);

    for(int i=0; i < n_plays;i++){
        children[i].action = action_children[i];
        children[i].state = apply_play(state, children[i].action);
        children[i].n = 0;
        children[i].reward = 0;
    }

    int layer_size = n_plays;
    int layer_budget = budget;
    while(layer_size > 1){
        layer_budget = layer_budget/layer_size; //TODO
        for(int i = 0; i < layer_size;i++){
            child_budget = layer_budget/layer_size; //TODO
            simulate(children[i], child_budget);
        }
        layer_size = (int)layer_budget/2;
        sort(children, layer_budget);
    }
    Play action = children[0].action;
    free(children);
    free(action_children);
    return children[0].action;

}

