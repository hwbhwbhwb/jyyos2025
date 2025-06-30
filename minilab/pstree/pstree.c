#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <testkit.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include "pstree.h"

bool GetProcessids(ProcessTree *processtree)
{
    assert(processtree != NULL);
    assert(processtree->processes != NULL);

    DIR *dir;
    struct dirent *entry;
    char path[256];

    dir = opendir("/proc");
    if (dir == NULL)
    {
        perror("Failed to open /proc directory");
        return false;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the directory name consists of digits (process ID)
        // 判断 d_name 是否是纯数字（代表 PID）
        char *name = entry->d_name;
        int is_pid = 1;
        for (int i = 0; name[i] != '\0'; ++i)
        {
            if (!isdigit(name[i]))
            {
                is_pid = 0;
                break;
            }
        }

        if (is_pid)
        {
            // 构造 /proc/[pid]/status 路径，确保不会截断
            if (strlen(name) > 240) {  // 预留足够空间
                continue;  // 跳过过长的目录名
            }
            snprintf(path, sizeof(path), "/proc/%s/status", name);

            // 尝试打开 status 文件
            FILE *fp = fopen(path, "r");
            if (fp)
            {
                char line[256];
                char pid_str[32] = "";
                char ppid_str[32] = "";

                while (fgets(line, sizeof(line), fp))
                {
                    if (strncmp(line, "Pid:", 4) == 0)
                    {
                        // 跳过字段名和空白字符，复制剩下的部分
                        char *start = line + 4;
                        while (*start == ' ' || *start == '\t')
                            start++;
                        
                        // 使用 snprintf 替代 strncpy 以避免截断警告
                        snprintf(pid_str, sizeof(pid_str), "%s", start);
                        pid_str[strcspn(pid_str, "\n")] = '\0'; // 去除结尾的换行
                    }
                    // 检查是否是 PPid 字段
                    // 这里使用 strncmp 来确保只匹配 "PPid:" 开头的
                    else if (strncmp(line, "PPid:", 5) == 0)
                    {
                        char *start = line + 5;
                        while (*start == ' ' || *start == '\t')
                            start++;
                        
                        // 使用 snprintf 替代 strncpy 以避免截断警告
                        snprintf(ppid_str, sizeof(ppid_str), "%s", start);
                        ppid_str[strcspn(ppid_str, "\n")] = '\0'; // 去除换行
                    }

                    if (pid_str[0] && ppid_str[0])
                        break; // 如果两个都已读到，提前退出
                }

                fclose(fp);
                
                // 将进程信息存储到 ProcessTree 中
                if (pid_str[0] && ppid_str[0]) {
                    ProcessNode *process = &processtree->processes[processtree->count];
                    
                    // 分配内存并复制字符串
                    process->pid = malloc(strlen(pid_str) + 1);
                    process->ppid = malloc(strlen(ppid_str) + 1);
                    strcpy(process->pid, pid_str);
                    strcpy(process->ppid, ppid_str);
                    
                    processtree->count++;
                    
                    // 防止数组越界
                    if (processtree->count >= 10000) {
                        break;
                    }
                }
            }
        }
    }

    closedir(dir);
    return true;
}

// 查找指定 PID 的进程
ProcessNode* FindProcessByPid(ProcessTree* tree, const char* pid) {
    for (int i = 0; i < tree->count; i++) {
        if (strcmp(tree->processes[i].pid, pid) == 0) {
            return &tree->processes[i];
        }
    }
    return NULL;
}

// 获取指定父进程的所有子进程
void GetChildren(ProcessTree* tree, const char* parent_pid, ProcessNode** children, int* child_count) {
    *child_count = 0;
    for (int i = 0; i < tree->count; i++) {
        if (strcmp(tree->processes[i].ppid, parent_pid) == 0) {
            children[*child_count] = &tree->processes[i];
            (*child_count)++;
        }
    }
}

// 比较函数，用于对子进程按 PID 排序
int compare_pids(const void* a, const void* b) {
    ProcessNode* proc_a = *(ProcessNode**)a;
    ProcessNode* proc_b = *(ProcessNode**)b;
    return atoi(proc_a->pid) - atoi(proc_b->pid);
}

// 递归打印进程树
void PrintProcessTree(ProcessTree* tree, const char* root_pid, int depth, bool show_pids) {
    ProcessNode* root = FindProcessByPid(tree, root_pid);
    if (!root) return;
    
    // 打印缩进
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    // 获取进程名称（从 /proc/pid/comm 读取）
    char comm_path[256];
    char process_name[256] = "";
    snprintf(comm_path, sizeof(comm_path), "/proc/%s/comm", root->pid);
    
    FILE* comm_fp = fopen(comm_path, "r");
    if (comm_fp) {
        if (fgets(process_name, sizeof(process_name), comm_fp)) {
            // 去除换行符
            process_name[strcspn(process_name, "\n")] = '\0';
        }
        fclose(comm_fp);
    }
    
    // 如果无法读取进程名，使用默认名称
    if (process_name[0] == '\0') {
        strcpy(process_name, "unknown");
    }
    
    // 打印进程信息
    if (show_pids) {
        printf("%s(%s)\n", process_name, root->pid);
    } else {
        printf("%s\n", process_name);
    }
    
    // 获取子进程并排序
    ProcessNode* children[1000];
    int child_count;
    GetChildren(tree, root_pid, children, &child_count);
    
    // 对子进程按 PID 排序
    qsort(children, child_count, sizeof(ProcessNode*), compare_pids);
    
    // 递归打印子进程
    for (int i = 0; i < child_count; i++) {
        PrintProcessTree(tree, children[i]->pid, depth + 1, show_pids);
    }
}

// 查找根进程（通常是 init 进程，PID 为 1）
char* FindRootProcess(ProcessTree* tree) {
    // 首先尝试查找 PID 1
    ProcessNode* init_process = FindProcessByPid(tree, "1");
    if (init_process) {
        return init_process->pid;
    }
    
    // 如果找不到 PID 1，查找 PPID 为 0 的进程
    for (int i = 0; i < tree->count; i++) {
        if (strcmp(tree->processes[i].ppid, "0") == 0) {
            return tree->processes[i].pid;
        }
    }
    
    return NULL;
}

// 释放进程树内存
void FreeProcessTree(ProcessTree* tree) {
    for (int i = 0; i < tree->count; i++) {
        free(tree->processes[i].pid);
        free(tree->processes[i].ppid);
    }
    free(tree->processes);
    tree->count = 0;
}

// 显示帮助信息
void print_help(const char* program_name) {
    printf("Usage: %s [OPTION]...\n", program_name);
    printf("Display a tree of processes.\n\n");
    printf("Options:\n");
    printf("  -p, --show-pids        show PIDs; implies -c\n");
    printf("  -n, --numeric-sort     sort output by PID\n");
    printf("  -V, --version          output version information and exit\n");
    printf("  -h, --help             display this help and exit\n");
    printf("\nReport bugs to the maintainer.\n");
}

// 显示版本信息
void print_version() {
    printf("pstree (PSmisc) JYYOS\n");
    printf("Copyright (C) 2025 JYYOS Project\n");
    printf("This is free software; see the source for copying conditions.\n");
    printf("There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A\n");
    printf("PARTICULAR PURPOSE.\n");
}

int main(int argc, char* argv[]) {
    bool show_pids = false;
    bool numeric_sort = false;
    bool version = false;
    bool help = false;
    
    // 长选项定义
    static struct option long_options[] = {
        {"show-pids",    no_argument, 0, 'p'},
        {"numeric-sort", no_argument, 0, 'n'},
        {"version",      no_argument, 0, 'V'},
        {"help",         no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    // 解析命令行参数
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "pnVh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p':
                show_pids = true;
                break;
            case 'n':
                numeric_sort = true;
                break;
            case 'V':
                version = true;
                break;
            case 'h':
                help = true;
                break;
            case '?':
                // getopt_long 已经输出了错误信息
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Usage: %s [-pnVh] [--show-pids] [--numeric-sort] [--version] [--help]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    // 处理互斥选项
    if (help) {
        print_help(argv[0]);
        return 0;
    }
    
    if (version) {
        print_version();
        return 0;
    }
    
    // 检查是否有多余的非选项参数
    if (optind < argc) {
        fprintf(stderr, "%s: extra operand '%s'\n", argv[0], argv[optind]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // 暂时忽略 numeric_sort 警告，因为功能已实现但变量标记为未使用
    (void)numeric_sort;
    
    // 初始化进程树
    ProcessTree tree;
    tree.count = 0;
    tree.processes = malloc(10000 * sizeof(ProcessNode)); // 假设最多 10000 个进程
    if (!tree.processes) {
        perror("malloc failed");
        return 1;
    }
    
    // 获取所有进程信息
    if (!GetProcessids(&tree)) {
        fprintf(stderr, "Failed to get process information\n");
        FreeProcessTree(&tree);
        return 1;
    }
    
    // 找到根进程并打印进程树
    char* root_pid = FindRootProcess(&tree);
    if (root_pid) {
        PrintProcessTree(&tree, root_pid, 0, show_pids);
    } else {
        fprintf(stderr, "Unable to find root process\n");
        FreeProcessTree(&tree);
        return 1;
    }
    
    // 清理内存
    FreeProcessTree(&tree);
    
    return 0;
}
