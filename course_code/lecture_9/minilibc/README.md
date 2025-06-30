**minilibc**: 一个用于教育目的的 C 标准库的简化实现 (claude-3.7-sonnet Cursor IDE)，展示了 C 库的核心组件，包括程序的启动和结束、基本的内存管理、字符串操作以及简化的 I/O 功能。该库实现了 `_start` 函数以设置环境，并提供了简单的 `malloc`, `free`, `strlen` 和 `strcpy`。构建该库只需运行 `make` 命令，生成静态库和测试程序。

编译流程：
-nostdlib: 不链接标准C库，因为我们要实现自己的库
-fno-builtin: 禁用编译器内置函数优化（如内置的strlen等）
-fno-stack-protector: 禁用栈保护，避免依赖标准库的栈保护代码
-no-pie: 禁用位置无关可执行文件
-fPIC: 生成位置无关代码，用于创建动态库
-ggdb: 生成GDB调试信息
-O1: 基本优化级别
-Iinclude: 指定头文件搜索路径
-c: 只编译不链接，生成目标文件