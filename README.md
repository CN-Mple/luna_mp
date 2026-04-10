# luna_pool

``` c
/* main.c */
#include <stdio.h>

#define LUNA_MPOOL_IMPLEMENTATION
#include "luna_mpool.h"

int main(void)
{
	uint8_t buffer[1024];
	struct luna_mpool mpool;
	const size_t block_size = 64;

	luna_mpool_init(&mpool, buffer, sizeof(buffer), block_size);

	printf("=== Memory Pool Statistics ===\n");
	printf("Total Blocks: %u\n", mpool.total);
	printf("Free Blocks:  %u\n", mpool.unuse);
	printf("Watermark:    %u\n\n", mpool.bottom);

	void *p1 = luna_mpool_get(&mpool);
	void *p2 = luna_mpool_get(&mpool);
	void *p3 = luna_mpool_get(&mpool);

	printf("Allocated Addresses:\n");
	printf("p1: %p\n", p1);
	printf("p2: %p\n", p2);
	printf("p3: %p\n", p3);
	printf("Free Blocks After Allocation: %u\n\n", mpool.unuse);

	luna_mpool_put(&mpool, p1);
	luna_mpool_put(&mpool, p2);

	printf("Free Blocks After Partial Free: %u\n\n", mpool.unuse);

	void *p4 = luna_mpool_get(&mpool);
	void *p5 = luna_mpool_get(&mpool);

	printf("Reallocated Addresses:\n");
	printf("p4: %p\n", p4);
	printf("p5: %p\n", p5);
	printf("Free Blocks After Reallocation: %u\n\n", mpool.unuse);

	luna_mpool_put(&mpool, p3);
	luna_mpool_put(&mpool, p4);
	luna_mpool_put(&mpool, p5);

	printf("=== Final Memory Pool State ===\n");
	printf("Free Blocks: %u\n", mpool.unuse);
	printf("Lowest Watermark: %u\n", mpool.bottom);

	return 0;
}
```
