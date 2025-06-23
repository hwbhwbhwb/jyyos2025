import fileinput
"""
七段数码管终端显示模拟器

这个脚本模拟了七段数码管的显示效果，在终端中绘制数码管状态。
通过标准输入接收控制信号（如 "A=0; B=1; ..."），然后将对应的七段
数码管视觉化显示在终端上。

七段数码管由标记为 A 到 G 的七个线段组成：
    A: 顶部横线
    B: 右上竖线
    C: 右下竖线
    D: 底部横线
    E: 左下竖线
    F: 左上竖线
    G: 中间横线

使用方法：
    echo "A=1; B=1; C=1; D=1; E=1; F=1; G=0" | python seg-display.py

每个线段可以设置为:
    0: 关闭(显示为白色)
    1: 开启(显示为黑色)

输出是一个用 ASCII 艺术绘制的七段数码管显示，根据输入的控制信号显示相应的模式。
"""

TEMPLATE = '''
     AAAAAAAAA
    FF       BB
    FF       BB
    FF       BB
    FF       BB
     GGGGGGGG
    EE       CC
    EE       CC
    EE       CC
    EE       CC
     DDDDDDDDD
''' 

# ANSI 转义码，用于控制终端显示
CLEAR = '\033[2J\033[1;1f'  # 清屏并将光标移动到左上角
WHITE = '\033[37m░\033[0m'  # 白色块
BLACK = '\033[31m█\033[0m'  # 黑色块

for line in fileinput.input():
    # 执行输入的代码行（例如 "A=0; B=1; ..."），变量 A, B, ... 将存储在 ctx 中。
    exec(line, (ctx := {}))

    # 初始化显示内容，包含清屏和模板。
    disp = CLEAR + TEMPLATE

    for ch in 'ABCDEFG':
        # 根据变量值确定块的颜色。
        block = {
            0: WHITE,  # 如果值为 0，显示白色块
            1: BLACK,  # 如果值为 1，显示黑色块
        }.get(ctx.get(ch, 0), '?')  # 如果值不存在，显示 "?"

        # 将模板中的字符替换为对应的块。
        disp = disp.replace(ch, block)

    print(disp)  # 输出最终的显示内容
