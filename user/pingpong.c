#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int childToParent[2];
    int parentToChild[2]; 
    pipe(childToParent);
    pipe(parentToChild);
    int pid = fork();
    if (0 == pid) {
        // child process
        close(parentToChild[1]);
        // 对于read和write，函数参数一律用char[]数组，数组大小给得足够大就行了
        char tmp[2];
        int n = read(parentToChild[0], tmp, 1);
        close(parentToChild[0]);
        fprintf(1, "%d: received ping\n",  getpid());
        
        close(childToParent[0]);
        if (write(childToParent[1], tmp, n) != n) {
            fprintf(2, "write error\n");
            exit(1);
        }
        close(childToParent[1]);
        exit(0);
    } else {
        // parent process
        close(parentToChild[0]);
        if (write(parentToChild[1], "1", 1) != 1) {
            fprintf(2, "write error\n");
            exit(1);
        }
        close(parentToChild[1]);
        
        // 对于read和write，函数参数一律用char[]数组，数组大小给得足够大就行了
        char tmp[2];
        close(childToParent[1]);
        read(childToParent[0], tmp, 1);
        close(childToParent[0]);
        fprintf(1, "%d: received pong\n",  getpid());
        exit(0);
    }
    
    exit(0);
}
