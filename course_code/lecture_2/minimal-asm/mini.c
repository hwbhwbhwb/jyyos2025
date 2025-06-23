void _start() {
    __asm__("mov $60, %eax\n"  // syscall: exit
            "xor %edi, %edi\n" // status: 0
            "syscall");
}