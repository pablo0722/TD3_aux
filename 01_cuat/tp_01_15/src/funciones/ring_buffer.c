/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "funciones/ring_buffer.h"

#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					STRUCTS	 					*
 ************************************************/










/************************************************
 * 					DATA 	 					*
 ************************************************/
// BSS (No inicializado)

// DATA (inicializado)










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _ring_buffer_init(RingBuffer *ring_buffer)
{
	ring_buffer->length 	= MAX_RING_BUFFER_SIZE;
	ring_buffer->idx_read 	= 0;
	ring_buffer->idx_write 	= 0;
}

__FUNCTION__ dword _ring_buffer_push(RingBuffer *ring_buffer, const dword *src, dword src_length)
{
	int i;

	dword *idx_write = 	&(ring_buffer->idx_write);
	dword buf_length = 	ring_buffer->length;
	dword *buffer = 	ring_buffer->buffer;

	for(i=0; i<src_length; i++)
	{
		if(_ring_buffer_is_full(ring_buffer))
		{
			// No hay más lugar
			return i;
		}

		buffer[*idx_write] = *(src++);

		(*idx_write)++;

		(*idx_write) %= buf_length;
	}

	return i;
}

__FUNCTION__ dword _ring_buffer_pop(RingBuffer *ring_buffer, dword *dst, dword dst_length)
{
	int i;

	dword *idx_read = 	&(ring_buffer->idx_read);
	dword buf_length = 	ring_buffer->length;
	dword *buffer = 	ring_buffer->buffer;

	for(i=0; i<dst_length; i++)
	{
		if(_ring_buffer_is_empty(ring_buffer))
		{
			// No hay datos nuevos
			return i;
		}

		if(dst)
		{
			*(dst++) = buffer[*idx_read];
		}

		(*idx_read)++;
		(*idx_read) %= buf_length;
	}

	return i;
}

__FUNCTION__ dword _ring_buffer_peek(RingBuffer *ring_buffer, dword *dst, dword dst_length)
{
	int i;

	dword idx_read = 	ring_buffer->idx_read;
	dword buf_length = 	ring_buffer->length;
	dword *buffer = 	ring_buffer->buffer;

	for(i=0; i<dst_length; i++)
	{
		if(ring_buffer->idx_write == idx_read)
		{
			// No hay datos nuevos
			return i;
		}

		if(dst)
		{
			*(dst++) = buffer[idx_read];
		}

		idx_read++;
		idx_read %= buf_length;
	}

	return i;
}

__FUNCTION__ byte _ring_buffer_is_full(RingBuffer *ring_buffer)
{
	dword buffer_next_idx_write = ring_buffer->idx_write + 1;

	buffer_next_idx_write %= ring_buffer->length;

	return buffer_next_idx_write == ring_buffer->idx_read;
}

__FUNCTION__ byte _ring_buffer_is_empty(RingBuffer *ring_buffer)
{
	return ring_buffer->idx_write == ring_buffer->idx_read;
}