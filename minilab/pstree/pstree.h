#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char* pid;
    char* ppid;
} ProcessNode;

typedef struct {
    ProcessNode* processes;
    int count;
} ProcessTree;

// 函数声明
bool GetProcessids(ProcessTree* processtree);
ProcessNode* FindProcessByPid(ProcessTree* tree, const char* pid);
void GetChildren(ProcessTree* tree, const char* parent_pid, ProcessNode** children, int* child_count);
void PrintProcessTree(ProcessTree* tree, const char* root_pid, int depth, bool show_pids);
char* FindRootProcess(ProcessTree* tree);
void FreeProcessTree(ProcessTree* tree);