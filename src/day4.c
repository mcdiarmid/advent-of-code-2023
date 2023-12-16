#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define CHAR_PER_NUM 3


bool value_in_arr(uint32_t value, uint32_t* array, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (array[i] == value)
        {
        printf("√");
            return true;
        }
    }
    printf("x");
    return false;
}


uint32_t parse_scorecard(const char* card)
{
    // Starting index
    char *ptr = strchr(card, ':') + 1;
    printf("%s\n", card);

    size_t split_idx = (size_t) (strchr(ptr, '|') - ptr);
    size_t n_winners = (split_idx - 1) / CHAR_PER_NUM;
    printf("Winners (%zu):", n_winners);

    // First loop - populate array of winning numbers
    uint32_t *winners;
    winners = malloc(n_winners * sizeof(*winners));

    for (size_t i=0; i < n_winners; i++)
    {   
        winners[i] = (uint32_t) atoi(&ptr[i*CHAR_PER_NUM]);
        printf(" %u", winners[i]);
    }
    ptr += split_idx + 1;
    printf("\nValues:");

    // Second loop - calculate score
    uint32_t matching_nums = 0;
    while (*ptr)
    {   
        uint32_t value = (uint32_t) atoi(ptr);
        printf(" %d", value);
        if (value_in_arr(value, winners, n_winners))
            matching_nums++;
        ptr += 3;
    }

    free(winners);
    printf("\nCard Matches: %d\n\n", matching_nums);
    return matching_nums;
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

    // Iterate through input file lines
    char line[256];
    int card_scores[256] = { 0 };
    int idx = 0;
    uint32_t score = 0;
    while (fgets(line, sizeof(line), fileptr)) {
        line[strcspn(line, "\n")] = 0;
#ifdef PART1
        score += 1UL << (parse_scorecard(line) - 1);
#else
        uint32_t score_i = parse_scorecard(line);
        card_scores[idx]++;
        for (uint32_t j = 1; j <= score_i; j++)
            card_scores[idx + j] += card_scores[idx];
        printf("%d %d %d\n", idx, card_scores[idx], score + card_scores[idx]);
        score += card_scores[idx++];
#endif
    }
    printf("Total: %u\n", score);

    // Close file, return successful exit
    fclose(fileptr);
    return 0;
}