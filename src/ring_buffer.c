#include "ring_buffer.h"
#include "string.h"

struct ring_buffer
{
    size_t s_elem;
    size_t n_elem;
    alt_u8 *buf;
    size_t count;
    volatile size_t head;
    volatile size_t tail;
};

static struct ring_buffer _rb[RING_BUFFER_MAX];
static int ring_buffer_full(struct ring_buffer *rb);
static int ring_buffer_empty(struct ring_buffer *rb);

/**
 * Function to intialize ring buffer parameter.
 * @param[in] rbd_ptr pointer to the rbd_t module
 * @param[in] attr_ptr pointer to the rb_attr_t module
 * @return RET_SUCCESS in case of success
 */
ret_code_t ring_buffer_init(rbd_t *rbd_ptr, rb_attr_t *attr_ptr)
{
    static int idx = 0;
    ret_code_t err = RET_FAIL;

    if((idx < RING_BUFFER_MAX) && (rbd_ptr != NULL) && (attr_ptr != NULL))
    {
        if((attr_ptr->buffer != NULL) && (attr_ptr->s_elem > 0))
        {
            /* Check that the size of the ring buffer is a power of 2 */
            if(((attr_ptr->n_elem - 1) & attr_ptr->n_elem) == 0)
            {
                /* Initialize the ring buffer internal variables */
                _rb[idx].head = 0;
                _rb[idx].tail = 0;
                _rb[idx].buf = attr_ptr->buffer;
                _rb[idx].s_elem = attr_ptr->s_elem;
                _rb[idx].n_elem = attr_ptr->n_elem;
                _rb[idx].count = 0;

                *rbd_ptr = idx++;
                err= RET_SUCCESS;
            }
        }
    }
    return (err);
}
/**
 * Function to check if ring buffer is full.
 * @param[in] rb pointer to the ring buffer structure
 * @return 1:ring buffer full, 0:ring buffer not full
 */
static int ring_buffer_full(struct ring_buffer *rb)
{
    return (((rb->head - rb->tail) == rb->n_elem) ? 1 : 0);
}
/**
 * Function to check if ring buffer is empty.
 * @param[in] rb pointer to the ring buffer structure
 * @return 1:ring buffer empty, 0:ring buffer not empty
 */
static int ring_buffer_empty(struct ring_buffer *rb)
{
    return (((rb->head - rb->tail) == 0U) ? 1 : 0);
}
/**
 * Function that put many data to ring buffer
 * @param[in] rbd   pointer to the rbd_t module
 * @param[in] data data to add to the ring buffer
 * @param[in] n    data count
 * @return RET_SUCCESS in case of success
 */
ret_code_t ring_buffer_put_many(rbd_t rbd, const void *data, uint32_t n)
{
	ret_code_t err = RET_SUCCESS;
	if((rbd < RING_BUFFER_MAX) &&
		(ring_buffer_full(&_rb[rbd]) == 0) &&
		((_rb[rbd].n_elem - _rb[rbd].count) > n))
	{
		const size_t offset =
		(_rb[rbd].head & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
		memcpy(&(_rb[rbd].buf[offset]), data, n*_rb[rbd].s_elem);
		_rb[rbd].head +=n;
		_rb[rbd].count += n;
	}
	else
	{
		err = RET_FAIL;
	}
	return (err);
}
/**
 * Function that put data to ring buffer
 * @param[in] rbd   pointer to the rbd_t module
 * @param[in] data data to add to the ring buffer
 * @return RET_SUCCESS in case of success
 */
ret_code_t ring_buffer_put(rbd_t rbd, const void *data)
{
	ret_code_t err = RET_SUCCESS;
    if((rbd < RING_BUFFER_MAX) && (ring_buffer_full(&_rb[rbd]) == 0))
    {
        const size_t offset =
        (_rb[rbd].head & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
        memcpy(&(_rb[rbd].buf[offset]), data, _rb[rbd].s_elem);
        _rb[rbd].head++;
        _rb[rbd].count++;
    }
    else
    {
        err = RET_FAIL;
    }
    return (err);
}
/**
 * Function that get data from ring buffer
 * @param[in] data data to get from ring buffer
 * @return RET_SUCCESS in case of success
 */
ret_code_t ring_buffer_get(rbd_t rbd, void *data)
{
	ret_code_t err = RET_SUCCESS;
    if((rbd < RING_BUFFER_MAX) && (ring_buffer_empty(&_rb[rbd]) == 0))
    {
        const size_t offset =
        (_rb[rbd].tail & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
        memcpy(data, &(_rb[rbd].buf[offset]), _rb[rbd].s_elem);
        _rb[rbd].tail++;
        _rb[rbd].count--;
    }
    else
    {
        err = RET_FAIL;
    }
    return (err);
}
/**
 * Function that get ring buffer length
 * @param[in]  rbd  descriptor of the rbd_t module
 * @return count ring buffer number
 */
size_t ring_buffer_get_count(rbd_t rbd)
{
    return (_rb[rbd].count);
}
