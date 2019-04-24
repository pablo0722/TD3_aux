#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
#define MAX_RING_BUFFER_SIZE	32










/************************************************
 * 					STRUCTS	 					*
 ************************************************/
typedef struct
{
	dword length;
	dword idx_read;
	dword idx_write;
	dword buffer[MAX_RING_BUFFER_SIZE];
} RingBuffer;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void 	_ring_buffer_init(RingBuffer *ring_buffer);
__FUNCTION__ dword 	_ring_buffer_push(RingBuffer *ring_buffer, const dword *src, dword length);
__FUNCTION__ dword 	_ring_buffer_pop(RingBuffer *ring_buffer, dword *dst, dword length);
__FUNCTION__ byte 	_ring_buffer_is_full(RingBuffer *ring_buffer);
__FUNCTION__ byte 	_ring_buffer_is_empty(RingBuffer *ring_buffer);

#endif // _RING_BUFFER_H_