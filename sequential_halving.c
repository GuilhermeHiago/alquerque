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
    srand(time(NULL));   // Initialization, should only be called once.
    
    int** playout_state = createArray(MAX_SIZE, MAX_SIZE);
    clone_matrix(state, playout_state);
    int curr_player = 1;
    
    while(is_terminal(playout_state)){
        int n_plays;
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

