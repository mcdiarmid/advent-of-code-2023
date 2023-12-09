#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char* argv[])
{
    // Exit if no input provided
    if (argc < 2) {
        printf("Please provide /path/to/file as input.");
        exit(1);
    }

    // Open file and check success
    FILE* fileptr = fopen(argv[1], "r");
    if (fileptr == NULL)
    {
        printf("Failed to open file %s", argv[1]);
        exit(1);
    }

    // Iterate through input file lines
    char line[256];
    while (fgets(line, sizeof(line), fileptr)) {
        line[strcspn(line, "\n")] = 0;
        /* Code */
    }

    // Close file, return successful exit
    fclose(fileptr);
    return 0;
}