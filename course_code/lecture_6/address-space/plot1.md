# Registers

| Register | Hex Value | Decimal Value |
|----------|-----------|---------------|
| rax | 0x0000000000000000 | 0 |
| rbx | 0x0000000000000000 | 0 |
| rcx | 0x0000000000000000 | 0 |
| rdx | 0x0000000000000000 | 0 |
| rsi | 0x0000000000000000 | 0 |
| rdi | 0x0000000000000000 | 0 |
| rbp | 0x0000000000000000 | 0 |
| rsp | 0x00007fffffffdd60 | 140737488346464 |
| r8 | 0x0000000000000000 | 0 |
| r9 | 0x0000000000000000 | 0 |
| r10 | 0x0000000000000000 | 0 |
| r11 | 0x0000000000000000 | 0 |
| r12 | 0x0000000000000000 | 0 |
| r13 | 0x0000000000000000 | 0 |
| r14 | 0x0000000000000000 | 0 |
| r15 | 0x0000000000000000 | 0 |
| rip | 0x0000000000401620 | 4199968 |
| eflags | 0x0000000000000200 | 512 |

# Memory Mappings

| Start Address | End Address | Size | Permissions | Name |
|---------------|-------------|------|--------------|------|
| 0x400000 | 0x401000 | 0x1000（4KB） | r--p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/simple | ELF文件头
| 0x401000 | 0xe98000 | 0xa97000 | r-xp | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/simple | 可运行的代码（ELF文件在内存中映射的一部分）
| 0xe98000 | 0xec1000 | 0x29000 | r--p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/simple | 只读的数据
| 0xec1000 | 0xec8000 | 0x7000 | rw-p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/simple | 可读可写的数据
| 0xec8000 | 0x18cd000 | 0xa05000 | rw-p | [heap] | 堆区
| 0x7ffff7ff9000 | 0x7ffff7ffd000 | 0x4000 | r--p | [vvar] | 只读的数据（os管理的不可写入的部分）减少系统调用进入内核，准备好一些进程想知道的os信息（只读），比如cpu状态，当前时间等（实现不进人内核的系统调用）
| 0x7ffff7ffd000 | 0x7ffff7fff000 | 0x2000 | r-xp | [vdso] | 可执行的代码（os管理的不可写入的部分）
| 0x7ffffffde000 | 0x7ffffffff000 | 0x21000 | rw-p | [stack] |
| 0xffffffffff600000 | 0xffffffffff601000 | 0x1000 | --xp | [vsyscall] |

整体架构：
红区 | ELF文件 | heap | 
