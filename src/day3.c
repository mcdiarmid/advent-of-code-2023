#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>


#define LINEWIDTH 256
#define LINES 3
#define PART_2


int line_sum(
    const char buffer[LINES][LINEWIDTH]
)
{
    /*
    * 1. Find numbers in curr
    * 2. Check for touching symbols in prev, curr, next
    * 3. If check is true, add number to sum
    * 4. Return sum
    */
    regex_t num_reg;
    regex_t sym_reg;
    regmatch_t num_match;
    regmatch_t sym_match;

    if (
        regcomp(&num_reg, "([0-9]+)", REG_EXTENDED) |
        regcomp(&sym_reg, "([^0-9\\w.])", REG_EXTENDED)
    )
    {
        fprintf(stderr, "Failed to compile regex\n");
        exit(1);
    }

    unsigned long sum = 0, value = 0;
    int start_idx = 0, search_offset = 0;
#ifndef PART_2

    while (regexec(&num_reg, &buffer[1][start_idx], 1, &num_match, 0) == 0)
    {
        value = atoi(&buffer[1][start_idx + num_match.rm_so]);
        search_offset = start_idx + num_match.rm_so - (int)(start_idx + num_match.rm_so > 0);

        for (int row = 0; row < LINES; row++)
        {
            int search_result = regexec(&sym_reg, &buffer[row][search_offset], 1, &sym_match, 0);
            if (search_result == 0)
            {
                if ((search_offset + sym_match.rm_so) <= (start_idx + num_match.rm_eo))
                {
                    sum += value;
                    break;
                }
            }
        }
        start_idx += num_match.rm_eo;
    }
#else
    size_t lw = strlen(buffer[1]);

    // This time we're searching for symbols on outer loop
    while (regexec(&sym_reg, &buffer[1][start_idx], 1, &sym_match, 0) == 0)
    {
        start_idx += sym_match.rm_so;

        int touching_pns = 0;
        int touching[2] = {0};

        for (int row = 0; row < LINES; row++)
        {
            search_offset = 0;
            while (regexec(&num_reg, &buffer[row][search_offset], 1, &num_match, 0) == 0)
            {
                value = atoi(&buffer[row][search_offset + num_match.rm_so]);

                int lower_bound = start_idx - (int) (start_idx > 0);
                int upper_bound = start_idx + (int) (start_idx != (lw-1));
                if (
                    ((search_offset + num_match.rm_so) <= upper_bound) && 
                    ((search_offset + num_match.rm_eo - 1) >= lower_bound)
                )
                    touching[(++touching_pns)%2] = value;

                search_offset += num_match.rm_eo;
            }
        }

        if (touching_pns == 2)
            sum += touching[0] * touching[1];

        start_idx++;
    }
#endif
    regfree(&num_reg);
    regfree(&sym_reg);

    return sum;
}


int line_gr_sum(
    const char buffer[LINES][LINEWIDTH]
)
{
    /*
    * 1. Find numbers in curr
    * 2. Check for touching symbols in prev, curr, next
    * 3. If check is true, add number to sum
    * 4. Return sum
    */
    regex_t num_reg;
    regex_t sym_reg;
    regmatch_t num_match;
    regmatch_t sym_match;

    if (
        regcomp(&num_reg, "([0-9]+)", REG_EXTENDED) |
        regcomp(&sym_reg, "([*])", REG_EXTENDED)
    )
    {
        fprintf(stderr, "Failed to compile regex\n");
        exit(1);
    }

    int sym_idx = 0, search_offset = 0;
    unsigned long sum = 0, value = 0;
    size_t lw = strlen(buffer[1]);

#ifdef DEBUG
    printf("BUFFER:\n");
    for (int row = 0; row < LINES; row++)
    {
        if (!row)
        {
            printf("  ");
            for (int z=0; z<strlen(buffer[1]); z++)
                printf("%d",z%10);
            printf("\n");
        }
        printf("%d %s\n", row, buffer[row]);
    }
    printf("\n");
#endif
    // This time we're searching for symbols on outer loop
    while (regexec(&sym_reg, &buffer[1][sym_idx], 1, &sym_match, 0) == 0)
    {
        sym_idx += sym_match.rm_so;
        // printf("Gear in column %d\n", sym_idx);

        int touching_pns = 0;
        int touching[2] = {0};

        for (int row = 0; row < LINES; row++)
        {
            search_offset = 0;
            while (regexec(&num_reg, &buffer[row][search_offset], 1, &num_match, 0) == 0)
            {
                value = atoi(&buffer[row][search_offset + num_match.rm_so]);
                // printf("Value %lu at (%d, %lld) is ", value, row, search_offset + num_match.rm_so);

                int lower_bound = sym_idx - (int) (sym_idx > 0);
                int upper_bound = sym_idx + (int) (sym_idx != (lw-1));
                if (
                    ((search_offset + num_match.rm_so) <= upper_bound) && 
                    ((search_offset + num_match.rm_eo - 1) >= lower_bound)
                )
                {
                    // printf("touching!\n");
                    touching[(++touching_pns)%2] = value;
                }
                else
                {
                    // printf("NOT!\n");
                }
                search_offset += num_match.rm_eo;
            }
        }
        if (touching_pns == 2)
        {
            // printf("GR %lu -> %lu\n", sum, sum + (touching[0] * touching[1]));
            sum += touching[0] * touching[1];
        }

        sym_idx++;
    }

    regfree(&num_reg);
    regfree(&sym_reg);
    // printf("\n");

    return sum;
}


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

    // Need to store three lines, checking the middle one each time.
    char buffer[LINES][LINEWIDTH] = {0};
    unsigned long tally = 0;
 
    // Iterate through input file lines
    while (fgets(buffer[LINES-1], sizeof(buffer[LINES-1]), fileptr) || *buffer[1]) {
        buffer[LINES-1][strcspn(buffer[LINES-1], "\n")] = 0;
        tally += line_sum(buffer);

        // Roll lines
        memset(buffer[0], 0, sizeof(char) * LINEWIDTH);
        strcpy(buffer[0], buffer[1]);
        memset(buffer[1], 0, sizeof(char) * LINEWIDTH);
        strcpy(buffer[1], buffer[2]);
        memset(buffer[2], 0, sizeof(char) * LINEWIDTH);
    }
    printf("Tally: %lu\n", tally);

    // Close file, return successful exit
    fclose(fileptr);
    return 0;
}