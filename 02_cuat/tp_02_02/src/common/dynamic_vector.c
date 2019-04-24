/************************************
 *			INCLUDES				*
 ************************************/
// Main header
#include "dynamic_vector.h"     // t_dynamic_vector

// C headers
#include <stdlib.h>             // NULL
#include <assert.h>             // assert()
#include <string.h>             // memcpy()

// Common headers
#include "basics.h"             // bool
#include "IPC.h"            	// xxx_semaphore()

// Project headers










/************************************
 *			TYPEDEFS				*
 ************************************/
typedef t_dynamic_vector_element t_element;










/************************************
 *			VARIABLES				*
 ************************************/
// Statics
static t_size	_compare_size;
static t_key	_last_key = 1;










/************************************
 *			FORWARDS				*
 ************************************/
static t_key _dynamic_vector_find_address_unlocked 	(t_dynamic_vector * 	dyn_vec, 
													 void * 				element,
													 t_dynamic_vector_idx *	idx);

static void * _dynamic_vector_find_key_unlocked 	(t_dynamic_vector * 	dyn_vec,
													t_key 					key,
													t_dynamic_vector_idx *	idx);

static bool   _dynamic_vector_check 				(t_dynamic_vector *dyn_vec);

static bool   _bitwise_compare 						(void *element1, void *element2);










/************************************
 *			FUNCTIONS				*
 ************************************/
void dynamic_vector_create(t_dynamic_vector *dyn_vec, int element_size, int max_num)
{
	assert (dyn_vec);

	dyn_vec->element_size 			= element_size;
	dyn_vec->elements 				= NULL;
	dyn_vec->num_elements 			= 0;
	dyn_vec->initialized 			= true;
	dyn_vec->max_num				= max_num;
	dyn_vec->sem_id_atomic_access	= create_semaphore();
	dyn_vec->sem_id_max_num 		= create_semaphore();
	dyn_vec->sem_id_empty 			= create_semaphore();
}










void * dynamic_vector_add(t_dynamic_vector *dyn_vec, void *element)
{
	t_element *	new_ptr;
	t_element *	last_dynamic_vector_element;
	void *		ret_val = NULL;

	assert (dyn_vec);
	assert (element);
	assert (_dynamic_vector_check(dyn_vec));
	assert (_last_key != 0);


	lock_semaphore(dyn_vec->sem_id_atomic_access);


	if (dyn_vec->num_elements == dyn_vec->max_num && dyn_vec->max_num >= 0)
	{
		ret_val = NULL;
	}
	else
	{
		if (dyn_vec->num_elements == 0)
		{
			lock_semaphore(dyn_vec->sem_id_empty);
		}
		
		dyn_vec->num_elements++;

		if (dyn_vec->num_elements == dyn_vec->max_num && dyn_vec->max_num >= 0)
		{
			lock_semaphore (dyn_vec->sem_id_max_num);
			ret_val = NULL;
		}

		new_ptr = (t_element *) realloc (dyn_vec->elements, sizeof(t_element) * dyn_vec->num_elements);

		assert(new_ptr);
		
		dyn_vec->elements = new_ptr;

		last_dynamic_vector_element = &dyn_vec->elements[dyn_vec->num_elements - 1];
		
		last_dynamic_vector_element->element = malloc (dyn_vec->element_size);

		assert (last_dynamic_vector_element->element);

		last_dynamic_vector_element->key = _last_key++;
		memcpy(last_dynamic_vector_element->element, element, dyn_vec->element_size);

		ret_val = last_dynamic_vector_element->element;
	}


	unlock_semaphore(dyn_vec->sem_id_atomic_access);


	return ret_val;
}










void dynamic_vector_wait_for_vacancy (t_dynamic_vector * dyn_vec)
{
	assert (dyn_vec);
	assert (_dynamic_vector_check(dyn_vec));


	lock_semaphore (dyn_vec->sem_id_max_num);
	
	unlock_semaphore (dyn_vec->sem_id_max_num);
}










void dynamic_vector_wait_for_empty (t_dynamic_vector * dyn_vec)
{
	assert (dyn_vec);
	assert (_dynamic_vector_check(dyn_vec));



	lock_semaphore (dyn_vec->sem_id_empty);
	
	unlock_semaphore (dyn_vec->sem_id_empty);
}










void dynamic_vector_do_for_each(t_dynamic_vector *dyn_vec,
								void (*do_func) (void *element))
{
	int  i;

	
	assert (dyn_vec);
	assert (do_func);
	assert (_dynamic_vector_check(dyn_vec));
	assert (_last_key != 0);


	lock_semaphore (dyn_vec->sem_id_atomic_access);


	for (i = 0; i < dyn_vec->num_elements; i++)
	{
		do_func (dyn_vec->elements[i].element);
	}


	unlock_semaphore (dyn_vec->sem_id_atomic_access);
}










void dynamic_vector_remove_by_address (t_dynamic_vector *dyn_vec, void *element)
{
	t_element *				new_ptr;
	t_key					key;
	t_dynamic_vector_idx 	idx;

	assert (element);
	assert (dyn_vec);
	assert (_dynamic_vector_check(dyn_vec));


	lock_semaphore(dyn_vec->sem_id_atomic_access);
	
	key = _dynamic_vector_find_address_unlocked (dyn_vec, element, &idx);

	if (key == 0 || key >= _last_key)
	{
		return;
	}

	dyn_vec->num_elements--;

	free(element);

	if (idx < dyn_vec->num_elements)
	{
		memmove (&dyn_vec->elements[idx],
				 &dyn_vec->elements[idx + 1],
				 sizeof(t_element) * (dyn_vec->num_elements - idx));
	}

	if (dyn_vec->num_elements == 0)
	{
		free (dyn_vec->elements);
		dyn_vec->elements = NULL;
	}
	else
	{
		new_ptr = realloc (	dyn_vec->elements,
							sizeof(t_element) * (dyn_vec->num_elements));

		assert(new_ptr);
		
		dyn_vec->elements = new_ptr;
	}
	

	if (dyn_vec->num_elements == dyn_vec->max_num - 1 && dyn_vec->max_num >= 0)
	{
		unlock_semaphore (dyn_vec->sem_id_max_num);
	}


	if (dyn_vec->num_elements == 0)
	{
		unlock_semaphore(dyn_vec->sem_id_empty);
	}


	unlock_semaphore(dyn_vec->sem_id_atomic_access);
}










void dynamic_vector_remove_by_key (t_dynamic_vector *dyn_vec, t_key key)
{
	t_element *				new_ptr;
	void *					element;
	t_dynamic_vector_idx 	idx;

	assert (dyn_vec);
	assert (_dynamic_vector_check(dyn_vec));


	lock_semaphore(dyn_vec->sem_id_atomic_access);


	if (key == 0 || key >= _last_key)
	{
		return;
	}
	
	element = _dynamic_vector_find_key_unlocked (dyn_vec, key, &idx);
	
	if (element == NULL)
	{
		return;
	}

	dyn_vec->num_elements--;

	free(element);

	if (idx < dyn_vec->num_elements)
	{
		memmove (&dyn_vec->elements[idx],
				 &dyn_vec->elements[idx + 1],
				 sizeof(t_element) * (dyn_vec->num_elements - idx));
	}

	if (dyn_vec->num_elements == 0)
	{
		free (dyn_vec->elements);
		dyn_vec->elements = NULL;
	}
	else
	{
		new_ptr = realloc (	dyn_vec->elements,
							sizeof(t_element) * (dyn_vec->num_elements));

		assert(new_ptr);
		
		dyn_vec->elements = new_ptr;
	}
	

	if (dyn_vec->num_elements == dyn_vec->max_num - 1 && dyn_vec->max_num >= 0)
	{
		unlock_semaphore (dyn_vec->sem_id_max_num);
	}


	if (dyn_vec->num_elements == 0)
	{
		unlock_semaphore(dyn_vec->sem_id_empty);
	}


	unlock_semaphore(dyn_vec->sem_id_atomic_access);
}










static t_key _dynamic_vector_find_address_unlocked (t_dynamic_vector * 	dyn_vec, 
													 void * 				element,
													 t_dynamic_vector_idx *	idx)
{
	t_key ret_val = -1;
	t_dynamic_vector_idx i;



	for (i = 0; i < dyn_vec->num_elements; i++)
	{
		if (dyn_vec->elements[i].element == element)
		{
			if (idx)
			{
				*idx = i;
			}

			ret_val = dyn_vec->elements[i].key;

			break;
		}
	}


	return ret_val;
}










static void * _dynamic_vector_find_key_unlocked (t_dynamic_vector * 	dyn_vec, 
												 t_key 					key,
												 t_dynamic_vector_idx *	idx)
{
	void * ret_val = NULL;
	t_dynamic_vector_idx i;



	for (i = 0; i < dyn_vec->num_elements; i++)
	{
		if (dyn_vec->elements[i].key == key)
		{
			if (idx)
			{
				*idx = i;
			}

			ret_val = dyn_vec->elements[i].element;

			break;
		}
	}


	return ret_val;
}










void * dynamic_vector_find_key (t_dynamic_vector * 		dyn_vec,
								t_key 					key,
								t_dynamic_vector_idx *	idx)
{
	void * ret_val = NULL;

	assert (dyn_vec);
	assert (_dynamic_vector_check(dyn_vec));


	lock_semaphore(dyn_vec->sem_id_atomic_access);


	_dynamic_vector_find_key_unlocked(dyn_vec, key, idx);


	unlock_semaphore(dyn_vec->sem_id_atomic_access);


	return ret_val;
}










void * dynamic_vector_find ( t_dynamic_vector *dyn_vec,
							void *element_compare,
							bool (*compare_func)(void *element1, void* element2))
{
	void * ret_val = NULL;
	int i;

	assert (dyn_vec);
	assert (element_compare);
	assert (_dynamic_vector_check(dyn_vec));


	lock_semaphore(dyn_vec->sem_id_atomic_access);


	if (compare_func == NULL)
	{
		compare_func = _bitwise_compare;
	}

	for (i = 0; i < dyn_vec->num_elements; i++)
	{
		t_element *i_element = &(dyn_vec->elements)[i];

		if (compare_func(i_element->element, element_compare))
		{
			ret_val = i_element->element;
			break;
		}
	}


	unlock_semaphore(dyn_vec->sem_id_atomic_access);


	return ret_val;
}










void dynamic_vector_free_all(t_dynamic_vector *dyn_vec)
{
	t_dynamic_vector_idx i;

	assert (dyn_vec);
	assert (_dynamic_vector_check(dyn_vec));


	lock_semaphore(dyn_vec->sem_id_atomic_access);


	for (i = 0; i < dyn_vec->num_elements; i++)
	{
		t_element *i_element = &(dyn_vec->elements)[i];

		free (i_element->element);
	}

	free(dyn_vec->elements);

	dyn_vec->num_elements = 0;
	dyn_vec->elements = NULL;
	dyn_vec->element_size = 0;
	dyn_vec->initialized = false;


	unlock_semaphore(dyn_vec->sem_id_atomic_access);
}










/************************************
 *			LOCAL FUNCTIONS			*
 ************************************/
static bool _dynamic_vector_check(t_dynamic_vector *dyn_vec)
{
	bool ret = true;

	if ((dyn_vec->initialized == false) 							||
		(dyn_vec->num_elements > 0 && dyn_vec->elements == NULL)	||
		(dyn_vec->num_elements == 0 && dyn_vec->elements != NULL)	)
	{
		ret = false;
	}

	return ret;
}










static bool _bitwise_compare(void *element1, void *element2)
{
	bool ret = true;
	t_size i;
	char *e1 = element1;
	char *e2 = element2;



	for (i = 0; i < _compare_size; i++)
	{
		if (e1[i] != e2[i])
		{
			ret = false;
			break;
		}
	}

	return ret;
}