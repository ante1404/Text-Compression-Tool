#ifndef DECOMPRESS_H
#define DECOMPRESS_H
#include <stdio.h>
#include <stdint.h>
#include "../Headers/Compression.h"

#define ERROR_CHAR '\0'

int DecompressLF(FILE *compressed_data, FILE *decompressed_data, struct HuffmanNode *tree, long long int numOfLettes);
int DecompressSF(FILE *compressed_data, FILE *decompressed_data, struct HuffmanNode *tree, long long int fileSize);
int *DtB(int number);
int **FBtD(uint8_t arr[], int n);
char DecodeMsg(struct HuffmanNode *root, int **arr, int n);
#endif
