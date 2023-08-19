#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../Headers/SaveHm.h"
#include "../Headers/HashTable.h"
#include "../Headers/Compression.h"
#include "../Headers/Decompress.h"

int *DtB(int number)
{
    /*Converts the read bits from decimal numbers to binary effectivly back to its orginal form*/
    int i = 7;
    int *result = NULL;
    result = (int *)malloc((i + 1) * sizeof(int));

    while (number != 0 || i >= 0)
    {
        result[i] = number % 2;
        number /= 2;
        i--;
    }

    return result;
}

int **FBtD(uint8_t arr[], int n)
{
    /*working with 2d array beacuse its is easier to allocate memory for
    1 milion rows x 8 collums instead 8 milion bytes 1d array.*/
    int **bfile = (int **)calloc(n, sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        bfile[i] = DtB(arr[i]);
    }

    return bfile;
}

char DecodeMsg(struct HuffmanNode *root, int **arr, int n)
{
    /*Obviusly traverses the tree untill it finds a letter, indexes are static because we need to remembere the last
    known position in 2d array of bits after function calls*/
    static int i = 0;
    static int j = 0;
    static struct HuffmanNode *temp = NULL;
    if (temp != NULL)
    {
        // Here we just set it to last position if the function return ERROR CHAR in middle of the tree
        root = temp;
        temp = NULL;
    }
    while (root->letter == '*')
    {
        if (j > 7)
        {
            i++;
            j = 0;
        }
        if (i > n - 1)
        {
            if (root->letter == '*')
            {
                /*This condition assures that if the we reach the end of array and we didnt still find a letter but are somwhere in the tree,
                that we rememeber our last position once we allocate and load next milion of remaining bits*/
                temp = root;
            }
            j = 0;
            i = 0;
            return ERROR_CHAR;
        }
        if (arr[i][j] == 1)
        {
            root = root->right;
        }
        else
        {
            root = root->left;
        }
        j++;
    }

    return root->letter;
}

int DecompressSF(FILE *compressed_data, FILE *decompressed_data, struct HuffmanNode *tree, long long int fileSize)
{

    /*This function is for decompressing files equall or smaller than 1 milion bytes
    because of memory on my machine. So, for files this size we can dinamicly approcah the problem*/
    off_t m = ftell(decompressed_data); /*This is nesecery to know how much data exacly do we have left to write so we dont over
allocate of write more data then we should by those padded bits*/
    int numOfLetters = tree->frequency - m;
    uint8_t *arr2 = NULL;
    int **bfile2 = NULL;
    char *msg2 = NULL;
    arr2 = (uint8_t *)calloc(fileSize, sizeof(uint8_t));
    fread(arr2, sizeof(uint8_t), fileSize, compressed_data);
    bfile2 = FBtD(arr2, fileSize);
    msg2 = (char *)calloc(numOfLetters + 1, sizeof(char));
    int h = 0;
    while (1)
    {
        msg2[h] = DecodeMsg(tree, bfile2, fileSize);
        if (msg2[h] == ERROR_CHAR || h >= numOfLetters) // The condition i mention in WriteBit function in Huffman.c
        {
            break;
        }
        fputc(msg2[h], decompressed_data);
        h++;
    }
    for (int j = 0; j < fileSize; j++)
    {
        free(bfile2[j]);
    }
    free(bfile2);
    free(msg2);
    free(arr2);
    bfile2 = NULL;
    msg2 = NULL;
    arr2 = NULL;

    return 0;
}

int DecompressLF(FILE *compressed_data, FILE *decompressed_data, struct HuffmanNode *tree, long long int numOfLettes)
{

    fseek(compressed_data, 0, SEEK_END);
    off_t n = ftell(compressed_data);
    rewind(compressed_data);

    int **bfile = NULL;
    char *msg = NULL;
    uint8_t *arr = NULL;

    while (n >= 1000000)
    {
        int i = 0;
        arr = (uint8_t *)calloc(1000000, sizeof(uint8_t));
        fread(arr, sizeof(uint8_t), 1000000, compressed_data);
        bfile = FBtD(arr, 1000000);
        msg = (char *)calloc(1000000 * 4, sizeof(char));
        int h = 0;
        while (1)
        {
            msg[h] = DecodeMsg(tree, bfile, 1000000);
            if (msg[h] == ERROR_CHAR || h >= 1000000 * 4) /*this  || constion is here just as a safety mecahnisam if
            something else is wrong, so it will never  actualy we executed unless all letters in this 1 milion chunk
            each bit represents 1 letter, which is highly unlikly*/
            {
                break;
            }
            fputs(&msg[h], decompressed_data);
            h++;
        }
        for (int i = 0; i < 1000000; i++)
        {
            free(bfile[i]);
        }
        free(arr);
        free(bfile);
        free(msg);
        arr = NULL;
        msg = NULL;
        bfile = NULL;

        n -= 1000000;
    }

    return n;
}