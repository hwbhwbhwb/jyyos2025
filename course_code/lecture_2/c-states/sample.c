int main() {
    int n = 5, steps = 0;

    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }

    return steps;
}

/*
第 1 步：5 是奇数，计算 3 * 5 + 1 = 16，steps = 1。
第 2 步：16 是偶数，计算 16 / 2 = 8，steps = 2。
第 3 步：8 是偶数，计算 8 / 2 = 4，steps = 3。
第 4 步：4 是偶数，计算 4 / 2 = 2，steps = 4。
第 5 步：2 是偶数，计算 2 / 2 = 1，steps = 5。
*/