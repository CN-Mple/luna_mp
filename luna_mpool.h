/* luna_mpool.h */
#ifndef LUNA_MPOOL_H
#define LUNA_MPOOL_H

#include <stdint.h>
#include <string.h>

#ifndef LUNA_ASSERT
#include <assert.h>
#define LUNA_ASSERT			assert
#endif

#define ALIGNMENT               	(4)
#define ALIGN_UP(addr, align)   	(((addr) + (align - 1)) & ~(align - 1))

struct luna_free_block_tag {
        struct luna_free_block_tag *next;
};

struct luna_mpool {
        uint8_t *origin;
        void    *fb;

        uint32_t blk_size;
        uint32_t bottom;
        uint32_t unuse;
        uint32_t total;
};

void luna_mpool_init(struct luna_mpool *mpool, uint8_t *buffer, size_t buffer_size, size_t block_size);

void *luna_mpool_get(struct luna_mpool *mpool);
void luna_mpool_put(struct luna_mpool *mpool, void *ptr);

#endif
