#include "../util/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct valve {
    int pressure;
    u_int64_t key;
    char id[2];
    int* neighbours;
    int neighbour_count;
};

int** calculate_distances(struct valve** valves, int valves_count){
    int** distances = malloc(valves_count * sizeof(int *));
    for (int i=0; i<valves_count;i++){
        distances[i] = calloc(valves_count, sizeof(int));
        for (int j=0; j<valves_count;j++){
            if (i != j){
                distances[i][j] = 1000;
            } else {
                distances[i][j] = 0;
            }

            struct valve v = *valves[i];
            for (int k=0;k<v.neighbour_count;k++){
                if (v.neighbours[k] == j){
                    distances[i][j] = 1;
                }
            }
        }
    }

    for (int k=0; k<valves_count;k++){
        for (int i=0; i<valves_count;i++){
            for (int j=0; j<valves_count;j++){
                int dist = distances[i][k]+distances[k][j];
                if (distances[i][j] > dist){
                    distances[i][j] = dist;
                }
            }
        }
    }
    return distances;
}

void visit(int* results, int valve, uint16_t route, int remaining_minutes, int pressure, struct valve** valves, int** distances, int valves_count){
    int existing = results[route];
    if (existing < pressure){
        results[route] = pressure;
    }
    for (int i=0;i<valves_count;i++){
        if (i == valve) continue;
        struct valve v = *valves[i];
        int minutes = remaining_minutes - distances[valve][i] - 1;
        // ignore already visited or unpressurised valves
        if ((route & v.key) != 0 || v.pressure == 0 || remaining_minutes < 0) continue;
        int additional_pressure = minutes * v.pressure;
        visit(results, i, route | v.key, minutes, pressure+additional_pressure, valves, distances, valves_count);
    }
}

int main(){
    struct file_contents fc = read_lines("input");
    struct valve* valves[fc.line_count];
    int valves_count = fc.line_count;
    int non_zero_valves_count = 0;  
    int max_flow = 0;
    char* neighbours[fc.line_count];
    int start_valve_index = 0;
    for (int i=0; i<fc.line_count; i++){
        struct valve* v = malloc(sizeof(struct valve));
        v->neighbours = malloc(5 * sizeof(int));
        v->neighbour_count = 0;
        neighbours[i] = malloc(20*sizeof(char));
        sscanf(fc.lines[i], "Valve %2c has flow rate=%d; tunnels lead to valves %[^\n]", v->id, &v->pressure, neighbours[i]);
        valves[i] = v;
        if (v->pressure > 0){
            v->key = 1 << non_zero_valves_count;
            non_zero_valves_count++;            
        } else {
            v->key = 0;
        }
        if (strcmp(v->id, "AA")==0){
            start_valve_index = i;
        }
    }
    
    for (int i=0; i<fc.line_count; i++){
        char* entry;
        struct valve* valve = valves[i];
        char* neighbours_str = neighbours[i];
        while ((entry = strtok_r(neighbours_str, ", ", &neighbours_str)) != NULL){
            for (int j=0;j<fc.line_count;j++){
                if (strncmp(valves[j]->id, entry, 2) == 0){
                    valve->neighbours[valve->neighbour_count] = j;
                    valve->neighbour_count += 1;
                    break;
                }
            }
        };
    }
    int** distances = calculate_distances(valves, valves_count);    
    int total_routes = 1<<non_zero_valves_count;
    int* results = calloc(total_routes, sizeof(int));
    visit(results, start_valve_index, 0, 30, 0, valves, distances, valves_count); 
    int max_pressure = 0;
    for (int i=0;i<total_routes;i++){
        if (results[i] > max_pressure){
            max_pressure = results[i];
        } 
    }
    printf("Part 1 - %i\n", max_pressure);

    int* results_part_2 = calloc(total_routes, sizeof(int));
    visit(results_part_2, start_valve_index, 0, 26, 0, valves, distances, valves_count); 
    max_pressure = 0;
    for (int i=0;i<total_routes;i++){
        for (int j=0;j<total_routes;j++){
            if ((i & j) > 0) continue;
            int total = results_part_2[i] + results_part_2[j];
            if (total > max_pressure){
                max_pressure = total;
            }
        }
    }
    printf("Part 2 - %i\n", max_pressure);
}