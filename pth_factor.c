#include <stdio.h>
#include <stdlib.h>

long pthFactor(long n, long p) {
    long *factors = malloc(200000 * sizeof(long));
    long count = 0;
    
    for (long i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            factors[count++] = i;
            if (i != n / i) {
                factors[count++] = n / i;
            }
        }
    }
    
    for (long i = 0; i < count - 1; i++) {
        for (long j = 0; j < count - i - 1; j++) {
            if (factors[j] > factors[j + 1]) {
                long temp = factors[j];
                factors[j] = factors[j + 1];
                factors[j + 1] = temp;
            }
        }
    }
    
    printf("Total factors of %ld: %ld\n", n, count);

    long result = (p <= count) ? factors[p - 1] : 0;

    if (result != 0) {
            printf("The %ld-th factor of %ld is: %ld\n", p, n, result);
        } else {
            printf("%ld does not have %ld factors.\n", n, p);
        }

    free(factors);
    return result;
}

int main() {
    long num = 1000000000000000;
    long p = 256;
    pthFactor(num, p);
    return 0;
}