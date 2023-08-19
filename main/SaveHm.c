#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../Headers/HashTable.h"
#include "../Headers/SaveHm.h"

void WriteHm(struct HashMap *map, char *filename)
{

    FILE *HM = fopen(filename, "wb");

    if (HM == NULL)
    {
        return;
    }

    struct Node *temp = NULL;

    fwrite(&map->size, sizeof(uint32_t), 1, HM);
    for (int i = 0; i < map->size; i++)
    {
        struct bin *b = map->table[i];
        if (b->head != NULL)
        {
            temp = b->head;
            while (temp)
            {
                fwrite(&temp->key, sizeof(uint32_t), 1, HM);
                fwrite(&temp->letter, sizeof(char), 1, HM);
                fwrite(&temp->freq, sizeof(int), 1, HM);
                temp = temp->next;
            }
        }
    }
    fclose(HM);
}

struct HashMap *ReadHm(char *filename)
{

    FILE *fp1 = NULL;
    fp1 = fopen(filename, "rb");

    uint32_t size = 0;
    fread(&size, sizeof(uint32_t), 1, fp1);

    struct Data *data = NULL;
    data = (struct Data *)calloc(size, sizeof(struct Data));
    int i = 0;

    while (fread(&data[i].key, sizeof(uint32_t), 1, fp1))
    {
        fread(&data[i].letter, sizeof(char), 1, fp1);
        fread(&data[i].freq, sizeof(int), 1, fp1);
        data[i].empty = 1;
        i++;
    }
    struct HashMap *Table = NULL;
    Table = CreateHM(size);
    for (int i = 0; i < size; i++)
    {
        if (data[i].empty == 1)
        {
            InsertElement(Table, data[i].letter, data[i].letter, data[i].freq, data[i].letter);
        }
    }
    fclose(fp1);
    free(data);

    return Table;
}
