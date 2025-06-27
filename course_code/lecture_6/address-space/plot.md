# Registers

| Register | Hex Value | Decimal Value |
|----------|-----------|---------------|
| rax | 0x0000000000000000 | 0 |
| rbx | 0x00007fffffffdd78 | 140737488346488 |
| rcx | 0x000000000044fd67 | 4521319 |
| rdx | 0x0000000000000000 | 0 |
| rsi | 0x00000000000ded60 | 912736 |
| rdi | 0x0000000000000003 | 3 |
| rbp | 0x0000000000000001 | 1 |
| rsp | 0x00007fffffffdb90 | 140737488346000 |
| r8 | 0x00000000004ca270 | 5022320 |
| r9 | 0x0000000000000000 | 0 |
| r10 | 0x0000000000000010 | 16 |
| r11 | 0x0000000000000246 | 582 |
| r12 | 0x0000000000000001 | 1 |
| r13 | 0x00007fffffffdd68 | 140737488346472 |
| r14 | 0x00000000004c37d0 | 4995024 |
| r15 | 0x0000000000000001 | 1 |
| rip | 0x0000000000401e2a | 4202026 |
| eflags | 0x0000000000000246 | 582 |

# Memory Mappings

| Start Address | End Address | Size | Permissions | Name |
|---------------|-------------|------|--------------|------|
| 0x400000 | 0x401000 | 0x1000 | r--p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/mmap-demo |
| 0x401000 | 0x499000 | 0x98000 | r-xp | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/mmap-demo |
| 0x499000 | 0x4c2000 | 0x29000 | r--p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/mmap-demo |
| 0x4c3000 | 0x4c7000 | 0x4000 | r--p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/mmap-demo |
| 0x4c7000 | 0x4ca000 | 0x3000 | rw-p | /home/hwb/jyyos/os2025/course_code/lecture_6/address-space/mmap-demo |
| 0x4ca000 | 0x4cf000 | 0x5000 | rw-p |  |
| 0x4cf000 | 0x4f1000 | 0x22000 | rw-p | [heap] |
| 0x7ffff7ff9000 | 0x7ffff7ffd000 | 0x4000 | r--p | [vvar] |
| 0x7ffff7ffd000 | 0x7ffff7fff000 | 0x2000 | r-xp | [vdso] |
| 0x7ffffffde000 | 0x7ffffffff000 | 0x21000 | rw-p | [stack] |
| 0xffffffffff600000 | 0xffffffffff601000 | 0x1000 | --xp | [vsyscall] |
