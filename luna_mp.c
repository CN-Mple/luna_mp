/* luna_mp.c */
#include "luna_mp.h"

luna_mp_err_t luna_mp_init(struct luna_mp *mp, uint8_t *buffer, uint32_t buffer_size, uint32_t block_size)
{
        luna_mp_err_t rt = LUNA_MP_OK;
        if (!mp || !buffer || buffer_size == 0 || block_size == 0) {
                return LUNA_MP_PARAM_ERR;
        }
        memset(mp, 0, sizeof(struct luna_mp));

        mp->blk_size = block_size;

        block_size = block_size + sizeof(struct luna_mp_fb_tag);
	block_size = ALIGN_UP(block_size, sizeof(struct luna_mp_fb_tag));

        uintptr_t aligned = ALIGN_UP(((uintptr_t)buffer), ALIGNMENT);
        uint32_t  offset  = aligned - (uintptr_t)buffer;
	buffer_size      -= offset;
        if (buffer_size < block_size) {
                return LUNA_MP_NO_MEM;
        }

        mp->start    = (uint8_t *)aligned;
        mp->fb       = (struct luna_mp_fb_tag *)aligned;
        mp->capacity = buffer_size;
	while (buffer_size >= block_size) {
		mp->total += 1;
		buffer_size -= block_size;
	}

        struct luna_mp_fb_tag *fb = (struct luna_mp_fb_tag *)mp->fb;
        for (uint32_t i = 0; i < mp->total - 1; ++i) {
                fb->next = (struct luna_mp_fb_tag *)((uintptr_t)fb + block_size);
                fb = fb->next;
        }
        fb->next = NULL;

        mp->unuse  = mp->total;
        mp->bottom = mp->total;

        return rt;
}

void *luna_mp_get(struct luna_mp *mp)
{
        if (!mp) {
                return NULL;
        }
        if (mp->unuse == 0) {
                return NULL;
        }

        struct luna_mp_fb_tag *fb = (struct luna_mp_fb_tag *)mp->fb;
        mp->fb    = fb->next;
        mp->unuse -= 1;
        if (mp->bottom > mp->unuse) {
                mp->bottom = mp->unuse;
        }
        fb->next = NULL;

        return (fb + 1);
}

luna_mp_err_t luna_mp_put(struct luna_mp *mp, void *ptr)
{
        luna_mp_err_t rt = LUNA_MP_OK;
        if (!mp || !ptr) {
                return LUNA_MP_PARAM_ERR;
        }
        struct luna_mp_fb_tag *fb = (struct luna_mp_fb_tag *)ptr;
	fb -= 1;
        if (fb->next != NULL) {
                return LUNA_MP_REFREE;
        }
        fb->next   = (struct luna_mp_fb_tag *)mp->fb;
        mp->fb     = (struct luna_mp_fb_tag *)fb;
        mp->unuse += 1;
        return rt;
}
