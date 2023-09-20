#include <stdio.h>
#include <stdlib.h>
#include "generic_vector.h"
#include "generic.h"

struct generic_vector
{
	int size;
	int capacity;
	ITEM* data;
	void (*item_destroy)(ITEM* phItem);
	Status(*item_assignment)(ITEM* pLeft, ITEM right);
	
};
typedef struct generic_vector Generic_vector;

GENERIC_VECTOR generic_vector_init_default(
	void (*item_assignment)(ITEM* pLeft, ITEM right),
	void (*item_destroy)(ITEM* phItem))
{
	int i;
	Generic_vector* pVector = (Generic_vector*)malloc(sizeof(Generic_vector));
	if (pVector != NULL)
	{
		pVector->size = 0;
		pVector->capacity = 1;
		pVector->data = (ITEM*)malloc(sizeof(ITEM) * pVector->capacity);
		if (pVector->data == NULL)
		{
			free(pVector);
			return NULL;
		}
		for (i = 0; i < pVector->capacity; i++)
		{
			pVector->data[i] = NULL;
		}
		pVector->item_assignment = item_assignment;
		pVector->item_destroy = item_destroy;
	}
	return (GENERIC_VECTOR)pVector;
}

int generic_vector_get_size(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	return pVector->size;
}

int generic_vector_get_capacity(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	return pVector->capacity;
}

Status generic_vector_push_back(GENERIC_VECTOR hVector, ITEM value)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	ITEM* temp;
	int i;

	//if there is not enough room then make room.
	if (pVector->size >= pVector->capacity)
	{
		temp = (ITEM*)malloc(sizeof(ITEM) * pVector->capacity * 2);
		if (temp == NULL)
		{
			return Failure;
		}
		for (i = 0; i < pVector->size; i++)
		{
			temp[i] = pVector->data[i];
		}
		pVector->capacity *= 2;
		for (; i < pVector->capacity; i++)
		{
			temp[i] = NULL;
		}
		free(pVector->data);
		pVector->data = temp;
	}
	pVector->item_assignment(&(pVector->data[pVector->size]), value);
	pVector->size++;
	return Success;
}

Status generic_vector_pop_back(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	if (generic_vector_empty(hVector))
	{
		return Failure;
	}
	pVector->item_destroy(&(pVector->data[pVector->size - 1]));
	pVector->size--;
	return Success;
}

Boolean generic_vector_empty(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	if (pVector->size <= 0)
	{
		return TRUE;
	}
	return FALSE;
}

ITEM generic_vector_at(GENERIC_VECTOR hVector, int index)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	if (index < 0 || index >= pVector->size)
	{
		return NULL;
	}

	return (pVector->data[index]);
}

void generic_vector_destroy(GENERIC_VECTOR* phVector)
{
	Generic_vector* pVector = (Generic_vector*)*phVector;
	int i;

	for (i = 0; i < pVector->size; i++)
	{
		pVector->item_destroy(&(pVector->data[i]));
	}

	free(pVector->data);
	free(pVector);
	*phVector = NULL;
}

Status generic_vector_copy(GENERIC_VECTOR left_vector, GENERIC_VECTOR right_vector) {
	Generic_vector* pLeft_vector = (Generic_vector*)left_vector;
	Generic_vector* pRight_vector = (Generic_vector*)right_vector;
	
	if (pLeft_vector->size > 0) {
		while (generic_vector_pop_back(left_vector));
	}
	for (int i = 0; i < generic_vector_get_size(right_vector); i++) {
		generic_vector_push_back(left_vector, generic_vector_at(right_vector, i));
	}

	pLeft_vector->size = pRight_vector->size;
	pLeft_vector->item_assignment = pRight_vector->item_assignment;
	pLeft_vector->item_destroy = pRight_vector->item_destroy;
}