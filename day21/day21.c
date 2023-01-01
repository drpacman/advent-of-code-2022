#include "../util/util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

enum type {unresolved, resolved};

struct expr {
    union {
        char* unresolved_id;
        long resolved_value;
    } a;
    enum type value_a;
    char operator;
    union {
        char* unresolved_id;
        long resolved_value;
    } b;
    enum type value_b;    
};

struct entry {
    char* id;
    union {
        struct expr unresolved_expr;
        long resolved_value;
    } rhs;
    enum type rhs_type;
};

struct entries {
    struct entry* entries;
    int len;
};

struct entry* find_entry(char* id, struct entries e){
    for (int i=0;i<e.len;i++){
        if (strcmp(e.entries[i].id, id) == 0){
            return &e.entries[i];
        }
    }
    return NULL;
}

long resolve(char* id, struct entries entries){
    struct entry* entry = find_entry(id, entries);
    assert(entry != NULL);
    if (entry->rhs_type == resolved){
        return entry->rhs.resolved_value;
    } 
    struct expr e = entry->rhs.unresolved_expr;
    long a, b;
    if (e.value_a == resolved){
        a = e.a.resolved_value;
    } else {
        a = resolve(e.a.unresolved_id, entries);
    } 
    if (e.value_b == resolved) {
        b = e.b.resolved_value;
    } else {
        b = resolve(e.b.unresolved_id, entries);
    } 
        
    long result;
    switch (e.operator){
        case '+': 
            result = a + b;
            break;
        case '-': 
            result = a - b;
            break;
        case '*': 
            result = a * b;
            break;
        case '/': 
            result = a / b;
            break;
        case '=': 
            result = a == b;
            break;
    }
    entry->rhs.resolved_value = result;
    entry->rhs_type = resolved;
    return result;
}

struct entries read_entries(char* filename){
    struct file_contents fc = read_lines(filename);
    struct entries entries;
    entries.entries = malloc(fc.line_count*sizeof(struct entry));
    entries.len = fc.line_count;
    for (int i=0;i<fc.line_count;i++){
        struct entry* entry = malloc(sizeof(struct entry));
        char* expr = malloc(11*sizeof(char));
        entry->id = malloc(4*sizeof(char));
        sscanf(fc.lines[i], "%4s: %99[^\n]", entry->id, expr);
        if (strlen(expr) < 11){
            entry->rhs.resolved_value = atol(expr);
            entry->rhs_type = resolved;            
        } else {
            struct expr* e = malloc(sizeof(struct expr));
            e->a.unresolved_id=malloc(4*sizeof(char));
            e->value_a = unresolved;
            e->b.unresolved_id=malloc(4*sizeof(char));            
            e->value_b = unresolved;
            sscanf(expr, "%4s %c %4s", e->a.unresolved_id, &e->operator, e->b.unresolved_id);
            entry->rhs.unresolved_expr = *e;
            entry->rhs_type = unresolved;            
        }
        entries.entries[i] = *entry;
    }
    return entries;
}

char* find_reference(char* id, struct entries e){
    for (int i=0;i<e.len;i++){
        if (e.entries[i].rhs_type == unresolved){
            if (strcmp(id, e.entries[i].rhs.unresolved_expr.a.unresolved_id) == 0 ||
                strcmp(id, e.entries[i].rhs.unresolved_expr.b.unresolved_id) == 0){
                return e.entries[i].id;
            }
        }
    }
    return NULL;
}

int main(){
    struct entries entries=read_entries("input");
    printf("Part 1 - %li\n", resolve("root", entries));
    // with the assumption that each identifier only appears once...
    // build a look up of where each identifier is used
    entries=read_entries("input");
    char* id = "humn";
    char** expressions = malloc(entries.len*sizeof(char *));
    expressions[0] = id;
    int index = 1;
    while (strcmp(id, "root") != 0){
        id = find_reference(id, entries);
        expressions[index++] = id;    
    }
    struct entry* root = find_entry("root", entries);
    root->rhs.unresolved_expr.operator = '=';
    long value = 0;
    for (int i=index-1;i>0;i--){
        char* referer = expressions[i];
        // get the equation which references it
        struct entry* e = find_entry(referer, entries);
        // find which side of the expression does not reference the unknown value and resolve it
        // update the value with 
        if (strcmp(expressions[i-1], e->rhs.unresolved_expr.a.unresolved_id) == 0){
            long other = resolve(e->rhs.unresolved_expr.b.unresolved_id, entries);
            switch (e->rhs.unresolved_expr.operator) {
                case '=': value = other; break;
                case '+': value = value - other; break;
                case '-': value = value + other; break;                    
                case '*': value = value / other; break;
                case '/': value = value * other; break;
            }
            
        } else {
            long other = resolve(e->rhs.unresolved_expr.a.unresolved_id, entries);
            switch (e->rhs.unresolved_expr.operator) {
                case '=': value = other; break;
                case '+': value = value - other; break;
                case '-': value = other - value; break;                    
                case '*': value = value / other; break;
                case '/': value = other / value; break;
            }
        }
    }
    printf("Part 2 - %li\n", value);
}