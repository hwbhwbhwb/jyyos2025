// Note: this file is statically linked.

// 10MB of data.
char arr[10485760];

int main() {
    // 10MB of NOP instructions.
    //这段代码的主要作用是：使用内联汇编（inline assembly）功能在程序的代码段中生成 1000 万个连续的 NOP（无操作）指令。
    asm volatile(
        ".rept 10485760;"
        "nop;"
        ".endr;"
    );
}

/*
 * This program allocates 10MB of memory and fills it with NOP instructions.
 * It serves as a simple demonstration of address space layout.
 * 如果使用execve来运行这个程序，会看到它的地址空间布局。这个进程的地址空间可能如下所示
 * 无法访问 | NOP(10MB) |无法访问 | data(10MB) ：一长串0
 * NOP = No Operation（无操作）
 */
