#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define NON_ZERO_DIGIS 9
const char* DIGITS[NON_ZERO_DIGIS] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
};


bool is_digit(const char c){
    return ((c >= '0') && (c <= '9'));
}


void update_first_last(int* first, int* last, int value){
    if (*first == 0)
        *first = value;
    *last = value;
}


int calibration_value(const char* input, bool check_words)
{
    char* ptr;
    int first = 0, last = 0;

    for (int ii=0; ii<strlen(input); ii++)
    {
        // Check for digit char at current index
        if (is_digit(input[ii]))
        {
            update_first_last(&first, &last, (int) (input[ii] - '0'));
        }
        // Check for digit words at current index
        else if (check_words)
        {
            for (int d=0; d<NON_ZERO_DIGIS; d++)
            {
                if (strlen(DIGITS[d]) > strlen(&input[ii]))
                    continue;

                if (
                    (ptr = strstr(&input[ii], DIGITS[d])) != NULL && 
                    (ptr - &input[ii] == 0)
                )
                {
                    update_first_last(&first, &last, d + 1);
                    break;
                }
            }
        }
    }
    return (first * 10) + last;
}


int main(int argc, char* argv[])
{
    bool check_words = false;
    if (argc < 2) {
        printf("Please provide /path/to/file as input.");
        exit(1);
    }
    else if (argc >= 3)
    {
        check_words = true;
    }

    FILE* fileptr = fopen(argv[1], "r");

    if (fileptr == NULL)
    {
        printf("Failed to open file %s", argv[1]);
        exit(1);
    }

    // Iterate through lines of the input file, update sum
    int sum;
    char line[256];

    while (fgets(line, sizeof(line), fileptr)) {
        line[strcspn(line, "\n")] = 0;
        int value = calibration_value(line, check_words);
        sum += value;
        printf("%s -> %d\n", line, value);
    }

    printf("Sum: %d\n", sum);
    fclose(fileptr);

    return 0;
}