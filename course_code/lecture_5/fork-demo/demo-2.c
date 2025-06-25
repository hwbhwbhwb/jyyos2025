#include <unistd.h>
#include <stdio.h>

int main() {
    for (int i = 0; i < 2; i++) {
        fork();
        printf("Hello\n");
    }
}

/*程序是状态机的静态描述，printf似乎可以等价写为：
if (fork() == 0) {
    printf("Hello\n");
} else {
    printf("Hello\n");
}
*/

/*神奇的输出：
hwb@hwb-virtual-machine:~/jyyos/os2025/course_code/lecture_5/fork-demo$ ./demo-2
Hello
Hello
Hello
Hello
Hello
Hello
hwb@hwb-virtual-machine:~/jyyos/os2025/course_code/lecture_5/fork-demo$ ./demo-2 | wc -l
8
我们不禁要问为什么呢？
Answer:实际上printf有这样的特性：在输出到管道的时候并不是立刻输出，而是先缓冲起来，等到缓冲区满了或者遇到换行符才会输出。
由于每次fork都会创建一个新的进程，所以每个进程都会有自己的缓冲区。
设总共最终有4个进程，A1~A4。
输出到终端时：
A1————>A1、A2,A1、A2各执行一次hello
A2————>A2、A3各执行一次hello
A1————>A1、A4各执行一次hello,共6次hello

输出到管道时的不同：
A1————>A1、A2,原来A1的缓冲区为NULL,A1、A2各执行一次hello
A2————>A2执行一次hello,同时A3复制A2的缓冲区，此时A3的缓冲区也为hello，再执行一次hello，
为hello\nhello,所以6+2=8
注意：本程序实际上等效于fork-printf1.c,而不是fork-printf.c。
*/