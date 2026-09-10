/* luna_mp.h */
#ifndef LUNA_MP_H
#define LUNA_MP_H

#include <stdint.h>
#include <string.h>

#define ALIGNMENT               	(4)
#define ALIGN_UP(addr, align)           ((((addr) + ((align) - 1))) & ~(((align) - 1)))

typedef enum {
        LUNA_MP_OK = 0,
        LUNA_MP_PARAM_ERR = -1,
        LUNA_MP_NO_MEM = -2,
        LUNA_MP_REFREE = -3,
} luna_mp_err_t;

struct luna_mp_fb_tag {
        struct luna_mp_fb_tag *next;
};

struct luna_mp {
        uint8_t               *start;
        uint32_t               capacity;
        struct luna_mp_fb_tag *fb;
        uint32_t               blk_size;
        uint32_t               bottom;
        uint32_t               unuse;
        uint32_t               total;
};

luna_mp_err_t luna_mp_init(struct luna_mp *mp, uint8_t *buffer, uint32_t buffer_size, uint32_t block_size);

void *luna_mp_get(struct luna_mp *mp);
luna_mp_err_t luna_mp_put(struct luna_mp *mp, void *ptr);

#endif
