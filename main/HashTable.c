#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../Headers/HashTable.h"
#include "../Headers/SaveHm.h"

struct HashMap *CreateHM(uint32_t size)
{
	struct HashMap *Table = (struct HashMap *)malloc(sizeof(struct HashMap));
	if (Table == NULL)
	{
		return NULL;
	}
	Table->size = size;
	Table->table = (struct bin **)malloc(Table->size * sizeof(struct bin *));
	if (Table->table == NULL)
	{
		return NULL;
	}

	for (uint32_t i = 0; i < Table->size; i++)
	{
		Table->table[i] = (struct bin *)malloc(sizeof(struct bin));
		if (Table->table[i] == NULL)
		{
			printf("FAILD");
			return NULL;
		}

		Table->table[i]->head = NULL;
	}
	return Table;
}

struct Node *Newnode(uint32_t key, struct HashMap *Table, char fn, int freq)
{

	struct Node *newnode = (struct Node *)malloc(sizeof(struct Node));
	if (newnode == NULL)
	{
		return NULL;
	}
	newnode->letter = fn;
	newnode->key = key;
	newnode->freq = freq;
	newnode->mark = 0;
	newnode->next = NULL;

	return newnode;
}

void InsertElement(struct HashMap *map, uint32_t key, char fn, int freq, char keys)
{

	struct Node *newnode = Newnode(key, map, fn, freq);
	if (map->table[keys]->head == NULL)
	{
		map->table[keys]->head = newnode;
	}
	else
	{
		free(newnode);
		return;
	}
}

int Delete(struct HashMap *map)
{

	struct Node *temp = NULL;
	struct Node *temp1 = NULL;

	for (int i = 0; i < map->size; i++)
	{
		if (map->table[i]->head != NULL)
		{
			temp = map->table[i]->head;
			while (temp)
			{
				temp1 = temp->next;
				free(temp);
				temp = temp1;
			}
		}
		free(map->table[i]);
	}

	free(map->table);
	free(map);
	return 0;
}
