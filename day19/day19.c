#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/util.h"

struct cost {
    int ore;
    int clay;
    int obsidian;
};

struct blueprint {
    int id;
    struct cost* ore_robot;
    struct cost* clay_robot;
    struct cost* obsidian_robot;    
    struct cost* geode_robot;    
};

struct blueprints {
    struct blueprint* prints;
    int count;
};

void parse_cost(struct cost* cost, char* entry){
    char* type = malloc(8*sizeof(char));
    int count = 0;
    sscanf(entry, "%d %s", &count, type);
    if (strcmp(type, "ore") == 0){
        cost->ore = count;
    } else if (strcmp(type, "clay")  == 0){
        cost->clay = count;
    } else if (strcmp(type, "obsidian") == 0){
        cost->obsidian = count;
    }
    char* end = strstr(entry, "and");
    if (end != NULL){
        parse_cost(cost, end+4);
    }
}

struct blueprints parse(char* input){
    struct file_contents fc = read_lines(input);
    struct blueprints bps;
    bps.count = fc.line_count;
    bps.prints = malloc(bps.count*sizeof(struct blueprint));
    for (int i=0;i<fc.line_count;i++){
        struct blueprint bp = bps.prints[i];
        char* entry = strdup(fc.lines[i]);
        char* block = strtok_r(entry, ":", &entry);   
        sscanf(block, "Blueprint %d", &bp.id);
        block = strtok_r(entry, ".", &entry);   
        char* costs = malloc(255*sizeof(char));
        sscanf(block, " Each ore robot costs %99[^.]", costs);         
        bp.ore_robot = malloc(sizeof(struct cost));
        parse_cost(bp.ore_robot, costs);
        
        block = strtok_r(entry, ".", &entry);   
        sscanf(block, " Each clay robot costs %99[^.]", costs); 
        bp.clay_robot = malloc(sizeof(struct cost));
        parse_cost(bp.clay_robot, costs);
        
        block = strtok_r(entry, ".", &entry);   
        sscanf(block, " Each obsidian robot costs %99[^.]", costs); 
        bp.obsidian_robot = malloc(sizeof(struct cost));
        parse_cost(bp.obsidian_robot, costs);
        
        block = strtok_r(entry, ".", &entry);   
        sscanf(block, " Each geode robot costs %99[^.]", costs); 
        bp.geode_robot = malloc(sizeof(struct cost));
        parse_cost(bp.geode_robot, costs);  
        bps.prints[i] = bp;
    }
    return bps;
} 

struct game {
    int ore_count;
    int clay_count;
    int obsidian_count;
    int geode_count;
    int ore_robots;
    int clay_robots;
    int obsidian_robots;
    int geode_robots;
    int rounds;
    struct blueprint* bp;
};

int robot_available(struct game* g, struct cost* robot_cost){
    if (robot_cost->obsidian <= g->obsidian_count &&
        robot_cost->clay <= g->clay_count &&
        robot_cost->ore <= g->ore_count){
        return 1;
    }
    return 0;
}

void pay_for_robot(struct game* g, struct cost* robot_cost){
    g->clay_count = g->clay_count-robot_cost->clay;
    g->ore_count = g->ore_count-robot_cost->ore;
    g->obsidian_count = g->obsidian_count - robot_cost->obsidian;                                  
}
int play_game(struct game* g, int* best_geode_count);

int play_round(struct game* g, struct cost* robot_cost, int* robot_count, int* best_geode_count){
    if (robot_cost != NULL){
        pay_for_robot(g, robot_cost);
    }
    g->geode_count += g->geode_robots;
    g->obsidian_count += g->obsidian_robots;
    g->clay_count += g->clay_robots;
    g->ore_count += g->ore_robots;
    if (robot_count != NULL){
        *robot_count += 1; 
    } 
    g->rounds -= 1;

    if (g->rounds == 1 && g->geode_robots == 0){
        return -1;
    }
    
    if (best_geode_count[g->rounds] <= g->geode_count){
        best_geode_count[g->rounds] = g->geode_count;
        return play_game(g, best_geode_count); 
    } 
    return -1;
}

int play_game(struct game* g, int* best_geode_count){
    if (g->rounds == 0){
        return g->geode_count;
    }
    
    int max_geodes = 0;
    int geode_robot_available = robot_available(g, g->bp->geode_robot);
    if (geode_robot_available){
        struct game updated_game;
        memcpy(&updated_game, g, sizeof(struct game));
        int geode_count = play_round(&updated_game, g->bp->geode_robot, &updated_game.geode_robots, best_geode_count);
        if (geode_count > max_geodes){
            max_geodes = geode_count;
        }
    }

    int obsidian_robot_available = robot_available(g, g->bp->obsidian_robot);
    if (obsidian_robot_available){
        struct game updated_game;

        memcpy(&updated_game, g, sizeof(struct game));
        int geode_count = play_round(&updated_game, g->bp->obsidian_robot, &updated_game.obsidian_robots, best_geode_count);
        if (geode_count > max_geodes){
            max_geodes = geode_count;
        }
    }

    int clay_robot_available = robot_available(g, g->bp->clay_robot);
    if (clay_robot_available){
        struct game updated_game;
        memcpy(&updated_game, g, sizeof(struct game));
        int geode_count = play_round(&updated_game, g->bp->clay_robot, &updated_game.clay_robots, best_geode_count);
        if (geode_count > max_geodes){
            max_geodes = geode_count;
        }
    }

    int ore_robot_available = robot_available(g, g->bp->ore_robot);
    if (ore_robot_available){
        struct game updated_game;
        memcpy(&updated_game, g, sizeof(struct game));
        int geode_count = play_round(&updated_game, g->bp->ore_robot, &updated_game.ore_robots, best_geode_count);
        if (geode_count > max_geodes){
            max_geodes = geode_count;
        }
    }
    int geode_count = play_round(g, NULL, NULL,best_geode_count);
    if (geode_count > max_geodes){
        max_geodes = geode_count;
    }
    return max_geodes;
}

int main(){
    struct blueprints bps = parse("input");
    int total = 0;
    for (int n=0;n<bps.count;n++){
        struct game g;
        g.ore_robots = 1;
        g.clay_robots = 0;
        g.obsidian_robots = 0;
        g.geode_robots = 0;
        g.ore_count = 0;
        g.clay_count = 0;
        g.obsidian_count = 0;
        g.geode_count = 0;
        g.rounds = 24;
        g.bp = &bps.prints[n];    
        int best_geode_count[24];
        for (int i=0;i<24;i++){
            best_geode_count[i] = 0;
        }
        int result = play_game(&g, (int *)best_geode_count);
        total = total + (result * g.bp->id);
        printf("Print %i has %i geodes\n", g.bp->id, result);
    }    
    printf("Part 1 - %i\n", total);

    total = 0;
    for (int n=0;n<3;n++){
        struct game g;
        g.ore_robots = 1;
        g.clay_robots = 0;
        g.obsidian_robots = 0;
        g.geode_robots = 0;
        g.ore_count = 0;
        g.clay_count = 0;
        g.obsidian_count = 0;
        g.geode_count = 0;
        g.rounds = 32;
        g.bp = &bps.prints[n];    
        int best_geode_count[32];
        for (int i=0;i<32;i++){
            best_geode_count[i] = 0;
        }
        int result = play_game(&g, (int *)best_geode_count);
        total = total * result;
        printf("Print %i has %i geodes\n", g.bp->id, result);
    }
    printf("Part 2 - %i\n", total);

}