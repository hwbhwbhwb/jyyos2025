#include <logisim.h>
#include <stdio.h>

// Wire and registers in the circuit
wire X, Y, X1, Y1, A, B, C, D, E, F, G;
// 定义两个寄存器 b1 和 b0，分别连接到输入和输出线
reg b1 = {.in = &X1, .out = &X}; // b1 的输入连接到 X1，输出连接到 X
reg b0 = {.in = &Y1, .out = &Y}; // b0 的输入连接到 Y1，输出连接到 Y

int main() {
    CLOCK_CYCLE {
        // 1. Propagate wire values through combinatorial logic        
        // 1. 通过组合逻辑传播导线的值
        //初值为defult值，X 和 Y 的初始值为 0
        X1 = AND(NOT(X), Y);
        Y1 = NOT(OR(X, Y));
        A = D = E = NOT(Y);
        B = 1;
        C = NOT(X);
        F = Y1;
        G = X;

        // 2. Edge triggering: Lock values in the flip-flops
        //b0 的值从输入线 Y1 获取，并更新到输出线 Y。(就是一个最基本的寄存器)
        //b1 的值从输入线 X1 获取，并更新到输出线 X。
        b0.value = *b0.in;
        b1.value = *b1.in;
        *b0.out = b0.value;
        *b1.out = b1.value;

        // 3. End of a cycle; display output wire values
        //#X：将参数 X 转换为字符串。例如，如果 X 是变量 A，那么 #X 会变成 "A"。
        #define PRINT(X) printf(#X " = %d; ", X)
        PRINT(A);
        PRINT(B);
        PRINT(C);
        PRINT(D);
        PRINT(E);
        PRINT(F);
        PRINT(G);
        printf("\n");
        fflush(stdout);//将缓冲区的内容输出到标准输出流，确保所有打印的内容立即显示在终端上。
        sleep(1);//在这里，它模拟数字电路中时钟周期的间隔，使程序的输出更接近实际硬件的行为。
    }
}
