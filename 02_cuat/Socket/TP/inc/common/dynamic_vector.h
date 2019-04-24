#ifndef DYNAMIC_VECTOR_H_
#define DYNAMIC_VECTOR_H_










/************************************
 *			INCLUDES				*
 ************************************/
#include "basics.h"










/************************************
 *			STRUCTS					*
 ************************************/
typedef unsigned long long 	t_key;
typedef t_key 				t_dynamic_vector_idx;
typedef unsigned int		t_size;

typedef struct
{
	t_key	key;
	void *	element;
} t_dynamic_vector_element;

typedef struct
{
	t_dynamic_vector_element *	elements;
	t_size						element_size;
	t_dynamic_vector_idx		num_elements;
	bool						initialized;
} t_dynamic_vector;










/************************************
 *			FUNCTIONS				*
 ************************************/
void  					dynamic_vector_create	(t_dynamic_vector * dyn_vec, int element_size);
t_key 					dynamic_vector_add		(t_dynamic_vector * dyn_vec, void *element);
void  					dynamic_vector_remove	(t_dynamic_vector * dyn_vec, t_key key);
void  					dynamic_vector_free_all	(t_dynamic_vector * dyn_vec);
t_dynamic_vector_idx	dynamic_vector_find_key	(t_dynamic_vector * dyn_vec, t_key key);
t_key  					dynamic_vector_find 	(t_dynamic_vector * dyn_vec,
												 void *			 	element_compare,
												 bool (*		 	compare_func)(void) );










#endif // DYNAMIC_VECTOR_H_