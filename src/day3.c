#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>


#define LINEWIDTH 256
#define LINES 3
// #define DEBUG

int line_pn_sum(
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

    int nptr_ofs = 0, sptr_ofs = 0;
    unsigned long sum = 0, value = 0;

#ifdef DEBUG
    printf("BUFFER:\n");
    for (int rel_idx = 0; rel_idx < LINES; rel_idx++)
    {
        if (!rel_idx)
        {
            printf("  ");
            for (int z=0; z<strlen(buffer[1]); z++)
                printf("%d",z%10);
            printf("\n");
        }
        printf("%d %s\n", rel_idx, buffer[rel_idx]);
    }
    printf("\n");
#endif

    while (regexec(&num_reg, &buffer[1][nptr_ofs], 1, &num_match, 0) == 0)
    {
        value = atoi(&buffer[1][nptr_ofs + num_match.rm_so]);
        sptr_ofs = nptr_ofs + num_match.rm_so - (int)(nptr_ofs + num_match.rm_so > 0);

        for (int rel_idx = 0; rel_idx < LINES; rel_idx++)
        {
            int search_result = regexec(&sym_reg, &buffer[rel_idx][sptr_ofs], 1, &sym_match, 0);
            if (search_result == 0)
            {
                if ((sptr_ofs + sym_match.rm_so) <= (nptr_ofs + num_match.rm_eo))
                {
                    sum += value;
                    break;
                }
            }
        }
        nptr_ofs += num_match.rm_eo;
    }

    regfree(&num_reg);
    regfree(&sym_reg);

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
        tally += line_pn_sum(buffer);

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