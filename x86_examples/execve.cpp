int main() {
    char *args[] = {"/bin/ls", nullptr};
    char *env[] = {"PATH=/usr/bin", nullptr};

    asm volatile (
        "movq $59, %%rax\n"  // system call number for execve()
        "movq %0, %%rdi\n"   // path to executable
        "movq %1, %%rsi\n"   // command-line arguments
        "movq %2, %%rdx\n"   // environment variables
        "syscall\n"          // issue software interrupt to invoke execve()
        "int3\n"             // breakpoint to check return value
        :
        : "r" (args[0]), "r" (args), "r" (env)
        : "rax", "rdi", "rsi", "rdx"
    );

    return 1;
}
