#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../util/util.h"

long long to_decimal(char* snafu){
    long long total = 0;
    int len = strlen(snafu);
    for (int i=0;i<len;i++){
        long long n = pow(5,i);
        switch(snafu[len - 1 - i]){
            case '0': break;
            case '1': total += n; break;
            case '2': total += 2*n; break;
            case '-': total -= n; break;
            case '=': total -= 2*n; break;
        }    
    }
    return total;
}

char* to_snafu(long long decimal){
    char* snafu = malloc(20*sizeof(char));
    int index = 0;
    for (int n = 20; n >= 0; n--){
        long factor = pow(5,n);
        long absd = labs(decimal);
        if (absd < 0.5*factor && index>0){
            snafu[index++] = '0';
        } else if (absd < 1.5*factor && absd > 0.5*factor) {
            if (decimal < 0){
                snafu[index++] = '-';
                decimal += factor;                
            } else {
                snafu[index++] = '1';
                decimal -= factor;                
            }
        } else if (absd > 1.5*factor){
            if (decimal < 0){ 
                snafu[index++] = '=';
                decimal += 2*factor;                
            } else {
                snafu[index++] = '2';
                decimal -= 2*factor;                
            }
        } 
    }
    snafu[index] = 0;
    return snafu;
}

int main(){
    struct file_contents fc = read_lines("input");
    long long total = 0;
    for (int i=0;i<fc.line_count;i++){
        total = total + to_decimal(fc.lines[i]);
    }
    printf("%lli - %s\n", total, to_snafu(total));
}