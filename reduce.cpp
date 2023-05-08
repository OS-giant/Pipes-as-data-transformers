#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#define SIZE 1024
#define HALFSIZE 128
#define NUM 20

using namespace std;

char positions[NUM][HALFSIZE];
int  attr[NUM][NUM] = {{0}};
int count[NUM];
int min[NUM];
int max[NUM];
int ave[NUM];

int find_pos_index(char* token) {
    int index = 0;
    for (int i = 0 ; i < NUM ; i++){
        if (token[0] == positions[i][0] && token[1] == positions[i][1]){
            index = i;
            break;
        }
    }
    return index;
}

int max_counter(int *ages){
    int max;
    for(int i = 0 ; i < NUM ; i++){
        if (i == 0){
            max = ages[i];
        } else if (max < ages[i]){
            max = ages[i];
        }
    }

    return max;
}

int min_counter(int *ages){
    int min;
    for(int i = 0 ; i < NUM ; i++){
        if (i == 0){
            min = ages[i];
        } else if (min > ages[i] && ages[i] != 0){
            min = ages[i];
        }
    }

    return min;
}

float ave_counter(int *ages){
    float ave;
    int sum = 0, num = 0;
    for(int i = 0 ; i < NUM ; i++){
        if(ages[i] == 0){
            continue;
        } else {
            sum+= ages[i];
            num++;
        }
    }

    if (num == 0) num = 1;

    ave = sum / num;
    return ave;
}

int simple_counter(int *ages){
    int num = 0;
    for(int i = 0 ; i < NUM ; i++){
        if (ages[i] != 0){
            num++;
        } else {
            break;
        }
    }

    return num;
}

void dedicate_data(int index, char* data){
    for (int i = 0 ; i < NUM ; i++){
        if(attr[index][i] == 0){
            attr[index][i] = atoi(data);
            break;
        } else {
            continue;
        }
    }
}

void reduce_each_line(char* addr){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int k = 0;
    char age_str[] = "  ";
    char line_copy[SIZE];
    
    int index = 0;
    FILE* fp = fopen(addr, "r" );
    read = getline(&line, &len, fp);
    while ((read = getline(&line, &len, fp)) != -1) {
        age_str[0] = line[4];
        age_str[1] = line[5];
        line[100] = ' ';
        char* token = strtok(line, " ");
        int index = find_pos_index(token);
        dedicate_data(index, age_str);
    }

    fclose(fp);
}

void record_final_file(){
    char snum1[5];
    char snum2[5];
    char snum3[5];
    char snum4[5];
    FILE* f = fopen("data.txt", "a");
    for (int i = 0 ; i < 11 ; i++){
        int count = simple_counter(attr[i]);
        float ave = ave_counter(attr[i]);
        int min = min_counter(attr[i]);
        int max = max_counter(attr[i]);
        
        fputc(positions[i][0], f);
        fputc(positions[i][1], f);
        fprintf(f, " count : %d average : %.1f min : %d max : %d\n", count, ave, min, max);
    }

    fclose(f);
    // printf("\nnumber is : %d\n", simple_counter(attr[0]));
    // printf("\nave is : %d\n", ave_counter(attr[0]));
    // printf("\nmin is : %d\n", min_counter(attr[0]));
    // printf("\nmax is : %d", max_counter(attr[0]));
}

void reduce_positions(char* addr){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int k = 0;
    
    int index = 0;
    FILE* fp = fopen(addr, "r" );

    while ((read = getline(&line, &len, fp)) != -1) {
        if(index == 0){
            char* token = strtok(line, " ");
            while( token != NULL ) {
                positions[k][0] = token[0];
                positions[k][1] = token[1];
                k++;
                token = strtok(NULL, " ");
            }
        }        
        index++;
        fclose(fp);
    }
}

int main(int argc, char*argv[])
{
    char text_[SIZE];
    string final;
    char str[] = "data.txt";

    int named_pipe_fd = atoi(argv[1]);
    if(read(named_pipe_fd, text_, SIZE) == -1) 
        printf("Error in reading \n");

    reduce_positions(text_);
    reduce_each_line(text_);
    record_final_file();

    // for (int i = 0 ; i < 13 ; i++){
    //     printf("\npost: %s\n", positions[i]);
    //     for(int j = 0 ; j < NUM ; j++){
    //         printf("%d,", attr[i][j]);
    //     }
    // }

    int unnamed_pipe_fd = atoi(argv[0]);
    write(unnamed_pipe_fd, str, 9);
    return 0;
}