# luna_pool

``` c
/* main.c */
#include <stdio.h>

#include "luna_mp.h"

int main(void)
{
	uint8_t buffer[1024];
	struct luna_mp mp;
	const size_t block_size = 64;

	luna_mp_init(&mp, buffer, sizeof(buffer), block_size);

	printf("=== Memory Pool Statistics ===\n");
	printf("Total Blocks: %u\n", mp.total);
	printf("Free Blocks:  %u\n", mp.unuse);
	printf("Watermark:    %u\n\n", mp.bottom);

	void *p1 = luna_mp_get(&mp);
	void *p2 = luna_mp_get(&mp);
	void *p3 = luna_mp_get(&mp);

	printf("Allocated Addresses:\n");
	printf("p1: %p\n", p1);
	printf("p2: %p\n", p2);
	printf("p3: %p\n", p3);
	printf("Free Blocks After Allocation: %u\n\n", mp.unuse);

	luna_mp_put(&mp, p1);
	luna_mp_put(&mp, p2);

	printf("Free Blocks After Partial Free: %u\n\n", mp.unuse);

	void *p4 = luna_mp_get(&mp);
	void *p5 = luna_mp_get(&mp);

	printf("Reallocated Addresses:\n");
	printf("p4: %p\n", p4);
	printf("p5: %p\n", p5);
	printf("Free Blocks After Reallocation: %u\n\n", mp.unuse);

	luna_mp_put(&mp, p3);
	luna_mp_put(&mp, p4);
	luna_mp_put(&mp, p5);

	printf("=== Final Memory Pool State ===\n");
	printf("Free Blocks: %u\n", mp.unuse);
	printf("Lowest Watermark: %u\n", mp.bottom);

	return 0;
}
```
