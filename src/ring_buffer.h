#include "stddef.h"
#include "stdint.h"
#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#include "common/inc/ipg_types.h"

#define RING_BUFFER_MAX 10

typedef struct
{
    size_t s_elem;
    size_t n_elem;
    void *buffer;
} rb_attr_t;

typedef uint32_t rbd_t;/* ring buffer descriptor*/

ret_code_t ring_buffer_init(rbd_t *rbd_ptr, rb_attr_t *attr_ptr);
ret_code_t ring_buffer_put(rbd_t rbd, const void *data);
ret_code_t ring_buffer_get(rbd_t rbd, void *data);
size_t ring_buffer_get_count(rbd_t rbd);
ret_code_t ring_buffer_put_many(rbd_t rbd, const void* data, uint32_t n);
#endif /*_RING_BUFFER_H*/
