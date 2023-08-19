#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "../Headers/SaveHm.h"
#include "../Headers/HashTable.h"
#include "../Headers/Compression.h"
#include "../Headers/Decompress.h"

int main()
{
    int h = 0;
    int lj = 0;
    FILE *compressed_data = fopen("Path tovcompressed file in binary format", "rb");
    fseek(compressed_data, 0, SEEK_END);
    off_t n = ftell(compressed_data);
    fseek(compressed_data, 0, SEEK_SET);
    FILE *decompressed_data = fopen("Path to uncompressed file", "w");
    struct HashMap *map = ReadHm("Path to file data in binary format");
    struct HuffmanNode *tree = NULL;
    struct HuffmanNode *star = NULL;
    struct HuffmanNode *parrent = NULL;
    char *newstr = (char *)malloc(256 * sizeof(char));
    long int *newfreq = (long int *)malloc(256 * sizeof(long int));
    for (int i = 0; i < map->size; i++)
    {
        struct bin *b = map->table[i];
        if (b->head)
        {
            struct Node *temp = b->head;
            while (temp)
            {
                newstr[lj] = temp->letter;
                newfreq[lj] = temp->freq;
                parrent = huffmanNode(newstr[lj], newfreq[lj], NULL, NULL);
                star = HuffInsertion(parrent, star, parrent->frequency);
                lj++;
                temp = temp->next;
            }
        }
    }
    star = huffmanTree(star);
    printf("%ld\n", n);
    if (n < 1000000)
    {
        DecompressSF(compressed_data, decompressed_data, star, n);
        return 0;
    }
    h = DecompressLF(compressed_data, decompressed_data, star, star->frequency);
    if (h < 1000000 && n != 0)
    {
        DecompressSF(compressed_data, decompressed_data, star, h);
    }
    fclose(compressed_data);
    fclose(decompressed_data);
    free(newfreq);
    free(newstr);
    Delete(map);
    freeHuffmanTree(star);

    return 0;
}