/* luna_mpool.h */
#ifndef LUNA_MPOOL_H
#define LUNA_MPOOL_H

#include <stdint.h>
#include <string.h>

#ifndef LUNA_ASSERT
#include <assert.h>
#define LUNA_ASSERT			assert
#endif

#define ALIGNMENT               	(8)
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

#ifdef LUNA_MPOOL_IMPLEMENTATION

void luna_mpool_init(struct luna_mpool *mpool, uint8_t *buffer, size_t buffer_size, size_t block_size)
{
        LUNA_ASSERT(mpool       != 0);
        LUNA_ASSERT(buffer      != 0);
        LUNA_ASSERT(buffer_size != 0);
        LUNA_ASSERT(block_size  != 0);
        
        memset(mpool, 0, sizeof(struct luna_mpool));

        mpool->origin     = buffer;
        mpool->blk_size   = block_size;
	block_size        = ALIGN_UP(block_size, sizeof(struct luna_free_block_tag));
        block_size       += sizeof(struct luna_free_block_tag);

        uintptr_t aligned = ALIGN_UP((uintptr_t)buffer, ALIGNMENT);
        size_t offset     = aligned - (uintptr_t)buffer;
	buffer_size      -= offset;
	// At least support one block.
        LUNA_ASSERT(buffer_size > block_size);

        mpool->fb         = (void *)aligned;
	while (buffer_size >= block_size) {
		mpool->total += 1;
		buffer_size -= block_size;
	}

        struct luna_free_block_tag *fb = (struct luna_free_block_tag *)mpool->fb;
        for (uint32_t i = 0; i < mpool->total - 1; ++i) {
                fb->next = (struct luna_free_block_tag *)((uintptr_t)fb + block_size);
                fb = fb->next;
        }
        fb->next = NULL;

        mpool->unuse  = mpool->total;
        mpool->bottom = mpool->total;
}

void *luna_mpool_get(struct luna_mpool *mpool)
{
        LUNA_ASSERT(mpool        != 0);
	LUNA_ASSERT(mpool->unuse != 0);

        struct luna_free_block_tag *fb = (struct luna_free_block_tag *)mpool->fb;
        mpool->fb                      = fb->next;
        mpool->unuse                  -= 1;
        if (mpool->bottom > mpool->unuse) {
                mpool->bottom = mpool->unuse;
        }
        return (fb + 1);
}

void luna_mpool_put(struct luna_mpool *mpool, void *ptr)
{
        LUNA_ASSERT(mpool != 0);
        LUNA_ASSERT(ptr   != 0);

        struct luna_free_block_tag *fb = (struct luna_free_block_tag *)ptr;
	fb           -= 1;
        fb->next      = (struct luna_free_block_tag *)mpool->fb;
        mpool->fb     = (uint8_t *)fb;
        mpool->unuse += 1;
}

#endif
