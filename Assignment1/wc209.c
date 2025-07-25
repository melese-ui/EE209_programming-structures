/*-------------------------------------
 Name: Melese Medhin
 Student Id: 20210727
 Assignment 1
 file: wc209.c
-------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Enumerated type defining states for the lexical DFA
typedef enum {
    ST_START,
    ST_OUTSIDE_WORD,
    ST_INSIDE_WORD,
    ST_SLASH,
    ST_COMMENT,
    ST_ASTERISK,
    ST_COMMENT_END
} DFAState;

// Global state variables and counters
DFAState currentState = ST_START;
DFAState previousState = ST_START;
int lineCount = 0, wordCount = 0, charCount = 0;
int commentStartLine = 0;
int inputChar;

// Function declarations for state handlers
DFAState handleStart(int c);
DFAState handleOutsideWord(int c);
DFAState handleInsideWord(int c);
DFAState handleSlash(int c);
DFAState handleComment(int c);
DFAState handleAsterisk(int c);
DFAState handleCommentEnd(int c);

int main() {
    while ((inputChar = getchar()) != EOF) {
        switch (currentState) {
            case ST_START:
                lineCount = 1;  // Initialize line count on first input
                currentState = handleStart(inputChar);
                previousState = ST_START;
                break;

            case ST_OUTSIDE_WORD:
                currentState = handleOutsideWord(inputChar);
                previousState = ST_OUTSIDE_WORD;
                break;

            case ST_INSIDE_WORD:
                currentState = handleInsideWord(inputChar);
                previousState = ST_INSIDE_WORD;
                break;

            case ST_SLASH:
                currentState = handleSlash(inputChar);
                previousState = ST_SLASH;
                break;

            case ST_COMMENT:
                currentState = handleComment(inputChar);
                previousState = ST_COMMENT;
                break;

            case ST_ASTERISK:
                currentState = handleAsterisk(inputChar);
                previousState = ST_ASTERISK;
                break;

            case ST_COMMENT_END:
                currentState = handleCommentEnd(inputChar);
                previousState = ST_COMMENT_END;
                break;

            default:
                // Should not reach here, but reset if it does
                currentState = ST_START;
                previousState = ST_START;
                break;
        }
    }

    // Handle word count increment if last state was slash awaiting word or start/outside/left states
    if (currentState == ST_SLASH) {
        charCount++;
        if (previousState == ST_START || previousState == ST_OUTSIDE_WORD || previousState == ST_COMMENT_END) {
            wordCount++;
        }
    }

    // Check for unterminated comment error
    if ((currentState == ST_COMMENT || currentState == ST_ASTERISK) && inputChar == EOF) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", commentStartLine);
        return EXIT_FAILURE;
    } 

    printf("%d %d %d\n", lineCount, wordCount, charCount);
    return EXIT_SUCCESS;
}

// Handler for state ST_START
DFAState handleStart(int c) {
    if (isspace(c)) {
        charCount++;
        if (c == '\n') lineCount++;
        return ST_OUTSIDE_WORD;
    }
    else if (c == '/') {
        return ST_SLASH;
    }
    else {
        wordCount++;
        charCount++;
        return ST_INSIDE_WORD;
    }
}

// Handler for state ST_OUTSIDE_WORD
DFAState handleOutsideWord(int c) {
    if (isspace(c)) {
        charCount++;
        if (c == '\n') lineCount++;
        return ST_OUTSIDE_WORD;
    }
    else if (c == '/') {
        return ST_SLASH;
    }
    else {
        wordCount++;
        charCount++;
        return ST_INSIDE_WORD;
    }
}

// Handler for state ST_INSIDE_WORD
DFAState handleInsideWord(int c) {
    if (isspace(c)) {
        charCount++;
        if (c == '\n') lineCount++;
        return ST_OUTSIDE_WORD;
    }
    else if (c == '/') {
        return ST_SLASH;
    }
    else {
        charCount++;
        return ST_INSIDE_WORD;
    }
}

// Handler for state ST_SLASH, handling '/' character seen
DFAState handleSlash(int c) {
    if (isspace(c)) {
        charCount++;
        if (c == '\n') lineCount++;
        if (previousState == ST_START || previousState == ST_OUTSIDE_WORD || previousState == ST_COMMENT_END) {
            wordCount++;
            charCount++;
        } else if (previousState == ST_INSIDE_WORD || previousState == ST_SLASH) {
            charCount++;
        }
        return ST_OUTSIDE_WORD;
    }
    else if (c == '*') {
        charCount++;
        commentStartLine = lineCount;
        return ST_COMMENT;
    }
    else if (c == '/') {
        charCount++;
        if (previousState == ST_START || previousState == ST_OUTSIDE_WORD || previousState == ST_COMMENT_END) {
            wordCount++;
        }
        return ST_SLASH;
    }
    else {
        charCount++;
        if (previousState == ST_START || previousState == ST_OUTSIDE_WORD || previousState == ST_COMMENT_END) {
            wordCount++;
            charCount++;
        }
        else if (previousState == ST_INSIDE_WORD || previousState == ST_SLASH) {
            charCount++;
        }
        return ST_INSIDE_WORD;
    }
}

// Handler for comments inside '/* ... */'
DFAState handleComment(int c) {
    if (c == '*') {
        return ST_ASTERISK;
    }
    else {
        if (c == '\n') {
            lineCount++;
            charCount++;
        }
        return ST_COMMENT;
    }
}

// Handler after seeing '*' inside a comment (possible end of comment)
DFAState handleAsterisk(int c) {
    if (c == '/') {
        return ST_COMMENT_END;
    }
    else if (c == '*') {
        return ST_ASTERISK;
    }
    else {
        if (c == '\n') {
            lineCount++;
            charCount++;
        }
        return ST_COMMENT;
    }
}

// Handler after exiting comment block (seen '*/')
DFAState handleCommentEnd(int c) {
    if (c == '/') {
        return ST_SLASH;
    }
    else if (isspace(c)) {
        charCount++;
        if (c == '\n') lineCount++;
        return ST_OUTSIDE_WORD;
    }
    else {
        charCount++;
        wordCount++;
        return ST_INSIDE_WORD;
    }
}
