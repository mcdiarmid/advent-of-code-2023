#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>


#define N_COLORS 3
#define PART_2
const char* COLORS[N_COLORS] = {"red", "green", "blue"};
const unsigned long CMAX[N_COLORS] = {12, 13, 14};

typedef struct rgb {unsigned long buf[N_COLORS];} rgb_t;


int get_game_id(const char *input) 
{
    // Get the Game ID
    regex_t greg;
    char* pattern = "([0-9]+)";
    regmatch_t id_match;

    if (regcomp(&greg, pattern, REG_EXTENDED))
    {
        fprintf(stderr, "Failed to compile regex\n");
        exit(1);
    }
    
    if (regexec(&greg, input, 1, &id_match, 0) != 0)
    {
        fprintf(stderr, "Line \"%s\" does not match regex\n.", input);
        exit(1);
    }
    regfree(&greg);
    return atoi(&input[id_match.rm_so]);
}


rgb_t get_minimum_color_count(const char *input)
{
    // Compile regex for a quick forward search
    regex_t entry_reg;
    char *pattern = "([0-9]+) (red|green|blue)(, |; )?";
    regmatch_t entry_match;

    if (regcomp(&entry_reg, pattern, REG_EXTENDED))
    {
        fprintf(stderr, "Failed to compile regex\n");
        exit(1);
    }

    char *ptr = strchr(input, ':');
    char *cptr;
    rgb_t min_count = { .buf = {0, 0, 0} };

    while (*ptr && (regexec(&entry_reg, ptr, 1, &entry_match, 0) == 0)) 
    {
        ptr += entry_match.rm_so;
        int x = atoi(ptr);
        ptr = strchr(ptr, ' ') + 1;
    
        for (int ci = 0; ci < N_COLORS; ci++)
        {
            if ((cptr = strstr(ptr, COLORS[ci])) != NULL && ((cptr - ptr) == 0))
            {
                min_count.buf[ci] = (x > min_count.buf[ci]) ? x : min_count.buf[ci];
                ptr += strlen(COLORS[ci]);
            }
        }
    }
    regfree(&entry_reg);

    return min_count;
}


unsigned long get_game_score(rgb_t* min_count)
{
    unsigned long product = 1;
    for (int ci = 0; ci < N_COLORS; ci++)
        product *= min_count->buf[ci];
    return product;
}


bool is_game_valid(rgb_t* min_count)
{
    for (int ci = 0; ci < N_COLORS; ci++)
        if (min_count->buf[ci] > CMAX[ci])
            return false;

    return true;
}


int main(int argc, char *argv[])
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
    unsigned long gid, valid_id_sum, game_power_sum;

    while (fgets(line, sizeof(line), fileptr)) {
        line[strcspn(line, "\n")] = 0;
        int gid = get_game_id(line);
        rgb_t min_color_count = get_minimum_color_count(line);

        if (is_game_valid(&min_color_count)) 
            valid_id_sum += gid;
        
        game_power_sum += get_game_score(&min_color_count);
    }
    printf("Valid Game ID Sum: %lu\n", valid_id_sum);
    printf("Game Power Sum: %lu\n", game_power_sum);

    // Close file, return successful exit
    fclose(fileptr);
    return 0;
}