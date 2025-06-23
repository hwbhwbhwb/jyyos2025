debug: sample hanoi
	gdb -x run.gdb sample && mv plot.md sample.md	# Run gdb on sample and rename output file
	#	作用：运行 gdb 并加载 run.gdb 脚本（-x指定脚本文件）对 sample 进行调试。
	gdb -x run.gdb hanoi && mv plot.md hanoi.md

sample: sample.c
	gcc -g -O0 -o $@ $^	# -O0 for no optimization, -g for debug symbols

hanoi: hanoi.c
	gcc -g -O0 -o $@ $^

clean:
	rm -rf plot.md sample.md hanoi.md sample hanoi


# $@：
# 表示当前规则的目标文件。
# 在 sample: sample.c 规则中，$@ 的值是 sample。
# 在 hanoi: hanoi.c 规则中，$@ 的值是 hanoi。
# $^：
# 表示当前规则的所有依赖文件（以空格分隔）。
# 在 sample: sample.c 规则中，$^ 的值是 sample.c。
# 在 hanoi: hanoi.c 规则中，$^ 的值是 hanoi.c。