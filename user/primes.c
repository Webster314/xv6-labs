#include "kernel/types.h"
#include "user/user.h"

void
prime(int p[2], int pm){
    printf("prime %d\n", pm);
    close(p[1]);
    int np;
    while(read(p[0], &np, sizeof(int)) != 0){
        if(np % pm != 0){
            int pp[2];
            pipe(pp);
            int pid = fork();
            if(pid > 0){
                close(pp[0]);
                while(read(p[0], &np, sizeof(int)) != 0){
                    if(np % pm != 0){
                        write(pp[1], &np, sizeof(int));
                    }
                }
                close(p[0]);
                close(pp[1]);
                while(wait(0) != -1);
                exit(0);
            }else if(pid == 0){
                prime(pp, np);
                exit(0);
            }else{
                fprintf(2, "fork: failed\n");
                exit(1);
            }
        }
    }
}

void
primepipe(int ub){
    printf("prime 2\n");
    for(int i = 2; i <= ub; i++){
        if(i % 2 != 0){
            int p[2];
            pipe(p);
            int pid = fork();
            if(pid > 0){
                close(p[0]);
                for(i = i + 1; i <= ub; i++){
                    if(i % 2 != 0){
                        write(p[1], &i, sizeof(int));
                    }
                }
                close(p[1]);
                // wait until the entire pipeline terminates
                while(wait(0) != -1);
            }else if(pid == 0){
                prime(p, i);
                exit(0);
            }else{
                fprintf(2, "fork: failed\n");
                exit(1);
            }
        }
    }   
}

int main(int argc, char * argv[]){
    int ub = argc == 1 ? 35 : atoi(argv[1]);    //ub_max, 46
    primepipe(ub);
    exit(0);
}