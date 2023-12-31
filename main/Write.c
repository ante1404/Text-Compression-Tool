#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../Headers/SaveHm.h"
#include "../Headers/HashTable.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("Wrong number of argumnets\n");
        return 1;
    }
    char uncompressed_file[50] = {0};
    char binary_file_data[50] = {0};
    strcpy(uncompressed_file, argv[1]);
    strcpy(binary_file_data, argv[2]);

    char *str = NULL;
    FILE *DNA = fopen(uncompressed_file, "r");

    fseek(DNA, 0, SEEK_END);
    off_t file_size = ftell(DNA);
    fseek(DNA, 0, SEEK_SET);

    str = calloc(file_size + 1, sizeof(char));

    fread(str, 1, file_size, DNA);
    fclose(DNA);

    long long int n = strlen(str);

    unsigned int freq[256] = {0}; // initialize all frequencies to 0
    struct HashMap *map = CreateHM(256);
    // calculate frequency of each character

    for (long long int i = 0; i < n + 1; i++)
    {
        freq[(long long int)str[i]]++;
    }
    /*We insert all letter that appear in a file along with their frequencies to a hashmap.
    This step can be skiped and the code would have to be changed and it would still work but this is
    all information nececery for commpressing a file and decompressing it, its just easier to work
    with and the entire infromation about a file is only few kilobytes. The more uneque letter the more bytes will the file be.*/
    for (long long int i = 0; i < n; i++)
    {
        InsertElement(map, str[i], str[i], freq[(int)str[i]], str[i]);
    }

    WriteHm(map, binary_file_data);
    Delete(map);
    free(str);
    return 0;
}