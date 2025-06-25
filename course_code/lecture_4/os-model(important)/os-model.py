#!/usr/bin/env python3

import sys
import random
from pathlib import Path


class OS:  # 类是一些变量与操作变量方法（函数）的集合
    """
    A minimal executable operating system model. Processes
    are state machines (Python generators) that can be paused
    or continued with local states being saved.
    """

    """
    We implement four system calls:

    - read: read a random bit value.
    - write: write a string to the buffer.
    - spawn: create a new state machine (process).
    """
    SYSCALLS = ["read", "write", "spawn"]  # 理解为类中的全局变量

    class Process:
        """
        A "freezed" state machine. The state (local variables,
        program counters, etc.) are stored in the generator
        object.
        """

        # __init__是带双下划线的，称为魔法方法（magic method），目前所知性质：
        # 自动调用：无需显式调用初始化方法，创建实例时会自动执行
        # 它作为类的构造方法，当创建类的新实例时自动调用
        # 不是私有性标记：这里的双下划线不是表示私有性（外部无法修改），而是表示特殊功能（解释器专门识别）
        # 单下划线前缀（_name）：表示"受保护"属性，建议不要直接访问
        # 双下划线前缀（__name）：触发名称改写，提供更强的私有性保护

        def __init__(
            self, func, *args
        ):  # func是变量(参数)，self代表类本身的实例对象（Process）
            # func should be a generator function. Calling
            # func(*args) returns a generator object.
            self._func = func(*args)  # _func是属性,传进来一个函数func

            # This return value is set by the OS's main loop.
            """
            retval 只是一个变量命名约定，
            常用来表示"return value"（返回值），
            但这完全是编程习惯，不是 Python 解释器的规定。
            """
            self.retval = None

        # jyy评价：比较tricky
        def step(self):
            """
            Resume the process with OS-written return value,
            until the next system call is issued.
            """
            # self._func 是一个生成器对象，
            # 调用 send 方法会将 retval 作为参数传入生成器，
            # 并继续执行生成器直到遇到下一个 yield 语句。
            # 生成器对象的 send 方法会将值发送到生成器中，
            # 并返回下一个 yield 的值。
          
            # 星号前缀表示"收集剩余所有项到一个列表中",_ 是一个惯用的变量名，表示"我不关心这些值"
            # 组合起来，*_ 表示"收集所有剩余项，但我不会使用它们"，类似于垃圾箱
            syscall, args, *_ = self._func.send(self.retval)
            self.retval = None
            return syscall, args

    def __init__(self, src):
        # This is a hack: we directly execute the source
        # in the current Python runtime--and main is thus
        # available for calling.
        exec(src, globals())  # exec 函数可以动态执行字符串形式的 Python代码
        # globals() 指定在全局命名空间中执行该代码
        self.procs = [OS.Process(main)]  # 创建一个 Process 对象，
        # 封装这个生成器，将这个进程对象放入列表 self.procs，作为系统的初始进程
        self.buffer = ""  #  初始化一个空的输出缓冲区

    #  运行
    def run(self):
        # Real operating systems waste all CPU cycles
        # (efficiently, by putting the CPU into sleep) when
        # there is no running process at the moment. Our model
        # terminates if there is nothing to run.
        while self.procs:# 仍然有进程，进程列表不为空

            # There is also a pointer to the "current" process
            # in today's operating systems.
            current = random.choice(self.procs)# 随机选择一个进程执行（模拟进程调度）

            # try/except 是 Python 语言内置的异常处理机制
            try:
                # Operating systems handle interrupt and system
                # calls, and "assign" CPU to a process.
                match current.step():# 对 current.step() 的返回值进行模式匹配
                    case "read", _:
                        current.retval = random.choice([0, 1])
                    case "write", s:
                        self.buffer += s
                    case "spawn", (fn, *args):
                        self.procs += [OS.Process(fn, *args)]
                    case _:
                        assert 0

            except StopIteration:
                # The generator object terminates.
                self.procs.remove(current)

        return self.buffer


"""
1.区分脚本执行与模块导入：
当文件作为脚本直接运行时,Python 会将 __name__ 变量设置为 '__main__'
当文件被作为模块导入时,__name__ 变量会被设置为模块名
2.控制代码执行：
这种方式可以让某些代码只在直接运行文件时执行
导入该文件作为模块时，这部分代码不会执行
3.一些疑问：
当 Python 解释器开始处理一个 .py 文件时，它会立即为该模块创建一个模块对象
在这个阶段，解释器就会根据加载方式设置 __name__ 变量(name等运行的方式在解释器中规定好了)
"""
# 入口点
# 当脚本直接运行时，__name__ 的值为 '__main__'

if __name__ == "__main__":
    # 检查命令行参数
    if len(sys.argv) < 2:  # 如果没有提供足够的命令行参数
        print(f"Usage: {sys.argv[0]} file")  # 打印使用说明
        exit(1)  # 退出程序，返回错误码1

    # 读取指定文件的内容作为源代码
    src = Path(sys.argv[1]).read_text()

    # 技巧：将sys_read(...)替换为yield "sys_read", (...)
    # 这样可以将普通函数调用转换为生成器的yield语句
    for syscall in OS.SYSCALLS:# 遍历所有系统调用
        src = src.replace(f"sys_{syscall}",# 查找系统调用的函数名
                          f'yield "{syscall}", ')# 替换为yield表达式，
    # 让普通的函数变成了生成器，便于操作系统模型统一调度和管理进程

    # 创建操作系统实例并运行，然后打印输出
    stdout = OS(src).run()# 创建OS实例并执行run方法
    print(stdout)

'''
yield简化实现
这种方法让整个操作系统模型变得非常简洁，无需复杂的进程管理机制：
不需要手动保存/恢复上下文
不需要实现线程/进程调度器
不需要内存管理
'''