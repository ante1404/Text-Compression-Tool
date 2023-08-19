#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdint.h>

#define MAX 100000

typedef struct Node *LIST;
typedef struct HashMap *HM;

struct Node
{
	LIST next;
	uint32_t key;
	char letter;
	int freq;
	int mark;
};

struct bin
{
	struct Node *head;
};

struct HashMap
{
	struct bin **table;
	uint32_t size;
};

struct HashMap *CreateHM(uint32_t size);
struct Node *Newnode(uint32_t key, struct HashMap *Table, char fn, int freq);
void InsertElement(struct HashMap *map, uint32_t key, char fn, int freq, char keys);
int Delete(struct HashMap *map);

#endif
