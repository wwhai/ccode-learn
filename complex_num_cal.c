#include <stdio.h>
#include <complex.h>
#include <tgmath.h>

int main(void)
{
    double complex z1 = I * I; // 虚数单位平方
    printf("I * I = %.1f%+.1fi\n", creal(z1), cimag(z1));

    double complex z2 = pow(I, 2); // 虚数单位平方
    printf("pow(I, 2) = %.1f%+.1fi\n", creal(z2), cimag(z2));

    double PI = acos(-1);
    double complex z3 = exp(I * PI); // 欧拉公式
    printf("exp(I*PI) = %.1f%+.1fi\n", creal(z3), cimag(z3));

    double complex z4 = 1 + 2 * I, z5 = 1 - 2 * I; // 共轭
    printf("(1+2i)*(1-2i) = %.1f%+.1fi\n", creal(z4 * z5), cimag(z4 * z5));
}