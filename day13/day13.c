
#include "../util/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct list {
    struct entry* entries;
    int len;
};

struct entry {
    enum { num, list } kind;
    union {
        int i;
        struct list list;
    } item;
};

struct list parse(char* line){
    int len = strlen(line);
    int stack_size = 100;
    struct struct_stack stack;
    struct_stack_new(&stack, stack_size);
    struct list* curr = NULL;
    struct list* last;
    int curr_int = -1;
    struct entry* curr_entry = NULL;
            
    for (int i=0;i<len;i++){
        if (line[i] == '['){
            // start a new list
            if (curr != NULL){
                // save current list
                struct_stack_push(&stack, curr);
             }
            // create new list
            curr = malloc(sizeof(struct list));
            curr->entries = malloc(100*sizeof(struct entry));
            curr->len = 0;
        } else if (line[i] == ']'){
            // done!
            if (curr_entry != NULL){
                curr->entries[curr->len] = *curr_entry;
                curr->len += 1;
            }   
            if (struct_stack_is_empty(&stack)){
                return *curr;
            } else {            
                // create an entry which represents
                // the current list
                curr_entry = malloc(sizeof(struct entry));
                curr_entry->kind = list;
                curr_entry->item.list = *curr;
                // add it to the previous item on the stack
                curr = (struct list *)struct_stack_pop(&stack);
                curr->entries[curr->len] = *curr_entry;
                curr->len += 1;
                curr_entry = NULL;
            }
        } else if (line[i] == ','){
            if (curr_entry != NULL){
                curr->entries[curr->len] = *curr_entry;
                curr->len += 1;
                curr_entry = NULL;
            }
        } else {
            if (curr_entry == NULL){
                curr_entry = malloc(sizeof(struct entry));
                curr_entry->kind = num;
                curr_entry->item.i = line[i] - '0';
            } else {
                curr_entry->item.i = (curr_entry->item.i*10)+ (line[i] - '0');
            } 
        }
    }
    return *curr;
}

struct list num_to_list(int n) {
    struct list num_as_list;
    num_as_list.entries = malloc(sizeof(struct entry));
    num_as_list.len = 1;
    struct entry e;
    e.kind = num;
    e.item.i = n;
    num_as_list.entries[0] = e;
    return num_as_list;    
}

int compare_list(const void * l, const void * r){
    struct list left = *(struct list *)l;
    struct list right = *(struct list *)r;
    if (left.len == 0){
        if (right.len > 0){
            return -1;
        } else {
            return 0;
        }
    } else if (right.len == 0){
        return 1;
    }
    for (int i=0;i<left.len;i++){
        if (i>=right.len) {
            return 1;
        }
        struct entry left_entry = left.entries[i];
        struct entry right_entry = right.entries[i];
        int result = 0;
        if (left_entry.kind == num && right_entry.kind == num) {
            if (left_entry.item.i != right_entry.item.i){
                if (left_entry.item.i > right_entry.item.i){
                    return 1;
                } else {
                    return -1;
                }
            }
        } else if (left_entry.kind == num){
            struct list left = num_to_list(left_entry.item.i);
            if ( (result = compare_list(&left, &right_entry.item.list)) != 0){
                return result;
            };
        } else if (right_entry.kind == num) {
            struct list right = num_to_list(right_entry.item.i);
            if ( (result = compare_list(&left_entry.item.list,&right)) != 0){
                return result;
            }
        } else {
            // both are lists
            if ((result = compare_list(&left_entry.item.list, &right_entry.item.list)) != 0){
                return result;
            }
        }
    }
    if (left.len == right.len){
        return 0;
    }
    return -1;
}

int main(){
    struct file_contents fc = read_lines("input");
    int list_count = 2*((fc.line_count+1)/3);
    // include space for the dividers
    struct list* lists = malloc((list_count+ 2) * sizeof(struct list));
    int n=0;
    for (int i=0;i<fc.line_count;i=i+3){
        lists[n++] = parse(fc.lines[i]);
        lists[n++] = parse(fc.lines[i+1]);
    }
    
    // part 1
    int part1 = 0;
    for (int i=0;i<list_count/2;i++){
        int result = compare_list(&lists[2*i], &lists[(2*i)+1]);
        if (result == -1){
            part1 = part1+i+1;
        }
    }
    printf("Part 1 - %i\n", part1);
    struct list divider1 = parse("[[2]]");
    struct list divider2 = parse("[[6]]");
    lists[list_count] = divider1;
    lists[list_count+1] = divider2;
    qsort(lists, list_count+2, sizeof(struct list), compare_list); 
    int part2 = 1;
    for (int i=0;i<list_count+2;i++){
        if (compare_list(&lists[i], &divider1) == 0 || compare_list(&lists[i], &divider2) == 0){
            part2 = part2*(i+1);
        }
    }
    printf("Part 2 - %i\n", part2);       
}