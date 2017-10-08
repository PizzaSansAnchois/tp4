#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 1000000000

int main(int argc, char** argv){
    unsigned int seed = 0;
    unsigned long long num_number = 0;
    if (argc < 2){
        printf("Usage: %s <number of number> <seed>\n", argv[0]);
        exit(0);
    }else if (argc == 2){
        seed = time(NULL);
    }else if (argc == 3){
        seed = atoi(argv[2]);
    }
    num_number = atoll(argv[1]);

    srand(seed);
    FILE* fd;
    char filename[] = "out.txt";

    if ((fd = fopen(filename, "w")) == NULL){
        perror("error opening file for writting");
        exit(1);
    }

    for (int i = 0; i < num_number; i++){
        fprintf(fd, "%d\n", rand() % MAX_VAL);
    }

    fclose(fd);

    return 0;
}
