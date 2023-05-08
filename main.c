#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define NUM 11
#define FIFO_ADDRESS "/tmp/myfifo"
#define SIZE 512
#define READ 0
#define WRITE 1

int count_file(const char* direction)
{
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    dirp = opendir(direction); 
    while ((entry = readdir(dirp)) != NULL){
        if (entry->d_type == DT_REG)
            file_count++;
    }
    closedir(dirp);
    return file_count;
}

int str_length(char str[]) {
    int count; 
    for (count = 0; str[count] != '\0'; ++count);
    return count+1; 
}

int main(int argc, char* argv[]){
    printf("ok");
    char poses[SIZE];
    char temp[SIZE];
    char position_list[9][3];
    char address[SIZE];
    char *addr = (char*)malloc(SIZE);
    char eng_addr[] = "clubs/ENG";
    char eng_files[11][SIZE] = {" ", "clubs/positions.csv" ,"clubs/ENG/liverpool.csv", "clubs/ENG/manchester_city.csv",
    "clubs/ESP/barcelona.csv", "clubs/ESP/real_madrid.csv", "clubs/FRA/marseille.csv",
    "clubs/FRA/paris_saint_germain.csv", "clubs/GER/bayern_munich.csv",
    "clubs/GER/borussia_dortmund.csv" ," "};
 
    FILE* fp = fopen("./clubs/positions.csv", "r" );
    char ch; int i = 0;
    while ((ch = fgetc(fp)) != EOF){
        poses[i] = ch;
        i++;
    }
    fclose(fp);
    printf("All positions: \n");
    printf("%s \n", poses);

    int index = 0 , k = 0;
    for(int i = 0 ; i < sizeof(poses)/sizeof(poses[1]) ; i+=3){
        position_list[k][0] = poses[i];
        position_list[k][1] = poses[i+1];
        k++;
    }

    // for (int g = 0 ; g < 9 ; g++){
    //     printf("%c%c\n", position_list[g][0],position_list[g][1]);
    // }

    int eng_count = count_file(eng_addr);
    
    //unnamed pipe
    int fd[2];
    if(pipe(fd) == -1)
        printf("An error occured!\n");
    
    //named pipe
    int fifo = mkfifo(FIFO_ADDRESS, 0777);
    int pipe_fd = open(FIFO_ADDRESS, O_RDWR);

    //map
    for(int i = 1; i < 11; i++){
        pid_t id = fork();
        if(id == 0)
        {
            char unnamed_pipe[2], named_pipe[3];
            sprintf(unnamed_pipe, "%d", fd[READ]);
            sprintf(named_pipe, "%d", pipe_fd);
            char *args[]={unnamed_pipe, named_pipe,poses, NULL};
		    execv("./map.out", args);
        }
        else
        {
            if(write(fd[WRITE], eng_files[i], str_length(eng_files[i])) == -1)
                printf("Error in writing: %s\n", strerror(errno));                
            wait(NULL);
        }   
        close(id);
        //printf("i : %d\n", i);
    }

    pid_t child_pid;
    int s = 0;
    while ((child_pid = wait(&s)) > 0);

    //Reduce
    pid_t id2 = fork();
    if(id2 == 0)
    {
        char unnamed_pipe[2], named_pipe[3];
        sprintf(unnamed_pipe, "%d", fd[WRITE]);
        sprintf(named_pipe, "%d", pipe_fd);
        char *args[]={unnamed_pipe, named_pipe, NULL};
	    execvp("./reduce.out", args);
    }
    else
    {
        pid_t child_pid;
        int s = 0;
        while ((child_pid = wait(&s)) > 0);
    }   
    close(fd[WRITE]);

    //read data file address
    char * line = NULL;
    size_t len = 0;
    char text_[SIZE];
    if(read(fd[READ], text_, SIZE) == -1) 
        printf("Error in reading\n");
    
    char order[SIZE];
    printf("what positions do you wanna know about?\n$ ");
    fgets(order, SIZE, stdin);
    char* token = strtok(order, " ");
    while (token != NULL) {
        printf("%s\n", token);
        // FILE* fp = fopen("text_", "r");
        // char chunk[128];
        // for (int i = 0 ; i < NUM ; i++){
        //     getline(&line, &len, fp);
        //     printf("%s", line);
        // }
        token = strtok(NULL, " ");
        // fclose(fp);
    }

    close(fd[READ]);
    close(pipe_fd);
    printf("this is the end!!\n");
    return 0;
}