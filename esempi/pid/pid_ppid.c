#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid, ppid;

    pid = getpid(); // numero che indentifica un processo

    printf("pid processo: %d", pid);

    ppid = getppid(); // numero che identifica un gruppo di processi

    printf("ppid processo: %d", ppid);

    return 0;
}