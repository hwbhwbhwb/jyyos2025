**ELF 可执行文件**：ELF 是 Executable and Linkable Format 的缩写，是 Linux 系统上的一种可执行文件格式。我们可以试着去构造、理解、可视化 ELF 文件的结构。

hwb@hwb-virtual-machine:~/jyyos_hwb/os2025/course_code/lecture_10/elf-play$ strace ./a.out
execve("./a.out", ["./a.out"], 0x7ffeba41e5f0 /* 39 vars */) = 0
exit(0)                                 = ?
+++ exited with 0 +++

elfcat十分好用：
![alt text](image.png)