#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "labyrinth.h"

int main(int argc, char *argv[]) {
    Labyrinth *labyrinth = malloc(sizeof(Labyrinth));
    assert(labyrinth != NULL); // 确保内存分配成功
    bool isloadMapSuccess = false;
    isloadMapSuccess = loadMap(labyrinth, "map.txt");
    if (!isloadMapSuccess) {
        free(labyrinth);
        printf("Failed to load map from file.\n");
        return 1; // 如果加载地图失败，返回错误码 1
    }else {
        for (size_t i = 0; i < labyrinth->rows; i++)
        {
            printf("%s\n", labyrinth->map[i]);  
        }
        printf("Map loaded successfully with %d rows and %d columns.\n", labyrinth->rows, labyrinth->cols); 
    }
    
    return 0;
}

/*
    从文件加载迷宫地图，将其付给结构体 Labyrinth
    解析地图，并打印 map.txt 中的内容 (原样打印)
    如果地图文件不存在或格式不正确，退出并返回错误码 1
    输出定义：1.如果迷宫过大，退出并返回错误码 1
*/
bool loadMap(Labyrinth *labyrinth, const char *filename) {
    FILE *file = fopen(filename, "r");

    // 检查文件是否成功打开
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return false;
    }

    // 设变量，缓存区
    int rows = 0;
    int maxCols = 0;
    char buffer[MAX_COLS + 2]; // 缓存区

    // 逐行读取
    while (fgets(buffer, sizeof(buffer), file) && rows < MAX_ROWS) {
        // 去除行末的换行符
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        // 更新最大列数
        if (len > maxCols) {
            maxCols = len;
        }

        // 将行内容复制到迷宫地图中
        strncpy(labyrinth->map[rows], buffer, len);
        labyrinth->map[rows][len] = '\0'; // 确保字符串以 null 结尾
        rows++;
    }

    // 检查地图是否为空
    if (rows == 0 || maxCols == 0) {
        printf("Error: Empty map file\n");
        fclose(file);
        return false;
    }

    //检查地图是否过大（此时labyrinth不为空）
    if (rows > MAX_ROWS || maxCols > MAX_COLS) {
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


