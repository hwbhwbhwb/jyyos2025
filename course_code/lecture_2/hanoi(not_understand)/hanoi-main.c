#include <stdio.h>
#include <assert.h>

int hanoi(int n, char from, char to, char via);

int main()
{

    int n = 4;
    char from = 'A';
    char to = 'B';
    char via = 'C';

    int step_count = hanoi(n, from, to, via);
    printf("\nHanoi(%d, %c, %c, %c) = %d\n",
           n, from, to, via, step_count);
}

/*这个汉诺塔程序的目的是通过非递归方式解决经典的汉诺塔问题，
并计算完成移动所需的步骤总数。汉诺塔问题要求将 n 个盘子从一个柱子
（from）移动到另一个柱子（to），使用第三个柱子（via）作为辅助。
*/