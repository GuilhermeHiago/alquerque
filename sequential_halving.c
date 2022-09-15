#include "alquerque.c"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>

#define BUDGET 100000

typedef struct{
    Play action;
    int** state;
    int n;
    int reward;
}node;

int playout(int **state){
    double upper_limit = 1.0;
    double lower_limit = .4;
    double value = ((upper_limit-lower_limit)*rand())/RAND_MAX + lower_limit;
    sleep(value);
    return rand() % 2;
}

void simulate(node *n, int budget){
    //* PODE PARALELIZAR AQUI *//
    for(int i = 0; i < budget; i++){
        n->reward = n->reward + playout(n->state);
        n->n = n-> n + 1;
    }
}

// gcc sequential_halving.c -lm -o run
int main (int argc, const char * argv[]) 
{
    double starttime, stoptime;
    //starttime = omp_get_wtime(); 
    //omp_set_num_threads(2);
    
    srand(time(NULL));
    int *state[] = {(int[]){1,-1,1,-1,1},
                          (int[]){0,0,0,0,0},
                          (int[]){0,0,0,0,0},
                          (int[]){0,0,0,0,0},
                          (int[]){-1,1,-1,1,-1}
    };

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
    while(layer_plays > 1){
        int layer_budget = (int)floor(BUDGET/(layer_plays*ceil(log2(n_plays))));
        
        //monte-carlo
        //* PODE PARALELIZAR AQUI *//
        //#pragma omp for schedule (dynamic)
        for(int i = 0; i < layer_plays;i++){
            simulate(&children[i], layer_budget);
        }
        
        //sort
        for (int counter = 0 ; counter < layer_plays - 1; counter++)
        {
            for (int counter1 = 0 ; counter1 < layer_plays - counter - 1; counter1++)
            {
                double r1 = (float)children[counter1].reward/children[counter1].n; 
                double r2 = (float)children[counter1+1].reward/children[counter1+1].n; 
                if (r1 < r2) {
                    node swap_var     = children[counter1];
                    children[counter1]   = children[counter1+1];
                    children[counter1+1] = swap_var;
                }
            }
        }

        //halve 
        layer_plays = (int)layer_plays/2;
    }
}