#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

#define SIZE 128
#define FILEADDRESS "file1.txt"
using namespace std;

int count_min_max_ave[11][4];

int str_length(char str[]) {
    int count; 
    for (count = 0; str[count] != '\0'; ++count);
    return count+1; 
}

void make_key(char* addr)
{   
    char line[SIZE] = " ";
    FILE* fp = fopen(addr, "r" );
    FILE* fw = fopen(FILEADDRESS, "a+");
    char ch;
    char sp = ' ';
    int x = 0;
    while ((ch = fgetc(fp)) != EOF){
        printf("%c", ch);
        if(ch == '\n'){
            fputc('\n', fw);
            x = 0;
        }
        if(ch == ','){
            x = 1;
            fputc(' ', fw);
            continue;
        }
        if (x == 1){
            fputc(ch, fw);
            strncat(line, &ch, 1);
        }
    }
    //printf("\nline:%s\n", line);
    //fputs(line, fw);
    //fputs("\n", fw);
    fclose(fw);
    fclose(fp);
}

int main(int argc, char*argv[])
{
    char *addr = (char*)malloc(SIZE);
    int unnamed_pipe_fd = atoi(argv[0]);
    if(read(unnamed_pipe_fd, addr, SIZE) == -1) 
        printf("Error in reading\n");

    printf("address is: %s\n", addr);
    make_key(addr);

    string final = "";    

    char extract_file_name[] = "file1.txt";
    //char key[1024]; 
    //strcpy(key, final.c_str());
    printf("length: %d\n", str_length(extract_file_name));
    int named_pipe_fd = atoi(argv[1]);
    write(named_pipe_fd, FILEADDRESS, 10);

    return 0;
}