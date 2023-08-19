#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/SaveHm.h"
#include "../Headers/Compression.h"

// Indexes for the GenerateMsg function
int m = 0;
long long u = 0;

struct HuffmanNode *huffmanNode(char letter, int frequency, struct HuffmanNode *left, struct HuffmanNode *right)
{

    struct HuffmanNode *newnode = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
    if (newnode == NULL)
    {
        return NULL;
    }
    newnode->letter = letter;
    newnode->frequency = frequency;
    newnode->left = left;
    newnode->right = right;
    newnode->next = NULL;

    return newnode;
}

struct HuffmanNode *huffmanTree(struct HuffmanNode *root)
{
    struct HuffmanNode *parrent = NULL;
    struct HuffmanNode *right = NULL;
    struct HuffmanNode *left = NULL;

    while (root->next)
    {
        int newfreq = root->frequency + root->next->frequency;                      // 2 lowest frequencys
        left = huffmanNode(root->letter, root->frequency, root->left, root->right); // Make copyes of the first 2 nodes
        right = huffmanNode(root->next->letter, root->next->frequency, root->next->left, root->next->right);
        parrent = huffmanNode('*', newfreq, left, right);        // Create new node that is a parent to the first 2
        root = Deletion(root);                                   // Delete 2 first nodes
        root = HuffInsertion(parrent, root, parrent->frequency); // Insert the node back to the pq
    }

    return root;
}

struct HuffmanNode *HuffInsertion(struct HuffmanNode *newnode, struct HuffmanNode *root, int priority)
{
    // This function is effeectivly the same as the one in the uneque.c but the diffrience is the data structire we are working with
    struct HuffmanNode *temp = NULL;

    if (root == NULL)
    {
        root = newnode;
        return root;
    }
    if (newnode->frequency < root->frequency)
    {
        newnode->next = root;
        root = newnode;
    }
    else
    {
        temp = root;
        while (temp->next && temp->next->frequency <= priority)
        {
            temp = temp->next;
        }
        newnode->next = temp->next;
        temp->next = newnode;
    }

    return root;
}

struct HuffmanNode *Deletion(struct HuffmanNode *root)
{
    struct HuffmanNode *temp1 = NULL;
    struct HuffmanNode *temp2 = NULL;
    if (root == NULL || root->next == NULL)
    {
        return root;
    }

    temp1 = root;
    temp2 = root->next;

    root = temp2->next;
    free(temp1);
    free(temp2);

    return root;
}

void printHuffmanTree(struct HuffmanNode *root, int level)
{
    if (root != NULL)
    {
        printHuffmanTree(root->right, level + 1);
        for (int i = 0; i < level; i++)
        {
            printf("\t");
        }
        printf("%c (%d)\n", root->letter, root->frequency);
        printHuffmanTree(root->left, level + 1);
    }
}

char **constructHuffmanCodes(struct HuffmanNode *root, int level)
{
    static char *code[256] = {0}; // Array to store codes
    static char buffer[256];      // Buffer for temporary code storage

    if (root->letter != '*')
    {
        code[root->letter] = strdup(buffer); // Copy buffer to code
        return code;
    }

    // Traverse right subtree
    buffer[level] = '1';
    buffer[level + 1] = '\0';
    constructHuffmanCodes(root->right, level + 1);

    // Traverse left subtree
    buffer[level] = '0';
    buffer[level + 1] = '\0';
    constructHuffmanCodes(root->left, level + 1);

    return code;
}

void GenerateMsg(struct HuffmanNode *root, char *code[], char orgMSg[])
{

    char *str = NULL;
    long file_size;
    FILE *DNA = fopen(orgMSg, "r");

    fseek(DNA, 0, SEEK_END);
    file_size = ftell(DNA);
    fseek(DNA, 0, SEEK_SET);

    str = calloc(file_size + 1, sizeof(char));

    fread(str, 1, file_size, DNA);
    fclose(DNA);
    unsigned long long int n = strlen(str);
    printf("%lld\n", n);
    int *msg = (int *)calloc((1000000), sizeof(int));
    FILE *hun = fopen("Path to compressed file in binary format", "wb");
    /*Now for each charater in original message we traverse the inner loop to find its
    coresponding binary code that was constructed by huffman tree and we converti it to int and store it to an msg array
    The reason we allocate only 1 milion bytes and free and repeate is because you can allocate gbs of
    data to do it all at one unless you have supercomputer*/
    for (unsigned long long int i = 0; i < n; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (code[j] != NULL)
            {
                if (str[i] == j)
                {
                    for (int lj = 0; code[j][lj] != '\0'; lj++)
                    {
                        u++;
                        if (m == 1000000)
                        {
                            for (int i = 0; i < m; i++)
                            {
                                // this function does the intiger conversion to is binary reprenstation and writes it to a fille thus compressing the data
                                writeBit(hun, msg[i], 0);
                            }
                            free(msg);
                            msg = (int *)calloc((1000000), sizeof(int));
                            m = 0;
                        }

                        if (code[j][lj] == '0')
                        {
                            msg[m] = 0;
                            m++;
                        }
                        else if (code[j][lj] == '1')
                        {
                            msg[m] = 1;
                            m++;
                        }
                    }
                    break;
                }
            }
        }
    }
    if (u % 8 != 0)
    {
        /*this checks if we have enough bytes to write the last byte or we have to pad it.
        Because we cant write 123.4 bytes to disck*/
        for (int i = 0; i < m; i++)
        {
            writeBit(hun, msg[i], 0);
        }
        writeBit(hun, msg[m], 1);
    }
    else
    {
        for (int i = 0; i < m; i++)
        {
            writeBit(hun, msg[i], 0);
        }
    }
    fclose(hun);
    free(str);
    free(msg);
}

void writeBit(FILE *file, int bit, int i)
{
    /*This function writes bytes to a file, each 8 bits we write 1 byte, if the
    inut is 1,0,0,1,1,0,1,0 then the byte we write will be 10011010, this
    obviusly saves space as this is the whole point of it insted of just writing
    10000000 and 00000000 for each of those inputs, thus compression would not be possible*/
    static int currentByte = 0;
    static int bitCount = 8;

    if (bitCount == 8)
    {
        currentByte = 0;
    }

    currentByte |= (bit << bitCount);
    bitCount--;

    if (bitCount == 0)
    {
        currentByte = currentByte >> 1;
        fwrite(&currentByte, sizeof(unsigned char), 1, file);
        bitCount = 8;
    }
    if (i == 1 && bitCount != 8)
    {
        /*The argument i acts like a boolean, where 1 means we still have few bites but not
        enough to have 1 8 bit byte, so we pad the remaning spaces with zeros, you might think this can
        be a probelm if we have a letter on location 000 with huffman code and when we
        get to the last byte the last the function in decoding program will itterate the tree
        with false data (padded zeros) and we might get the, but condition in DecompressSF h >= NumOfLetters keeps that from happening*/
        
        while (bitCount != 0)
        {
            currentByte |= (bit << bitCount);
            bitCount--;
        }
        currentByte = currentByte >> 1;
        fwrite(&currentByte, sizeof(unsigned char), 1, file);
    }
}

void freeHuffmanTree(struct HuffmanNode *node)
{
    if (node == NULL)
    {
        return; // Base case: nothing to free
    }

    freeHuffmanTree(node->left);  // Free left subtree
    freeHuffmanTree(node->right); // Free right subtree
    free(node);                   // Free current node

    node = NULL;
}
