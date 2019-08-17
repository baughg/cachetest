#include <stdio.h>
#include <stdint.h>
#include <intrin.h>
#include <vector>

#define LINE_SIZE   64

#define L1_WAYS     8
#define L1_SETS     64
#define L1_LINES    512

// 32K memory for filling in L1 cache
uint8_t data[L1_LINES*LINE_SIZE];

int main()
{
	volatile uint8_t *addr = NULL;
	register uint64_t i;
	unsigned int junk = 0;
	register uint64_t t1, t2;
	const uint32_t patternSize = 56 * 56 * 64;
	memset(reinterpret_cast<void*>(data), 'a', sizeof(data));

	std::vector<uint32_t> skipPattern(patternSize);

	for (uint32_t p = 0; p < patternSize; ++p)
	{
		skipPattern[p] = static_cast<uint32_t>((p % 4) == 0);
	}

	
	_mm_clflush(data);
	_mm_clflush(&skipPattern[0]);
	uint32_t* skipPtr = &skipPattern[0];

	uint32_t sum = 0;
	for (i = 0; i < 16; i++) {
		t1 = __rdtscp(&junk);
		

		for (uint32_t p = 0; p < patternSize; ++p)
		{
			addr = &data[i];
			sum += (*addr * skipPtr[p]);
		}

		junk = *addr;
		t2 = __rdtscp(&junk) - t1;
		printf("i = %2d, cycles: %ld %u\n", i, t2,sum);
	}

	return 0;
}