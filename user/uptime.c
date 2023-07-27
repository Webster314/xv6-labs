#include "kernel/types.h"
#include "user/user.h"

int main(void){
    int now = uptime();
    printf("%d\n", now);
    exit(0);
}