#include "../util/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum file_type { file, dir };

struct fakefile {
    char* name;
    enum file_type ftype;
    long size;
    struct fakefile* parent; 
    struct fakefile** children;
    int child_count;    
};

long size(struct fakefile* f) {
    if (f->size < 0){
        long total = 0;
        for (int i=0; i<f->child_count; i++){
            total+=size(f->children[i]); 
        }
        f->size = total;
    }
    return f->size;
}

long total_of_dirs_less_then_size(struct fakefile* f, int max){
    int this_dir_total=0;
    for (int i=0;i<f->child_count;i++){
        this_dir_total += size(f->children[i]);
    }

    int results = 0;
    if (this_dir_total < max){
        results = this_dir_total;
    }
    for (int i=0;i<f->child_count;i++){
        if (f->children[i]->ftype == dir){
            results += total_of_dirs_less_then_size(f->children[i], max);
        } 
    }
    return results;
}

struct filter_results {
    struct fakefile** results;
    int count;
};

void filter_for_dirs_by_size(struct fakefile* f, int required, struct filter_results* results){
    // find any directories which are greater then required size
    if (f->ftype == file) return;
    if (size(f) < required) return;
    results->results[results->count] = f;
    results->count += 1;

    for (int i=0;i<f->child_count;i++){
        filter_for_dirs_by_size(f->children[i], required, results);
    }
}

int main() {
    struct file_contents fc = read_lines("input");
    struct fakefile* curdir = NULL;
    struct fakefile* root;
    for (int i=0;i<fc.line_count;i++){
        if (strcmp(fc.lines[i], "$ ls")==0){
            // read next lines until we get another command
            int file_count = 0;
            for (int j=1;i+j<fc.line_count;j++){
                if (fc.lines[i+j][0] == '$'){
                    break;
                }
                file_count = j;                
            }
            curdir->child_count = file_count;
            curdir->children=malloc(file_count*sizeof(struct fakefile*));
            for (int k=0;k<file_count;k++){
                char* entry = strdup(fc.lines[i+1+k]);
                struct fakefile* child = malloc(sizeof(struct fakefile));
                if (strncmp(entry, "dir", 3) == 0){
                    // add a directory
                    child->ftype = dir;
                    strtok_r(entry, " ", &entry);
                    child->size = -1;
                    child->name = strtok_r(entry, " ", &entry);
                } else {
                    child->ftype = file;
                    child->size = atoi(strtok_r(entry, " ", &entry));
                    child->name = strtok_r(entry, " ", &entry);
                }
                // add a file
                child->parent = curdir;
                curdir->children[k] = child;
            }
            i=i+file_count;
        } else if (strncmp(fc.lines[i], "$ cd ", 5) == 0){
            char* entry = strdup(fc.lines[i]);
            strtok_r(entry, " ", &entry);
            strtok_r(entry, " ", &entry);
            char* name = strtok_r(entry, " ", &entry);
            if (curdir != NULL){
                if (strncmp(name, "..", 2)==0){
                    if (curdir->parent){
                        curdir=curdir->parent;
                    }
                } else {
                    for (int j = 0; j < curdir->child_count; j++){
                        if (strcmp(curdir->children[j]->name, name) == 0){
                            curdir=curdir->children[j];
                        }
                    }
                }
            } else {
                struct fakefile* base = malloc(sizeof(struct fakefile));
                base->ftype = dir;
                base->name = name;
                base->size = -1;
                base->parent = NULL;
                curdir = base;

                root=base;
            }
        }
    } 
    printf("Part 1 - %li\n", total_of_dirs_less_then_size(root, 100000));

    int MAX=70000000;
    int spare = MAX - size(root);
    int required = 30000000 - spare;
    struct filter_results results;
    results.results = malloc(1000*sizeof(size_t));
    results.count = 0;
    filter_for_dirs_by_size(root, required, &results);
    int min = size(results.results[0]);
    struct fakefile* result = results.results[0];
    for (int i=1;i<results.count;i++){
        int dir_size = size(results.results[i]);
        if (min > dir_size){
            min = dir_size;
            result = results.results[i];
        }
    }
    printf("Part 2 - %i\n", min);
}