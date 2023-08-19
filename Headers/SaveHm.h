#ifndef SAVE_HM_H
#define SAVE_HM_H
#include "../Headers/HashTable.h"

struct Data
{
    uint32_t key;
    char letter;
    int freq;
    int mark;
    uint32_t empty;
};

void WriteHm(struct HashMap *map, char *filename);
struct HashMap *ReadHm(char *filename);

#endif
