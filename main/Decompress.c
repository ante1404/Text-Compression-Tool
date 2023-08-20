#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "../Headers/SaveHm.h"
#include "../Headers/HashTable.h"
#include "../Headers/Compression.h"
#include "../Headers/Decompress.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        perror("Wrong number of argumnets\n");
        return 1;
    }
    char uncompressed_file[50] = {0};
    char binary_file_data[50] = {0};
    char compressed_file[50] = {0};
    strcpy(uncompressed_file, argv[1]);
    strcpy(binary_file_data, argv[2]);
    strcpy(compressed_file, argv[3]);
    int h = 0;
    int lj = 0;
    FILE *compressed_data = fopen(compressed_file, "rb");
    fseek(compressed_data, 0, SEEK_END);
    off_t n = ftell(compressed_data);
    fseek(compressed_data, 0, SEEK_SET);
    FILE *decompressed_data = fopen(uncompressed_file, "w");
    struct HashMap *map = ReadHm(binary_file_data);
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