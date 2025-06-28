#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <testkit.h>
#include "labyrinth.h"
#include <getopt.h>

void printUsage()
{
    printf("Usage:\n");
    printf("  labyrinth --map map.txt --player id\n");
    printf("  labyrinth -m map.txt -p id\n");
    printf("  labyrinth --map map.txt --player id --move direction\n");
    printf("  labyrinth --version\n");
}

bool isValidPlayer(char playerId)
{
    if (playerId >= '0' && playerId <= '9')
    {
        return true; // 有效的玩家 ID
    }
    return false;
}

/*
    从文件加载迷宫地图，将其付给结构体 Labyrinth
    解析地图，并打印 map.txt 中的内容 (原样打印)
    文件无法打开、地图为空、地图过大情况都要return false
*/
bool loadMap(Labyrinth *labyrinth, const char *filename)
{
    FILE *file = fopen(filename, "r");

    // 检查文件是否成功打开
    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return false;
    }

    // 设变量，缓存区
    int rows = 0;
    int maxCols = 0;
    char buffer[MAX_COLS + 2]; // 缓存区

    // 逐行读取
    while (fgets(buffer, sizeof(buffer), file) && rows < MAX_ROWS)
    {
        // 去除行末的换行符
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            len--;
        }

        // 更新最大列数
        if (len > maxCols)
        {
            maxCols = len;
        }

        // 将行内容复制到迷宫地图中
        strncpy(labyrinth->map[rows], buffer, len);
        labyrinth->map[rows][len] = '\0'; // 确保字符串以 null 结尾
        rows++;
    }

    // 检查地图是否为空
    if (rows == 0 || maxCols == 0)
    {
        printf("Error: Empty map file\n");
        fclose(file);
        return false;
    }

    // 检查地图是否过大（此时labyrinth不为空）
    if (rows > MAX_ROWS || maxCols > MAX_COLS)
    {
        printf("Error: Map is too large\n");
        fclose(file);
        return false;
    }

    // 读取地图文件
    labyrinth->rows = rows;
    labyrinth->cols = maxCols;

    fclose(file);
    return true;
}

// 很简单，在迷宫中查找玩家的位置
// 如果找到，返回玩家的位置，否则返回 (-1, -1)
Position findPlayer(Labyrinth *labyrinth, char playerId)
{
    Position pos = {-1, -1};
    for (int i = 0; i < labyrinth->rows; i++)
    {
        for (int j = 0; j < labyrinth->cols; j++)
        {
            if (labyrinth->map[i][j] == playerId)
            {
                pos.row = i;
                pos.col = j;
                return pos; // 找到玩家位置，返回
            }
        }
    }
    // 如果没有找到玩家，返回 (-1, -1)
    return pos;
}

// 查找第一个空地的位置，返回 (-1, -1) 如果没有找到
Position findFirstEmptySpace(Labyrinth *labyrinth)
{
    Position pos = {-1, -1};
    for (int i = 0; i < labyrinth->rows; i++)
    {
        for (int j = 0; j < labyrinth->cols; j++)
        {
            if (labyrinth->map[i][j] == '.')
            {
                pos.row = i;
                pos.col = j;
                return pos;
            }
        }
    }
    return pos;
}

// 检查指定位置是否为空地，1代表是空地
bool isEmptySpace(Labyrinth *labyrinth, int row, int col)
{
    if (row < 0 || row >= labyrinth->rows || col < 0 || col >= labyrinth->cols)
    {
        return false; // 越界
    }
    if (labyrinth->map[row][col] == '.')
    {
        return true; // 是空格
    }
    return false;
}

bool isEmptySpaceorUser(Labyrinth *labyrinth, int row, int col)
{
    if (row < 0 || row >= labyrinth->rows || col < 0 || col >= labyrinth->cols)
    {
        return false; // 越界
    }
    if (labyrinth->map[row][col] == '.' || isValidPlayer(labyrinth->map[row][col]))
    {
        return true; // 是空格或玩家
    }
    return false;
}

// 实现了先初始化位置+根据指令移动玩家
bool movePlayer(Labyrinth *labyrinth, char playerId, const char *direction)
{
    Position playerPos = findPlayer(labyrinth, playerId);
    // 初始化位置,不存在的玩家直接丢到最近的空地上
    if (playerPos.row == -1 && playerPos.col == -1)
    {
        printf("Player %c not found in the labyrinth, put him on the first emptySpace.\n", playerId);
        Position emptySpace = findFirstEmptySpace(labyrinth);
        if (emptySpace.row != -1 && emptySpace.col != -1)
        {
            labyrinth->map[emptySpace.row][emptySpace.col] = playerId;
            playerPos.row = emptySpace.row;
            playerPos.col = emptySpace.col;
            printf("Placed player %c at (%d, %d).\n", playerId, playerPos.row, playerPos.col);
            return true; // 成功放置玩家
        }
        else
        {
            printf("No empty space found to place the player.\n");
            return false;
        }
    }
    else    //存在的玩家正常移动
    {
        // 根据方向移动玩家
        if (strcmp(direction, "up") == 0)
        {
            if (isEmptySpace(labyrinth, playerPos.row - 1, playerPos.col))
            {
                labyrinth->map[playerPos.row - 1][playerPos.col] = playerId;
                labyrinth->map[playerPos.row][playerPos.col] = '.'; // 清除原位置
                return true;
            }
            else
            {
                printf("Cannot move up, space is not empty.\n");
                return false; // 无法移动
            }
        }
        else if (strcmp(direction, "down") == 0)
        {
            if (isEmptySpace(labyrinth, playerPos.row + 1, playerPos.col))
            {
                labyrinth->map[playerPos.row + 1][playerPos.col] = playerId;
                labyrinth->map[playerPos.row][playerPos.col] = '.';
                return true;
            }
            else
            {
                printf("Cannot move down, space is not empty.\n");
                return false; // 无法移动
            }
        }
        else if (strcmp(direction, "left") == 0)
        {
            if (isEmptySpace(labyrinth, playerPos.row, playerPos.col - 1))
            {
                labyrinth->map[playerPos.row][playerPos.col - 1] = playerId;
                labyrinth->map[playerPos.row][playerPos.col] = '.';
                return true;
            }
            else
            {
                printf("Cannot move left, space is not empty.\n");
                return false; // 无法移动
            }
        }
        else if (strcmp(direction, "right") == 0)
        {
            if (isEmptySpace(labyrinth, playerPos.row, playerPos.col + 1))
            {
                labyrinth->map[playerPos.row][playerPos.col + 1] = playerId;
                labyrinth->map[playerPos.row][playerPos.col] = '.';
                return true;
            }
            else
            {
                printf("Cannot move right, space is not empty.\n");
                return false; // 无法移动
            }
        }
    }
    return false;
}

bool saveMap(Labyrinth *labyrinth, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: Could not open file %s for writing\n", filename);
        return false;
    }
    // Save the labyrinth map to the file
    for (int i = 0; i < labyrinth->rows; i++)
    {
        for (int j = 0; j < labyrinth->cols; j++)
        {
            fputc(labyrinth->map[i][j], file);
        }
        fputc('\n', file);
    }

    fclose(file);
    return true;
}

// Check if all empty spaces are connected using DFS
void dfs(Labyrinth *labyrinth, int row, int col, bool visited[MAX_ROWS][MAX_COLS])
{
    visited[row][col] = true;                                  // 标记当前格子为已访问,目标：遍历所有空格
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 定义方向:上、下、左、右

    // 进行递归
    for (int i = 0; i < 4; i++)
    {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];

        // 检查新位置是否在迷宫内且未被访问且是空格
        if (isEmptySpaceorUser(labyrinth, newRow, newCol) && !visited[newRow][newCol])
        {
            dfs(labyrinth, newRow, newCol, visited); // 递归访问
        }
    }
}

// 没有看到一个小细节，视玩家为空地
bool isConnected(Labyrinth *labyrinth)
{
    bool visited[MAX_ROWS][MAX_COLS] = {false};      // 访问标记数组
    Position start = findFirstEmptySpace(labyrinth); // 找到第一个空格
    if (start.row == -1 && start.col == -1)
    {
        printf("No empty spaces in the labyrinth.\n");
        return true; // 如果没有空格，认为是连通的（可能有问题：全是玩家，没有空地，但似乎也可以）
    }

    // 从第一个空格开始 DFS
    dfs(labyrinth, start.row, start.col, visited);

    // 检查所有空格是否都被访问过
    for (int i = 0; i < labyrinth->rows; i++)
    {
        for (int j = 0; j < labyrinth->cols; j++)
        {
            if ((labyrinth->map[i][j] == '.' || isValidPlayer(labyrinth->map[i][j])) && !visited[i][j])
            {
                printf("Empty space at (%d, %d) is not connected.\n", i, j);
                return false; // 如果有空格没有被访问过，返回 false
            }
        }
    }
    printf("All empty spaces are connected.\n");
    return true;
}

int main(int argc, char *argv[])
{
    /*
        写入命令，命令不符合格式就退出，正确了再进行命令解析，给到函数的一定是正确的命令
        （函数中不再对错误命令进行判断，实际上多判断判断也挺好的），再进行其他错误判断
        解析命令行参数
    */
    const char *filename = NULL;
    const char *playerId = NULL;
    const char *moveDirection = NULL;

    int version_flag = 0; // 标记是否包含 --version
    int other_args = 0;   // 标记是否有其他参数

    // 定义长选项
    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'}, // 这边似乎没有严格遵循不支持短选项
        {"map", required_argument, 0, 'm'},
        {"player", required_argument, 0, 'p'},
        {"move", required_argument, 0, 'd'}, // 这边似乎没有严格遵循不支持短选项
        {0, 0, 0, 0}                         // 结束标记
    };

    // 解析参数
    int opt;
    while ((opt = getopt_long(argc, argv, "vm:p:o:", long_options, NULL)) != -1)
    {
        if (opt == -1)
            break; // 所有选项解析完毕

        switch (opt)
        {
        case 'v':
            // 如果有 --version 选项，设置标志
            version_flag = 1;
            break;
        case 'm':
            // 处理 --map 或 -m 选项
            filename = optarg;
            if (filename != NULL)
            {
                printf("Map File: %s\n", optarg);
                other_args = 1; // 标记有其他参数
            }
            break;
        case 'p':
            playerId = optarg;
            if (playerId != NULL)
            {
                printf("Player ID: %s\n", optarg);
                other_args = 1; // 标记有其他参数
            }
            break;
        case 'd':
            // 处理 --move 或 -d 选项
            moveDirection = optarg;
            if (moveDirection != NULL && moveDirection[0] != '\0')
            { // 确保方向不为空
                if (strcmp(moveDirection, "up") != 0 && strcmp(moveDirection, "down") != 0 &&
                    strcmp(moveDirection, "left") != 0 && strcmp(moveDirection, "right") != 0)
                {
                    fprintf(stderr, "Error: Invalid move direction '%s'. Use 'up', 'down', 'left', or 'right'.\n", moveDirection);
                    printUsage(); // 显示正确用法
                    return 1;     // 返回错误码 1
                }
                else
                {
                    printf("Move Direction: %s\n", moveDirection);
                    other_args = 1; // 标记有其他参数
                }
            }
            break;
        case '?': // 选项缺少参数或未知选项
            if (optopt == 'm' || optopt == 'p' || optopt == 'd')
            { // 短选项 -m/-p/-d 缺少参数
                fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
            }
            else
            { // 未知选项
                fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
                printUsage(); // 显示正确用法
            }
            return 1;
        default:
            fprintf(stderr, "Error: Invalid option or missing argument.\n");
            printUsage(); // 显示正确用法
            break;
        }
    }

    // 判断version命令有无和其他参数合用
    if (version_flag && other_args)
    {
        fprintf(stderr, "Error: --version cannot be used with other options.\n");
        printUsage(); // 显示正确用法
        return 1;     // 返回错误码 1
    }
    else if (version_flag)
    {
        printf("Version: %s\n", VERSION_INFO); // 打印版本信息
        return 0;                              // 如果只有 --version，直接返回成功
    }

    // 命令解析完之后的读取map
    Labyrinth *labyrinth = malloc(sizeof(Labyrinth));
    assert(labyrinth != NULL); // 确保内存分配成功
    bool isloadMapSuccess = false;
    isloadMapSuccess = loadMap(labyrinth, filename); // 调用加载地图函数
    if (!isloadMapSuccess)
    {
        free(labyrinth);
        printf("Failed to load map from file.\n");
        return 1; // 如果加载地图失败，返回错误码 1
    }
    else
    {
        for (size_t i = 0; i < labyrinth->rows; i++)
        {
            printf("%s\n", labyrinth->map[i]); // 打印出来
        }
        printf("Map loaded successfully with %d rows and %d columns.\n", labyrinth->rows, labyrinth->cols);
    }

    // 判断玩家id是否合法
    if (playerId == NULL || !isValidPlayer(playerId[0]) ||playerId[1] != '\0')
   {
        printf("Error: Invalid player ID, player ID must be a single digit (0-9).\n");
        free(labyrinth);
        return 1;
    }

    // 检查图是否联通
    if (!isConnected(labyrinth))
    {
        printf("The labyrinth is not connected.\n");
        free(labyrinth);
        return 1; // 如果图不连通，返回错误码 1
    }

    // 实现移动玩家按指令移动
    if (moveDirection != NULL)
    {
        if (!movePlayer(labyrinth, playerId[0], moveDirection))
        {
            printf("Failed to move player %c in direction %s.\n", playerId[0], moveDirection);
            free(labyrinth);
            return 1; // 如果移动失败，返回错误码 1
        }
        else
        {
            printf("Player %c moved %s successfully.\n", playerId[0], moveDirection);
        }
    }

    // 保存地图到文件
    if (!saveMap(labyrinth, filename))
    {
        printf("Failed to save map to file.\n");
        free(labyrinth);
        return 1; // 如果保存地图失败，返回错误码 1
    }
    else
    {
        for (size_t i = 0; i < labyrinth->rows; i++)
        {
            printf("%s\n", labyrinth->map[i]); // 打印出来
        }
        printf("Map saved successfully to %s.\n", filename);
    }
    return 0;
}