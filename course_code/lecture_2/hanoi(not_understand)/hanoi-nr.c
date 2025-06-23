#include <stdio.h>
#include <assert.h>

struct Frame
{
    // 每个帧都有一个程序计数器，用于跟踪其下一条待执行的语句。
    int pc;

    // 帧的内部状态，包括参数和局部变量（如果有）。
    // 参数：
    int n;
    char from, to, via;

    // 局部变量：
    int c1, c2;
};

typedef struct Frame Frame;

int hanoi(int n, char from, char to, char via)
{
    // 定义一个栈，用于模拟递归调用。
    Frame stk[64];
    Frame *top = stk - 1;//栈顶指针，初始时指向栈底。

    // 函数调用：将一个新的帧（PC=0）压入栈。
#define call(...) ({ *(++top) = (Frame){.pc = 0, __VA_ARGS__}; })

    // 函数返回：弹出栈顶帧。
#define ret(val) ({ top--; retval = (val); })

    // 保存最后一次函数返回的值。注意我们只需要一个 retval。
    int retval = 0;

    // 初始调用递归函数。
    call(n, from, to, via);

    while (1)
    {
        // 获取栈顶帧。
        Frame *f = top;
        if (top < stk)
        {
            // 如果栈顶帧不存在，说明任务完成。
            break;
        }

        // 跳转可能会改变默认的下一条指令。
        int next_pc = f->pc + 1;

        // 单步执行。

        // 从当前帧中提取参数。（通常不建议重复使用变量名，但这里为了可读性这样做了。）
        int n = f->n, from = f->from, to = f->to, via = f->via;

        switch (f->pc)
        {
        case 0:
            // 如果只有一个盘子，直接移动并返回。
            if (n == 1)
            {
                printf("%c -> %c\n", from, to);
                ret(1);
            }
            break;
        case 1:
            // 调用递归函数，将 n-1 个盘子从 from 移动到 via。
            call(n - 1, from, via, to);
            break;
        case 2:
            // 保存返回值。
            f->c1 = retval;
            break;
        case 3:
            // 移动一个盘子。
            call(1, from, to, via);
            break;
        case 4:
            // 调用递归函数，将 n-1 个盘子从 via 移动到 to。
            call(n - 1, via, to, from);
            break;
        case 5:
            // 保存返回值。
            f->c2 = retval;
            break;
        case 6:
            // 返回总步数。
            ret(f->c1 + f->c2 + 1);
            break;
        default:
            // 如果程序计数器超出范围，触发断言。
            assert(0);
        }

        // 更新程序计数器。
        f->pc = next_pc;
    }

    return retval;
}
