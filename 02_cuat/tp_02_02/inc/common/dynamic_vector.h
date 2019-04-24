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
	bool						initialized;
	t_dynamic_vector_element *	elements;
	t_size						element_size;
	t_dynamic_vector_idx		num_elements;
	t_dynamic_vector_idx		max_num;
	int 						sem_id_max_num;
	int 						sem_id_empty;
	int 						sem_id_atomic_access;
} t_dynamic_vector;










/************************************
 *			FUNCTIONS				*
 ************************************/
void  	dynamic_vector_create				(t_dynamic_vector * 	dyn_vec,
											 int 					element_size,
											 int 					max_num);
void * 	dynamic_vector_add					(t_dynamic_vector * 	dyn_vec, void *element);
void 	dynamic_vector_wait_for_vacancy		(t_dynamic_vector * 	dyn_vec);
void 	dynamic_vector_wait_for_empty		(t_dynamic_vector * 	dyn_vec);
void 	dynamic_vector_do_for_each			(t_dynamic_vector * 	dyn_vec,
											 void (*				do_func) (void *element));
void 	dynamic_vector_remove_by_address 	(t_dynamic_vector * 	dyn_vec, void *element);
void  	dynamic_vector_remove_by_key		(t_dynamic_vector * 	dyn_vec, t_key key);
void  	dynamic_vector_free_all				(t_dynamic_vector * 	dyn_vec);
void *	dynamic_vector_find_key				(t_dynamic_vector * 	dyn_vec,
											 t_key 					key,
											 t_dynamic_vector_idx *	idx);
void *	dynamic_vector_find 				(t_dynamic_vector * 	dyn_vec,
											 void *			 		element_compare,
											 bool (*		 		compare_func)(void *element1,
											 								  void *element2) );










#endif // DYNAMIC_VECTOR_H_