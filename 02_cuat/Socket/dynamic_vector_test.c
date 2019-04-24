#include <stdlib.h>
#include <stdio.h>
#include "dynamic_vector.h"





int last_key = 1;





void print_all (t_dynamic_vector *vec, int value, t_key key);





int main()
{
	t_dynamic_vector vec;
	int value = 0;
	t_key key = 0;

	

	dynamic_vector_create	(&vec, sizeof(int));
	print_all				(&vec, value, key);


	value = 4;
	key = dynamic_vector_add(&vec, &value);
	print_all				(&vec, value, key);

	value = 8;
	key = dynamic_vector_add(&vec, &value);
	print_all				(&vec, value, key);

	value = 20;
	key = dynamic_vector_add(&vec, &value);
	print_all				(&vec, value, key);

	dynamic_vector_remove	(&vec, 2);
	print_all				(&vec, value, key);

	key = dynamic_vector_find_key (&vec, 3);
	print_all				(&vec, value, key);

	dynamic_vector_free_all	(&vec);
	print_all				(&vec, value, key);


	dynamic_vector_create	(&vec, sizeof(int));
	print_all				(&vec, value, key);

	value = 5;
	key = dynamic_vector_add(&vec, &value);
	print_all				(&vec, value, key);

	value = 9;
	key = dynamic_vector_add(&vec, &value);
	print_all				(&vec, value, key);

	value = 21;
	key = dynamic_vector_add(&vec, &value);
	print_all				(&vec, value, key);

	dynamic_vector_remove	(&vec, 4);
	print_all				(&vec, value, key);



	return 0;
}





void print_all				(t_dynamic_vector *vec, int value, t_key key)
{
	t_key i;

	printf ("vec->elements: %p\n", 		vec->elements);
	printf ("vec->element_size: %u\n",	vec->element_size);
	printf ("vec->num_elements: %llu\n",vec->num_elements);
	printf ("vec->initialized: %d\n",	vec->initialized);

	for (i = 0; i < vec->num_elements; i++)
	{
		printf ("i: %llu, key: %llu, elem: %d\n", i, vec->elements[i].key, *((int *) vec->elements[i].element));
	}

	printf ("value: %d\n", 	value);
	printf ("key: %llu\n", 	key);

	printf ("\n");
}