#include <stdio.h>
#include <stdint.h>
#include <intrin.h>

#define LINE_SIZE   64

#define L1_WAYS     8
#define L1_SETS     64
#define L1_LINES    512

// 32K memory for filling in L1 cache
uint8_t data[L1_LINES*LINE_SIZE];

int main()
{
	volatile uint8_t *addr;
	register uint64_t i;
	unsigned int junk = 0;
	register uint64_t t1, t2;

	printf("data: %p\n", data);

	//_mm_clflush(data);
	printf("accessing 16 bytes in a cache line:\n");
	for (i = 0; i < 16; i++) {
		t1 = __rdtscp(&junk);
		addr = &data[i];
		junk = *addr;
		t2 = __rdtscp(&junk) - t1;
		printf("i = %2d, cycles: %ld\n", i, t2);
	}

	return 0;
}