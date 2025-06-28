# Makefile for Labyrinth Project
# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -I../../testkit

# 源文件和目标文件
SRCDIR = .
SOURCES = $(SRCDIR)/try.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = labyrinth

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# 编译源文件为目标文件
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 清理生成的文件
clean:
	rm -f $(OBJECTS) $(TARGET)

# 重新编译
rebuild: clean all

# 显示帮助信息
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  run      - Build and run the program"
	@echo "  clean    - Remove generated files"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  help     - Show this help message"

# 声明伪目标
.PHONY: all run clean rebuild help
