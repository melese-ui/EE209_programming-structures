/************************************
 Name: Melese Medhin
 Student Id: 20210727
 Assignment 2
 file: sgrep.c
 ***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For skeleton code
#include <unistd.h>  // For getopt
#include "str.h"

#define ARG_FIND    "-f"
#define ARG_REPLACE "-r"
#define ARG_DIFF    "-d"

#define MAX_LEN 1023

#define SUCCESS 1
#define FAILURE 0

typedef enum {
    CMD_INVALID = 0,
    CMD_FIND,
    CMD_REPLACE,
    CMD_DIFF
} CmdType;


/*-------------------------------------------------------------------*/
/* ShowUsage:
   Displays usage instructions for the Simple Grep tool            */
/*-------------------------------------------------------------------*/
void ShowUsage(const char *programName) {
    const char *usageText =
        "Simple Grep (sgrep) Usage:\n"
        "%s [COMMAND] [OPTIONS]...\n"
        "\nCOMMANDS\n"
        "\tFind:    -f [search-string]\n"
        "\tReplace: -r [string1] [string2]\n"
        "\tDiff:    -d [file1] [file2]\n";

    printf(usageText, programName);
}


/*-------------------------------------------------------------------*/
/* ExecuteFind:
   - Validates the search string's length (<= MAX_LEN)
   - Reads lines from stdin, validating line length (<= MAX_LEN)
   - Prints lines containing the search string to stdout
   - Prints errors to stderr for invalid inputs or lines too long
   - Returns SUCCESS or FAILURE                             */
/*-------------------------------------------------------------------*/
int ExecuteFind(const char *searchStr) {
    char lineBuffer[MAX_LEN + 2];
    int searchStrLen = StrGetLength(searchStr);

    if (searchStrLen > MAX_LEN) {
        fprintf(stderr, "Error: argument is too long\n");
        return FAILURE;
    }

    while (fgets(lineBuffer, sizeof(lineBuffer), stdin)) {
        int lineLen = StrGetLength(lineBuffer);

        if (lineLen > MAX_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FAILURE;
        }

        if (StrSearch(lineBuffer, searchStr) != NULL) {
            // Print line with or without newline
            if (StrSearch(lineBuffer, "\n") != NULL)
                printf("%s", lineBuffer);
            else
                printf("%s\n", lineBuffer);
        }
    }

    return SUCCESS;
}


/*-------------------------------------------------------------------*/
/* ExecuteReplace:
   - Validates string arguments length (<= MAX_LEN)
   - Ensures the first string is not empty
   - Reads lines from stdin, validating line length (<= MAX_LEN)
   - Replaces all occurrences of string1 with string2 and prints result
   - Prints error messages for invalid arguments or input lines
   - Returns SUCCESS or FAILURE                                    */
/*-------------------------------------------------------------------*/
int ExecuteReplace(const char *oldStr, const char *newStr) {
    if (StrGetLength(oldStr) > MAX_LEN || StrGetLength(newStr) > MAX_LEN) {
        fprintf(stderr, "Error: argument is too long\n");
        return FAILURE;
    }
    if (StrGetLength(oldStr) == 0) {
        fprintf(stderr, "Error: Can't replace an empty substring\n");
        return FAILURE;
    }

    char line[MAX_LEN + 2];

    while (fgets(line, sizeof(line), stdin)) {
        int lineLen = StrGetLength(line);
        if (lineLen > MAX_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FAILURE;
        }

        char *pos = line;
        int oldLen = StrGetLength(oldStr);

        int matchedIndex = 0;
        int i = 0;
        while (line[i] != '\0') {
            if (line[i] == oldStr[matchedIndex]) {
                matchedIndex++;
                if (matchedIndex == oldLen) {
                    // Full match found: print replacement string
                    printf("%s", newStr);
                    matchedIndex = 0;
                }
            } else {
                if (matchedIndex > 0) {
                    // Print the partially matched substring plus current char
                    for (int k = i - matchedIndex; k < i; ++k)
                        putchar(line[k]);
                    matchedIndex = 0;
                }
                putchar(line[i]);
            }
            i++;
        }
        // If any trailing partial match exists, print it literally
        if (matchedIndex > 0) {
            for (int k = i - matchedIndex; k < i; ++k)
                putchar(line[k]);
        }
    }

    return SUCCESS;
}


/*-------------------------------------------------------------------*/
/* ExecuteDiff:
   - Checks arguments’ length (<= MAX_LEN)
   - Opens both files, prints errors if fails
   - Reads lines from both files, validates length (<= MAX_LEN)
   - Compares lines; prints differing lines with line numbers
   - Prints error if one file ends earlier than the other
   - Returns SUCCESS or FAILURE                                 */
/*-------------------------------------------------------------------*/
int ExecuteDiff(const char *filename1, const char *filename2) {
    if (StrGetLength(filename1) > MAX_LEN || StrGetLength(filename2) > MAX_LEN) {
        fprintf(stderr, "Error: argument is too long\n");
        return FAILURE;
    }

    FILE *file1 = fopen(filename1, "r");
    if (!file1) {
        fprintf(stderr, "Error: failed to open file %s\n", filename1);
        return FAILURE;
    }
    FILE *file2 = fopen(filename2, "r");
    if (!file2) {
        fclose(file1);
        fprintf(stderr, "Error: failed to open file %s\n", filename2);
        return FAILURE;
    }

    char line1[MAX_LEN + 2];
    char line2[MAX_LEN + 2];
    int lineNumber = 0;

    while (1) {
        char *res1 = fgets(line1, sizeof(line1), file1);
        char *res2 = fgets(line2, sizeof(line2), file2);

        if (!res1 && !res2) {
            // Both files ended simultaneously
            break;
        }
        if (res1 && !res2) {
            fprintf(stderr, "Error: %s ends early at line %d\n", filename2, lineNumber);
            fclose(file1);
            fclose(file2);
            return FAILURE;
        }
        if (!res1 && res2) {
            fprintf(stderr, "Error: %s ends early at line %d\n", filename1, lineNumber);
            fclose(file1);
            fclose(file2);
            return FAILURE;
        }

        // Validate line lengths
        if (StrGetLength(line1) > MAX_LEN) {
            fprintf(stderr, "Error: input line %s is too long\n", filename1);
            fclose(file1);
            fclose(file2);
            return FAILURE;
        }
        if (StrGetLength(line2) > MAX_LEN) {
            fprintf(stderr, "Error: input line %s is too long\n", filename2);
            fclose(file1);
            fclose(file2);
            return FAILURE;
        }

        lineNumber++;

        // Compare lines
        if (StrCompare(line1, line2) != 0) {
            int len1 = StrGetLength(line1);
            int len2 = StrGetLength(line2);

            // Special case: lines differ only by trailing newline presence
            if (!((len1 + 1 == len2 && line2[len1] == '\n') ||
                  (len1 == len2 + 1 && line1[len2] == '\n'))) {
                printf("%s@%d:%s%s", filename1, lineNumber, line1,
                       (StrSearch(line1, "\n") == NULL) ? "\n" : "");
                printf("%s@%d:%s%s", filename2, lineNumber, line2,
                       (StrSearch(line2, "\n") == NULL) ? "\n" : "");
            }
        }
    }

    fclose(file1);
    fclose(file2);

    return SUCCESS;
}


/*-------------------------------------------------------------------*/
/* ValidateCommand:
   - Checks if command and argument count are correct
   - Returns command type enumeration or FAILURE                  */
/*-------------------------------------------------------------------*/
int ValidateCommand(int argc, const char *commandStr) {
    if (argc < 3) {
        return CMD_INVALID;
    }

    if (StrCompare(commandStr, ARG_FIND) == 0) {
        if (argc == 3)
            return CMD_FIND;
        else
            return FAILURE;
    }
    else if (StrCompare(commandStr, ARG_REPLACE) == 0) {
        if (argc == 4)
            return CMD_REPLACE;
        else
            return FAILURE;
    }
    else if (StrCompare(commandStr, ARG_DIFF) == 0) {
        if (argc == 4)
            return CMD_DIFF;
        else
            return FAILURE;
    }
    else {
        return CMD_INVALID;
    }
}


/*-------------------------------------------------------------------*/
/* main:
   - Parses command-line arguments
   - Calls appropriate function based on command
   - Returns EXIT_SUCCESS or EXIT_FAILURE accordingly              */
/*-------------------------------------------------------------------*/
int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: argument parsing error\n");
        ShowUsage(argv[0]);
        return EXIT_FAILURE;
    }

    int cmdType = ValidateCommand(argc, argv[1]);

    if (!cmdType) {
        fprintf(stderr, "Error: argument parsing error\n");
        ShowUsage(argv[0]);
        return EXIT_FAILURE;
    }

    int status = FAILURE;

    switch (cmdType) {
        case CMD_FIND:
            status = ExecuteFind(argv[2]);
            break;

        case CMD_REPLACE:
            status = ExecuteReplace(argv[2], argv[3]);
            break;

        case CMD_DIFF:
            status = ExecuteDiff(argv[2], argv[3]);
            break;

        default:
            fprintf(stderr, "Error: unknown command\n");
            ShowUsage(argv[0]);
            return EXIT_FAILURE;
    }

    return status ? EXIT_SUCCESS : EXIT_FAILURE;
}
