#include <stdlib.h>
#include <string.h>
#include "../Headers/SaveHm.h"
#include "../Headers/Compression.h"
#include "../Headers/Decompress.h"
#include "../Headers/HashTable.h"

int main(int argc, char *argv[])
{
    int n = 0;
    struct HashMap *map = NULL;
    map = ReadHm("Path to file data on binary file");
    struct HuffmanNode *star = NULL;
    struct HuffmanNode *parrent = NULL;
    // We load the data from the hash map about the file and make the leafe nodes for out tree//
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
                newstr[n] = temp->letter;
                newfreq[n] = temp->freq;
                parrent = huffmanNode(newstr[n], newfreq[n], NULL, NULL);
                star = HuffInsertion(parrent, star, parrent->frequency);
                n++;
                temp = temp->next;
            }
        }
    }

    char **code = NULL;
    star = huffmanTree(star);
    printHuffmanTree(star, 0);
    code = constructHuffmanCodes(star, 0);
    GenerateMsg(star, code, "Path to uncompressed file");
    Delete(map);
    free(newstr);
    free(newfreq);
    freeHuffmanTree(star);

    return 0;
}