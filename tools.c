#include "tools.h"

int isLowercase(char c) { return (c >= 'a' && c <= 'z'); }
int isUppercase(char c) { return (c >= 'A' && c <= 'Z'); }
char upperToLower(char l) { return (l-'A')+'a';} 

char getNextLetter(FILE* input){
    int status;
    char c;
    
    if (input == NULL) {
        fprintf(stderr, "Error: file not found\n");
        return '0';
    }
    
    status = fscanf(input, "%c", &c);
    if (status != EOF && status == 1) {    
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            return isUppercase(c) ? upperToLower(c) : c;  // Normalize directly
        } else {
            return '2'; // Non-alphabetic character
        }
    } else {
        return '1'; // EOF
    }
}

char getMostFrequentLetter(FILE *input){
    rewind(input);
    int letterCount[26] = {0};
    char c;
    int mostFrequentIndex = 0;
    int i;
   
    // Count occurrences of each letter
    while ((c = getNextLetter(input)) != '1' && c != '0') {
        if (c != '2') {
            letterCount[c - 'a']++;
        }
    }

    // Find the index of the most frequent letter
    for (i = 0; i < 26; i++) {
        if (letterCount[i] > letterCount[mostFrequentIndex]) {
            mostFrequentIndex = i;
        }
    }
    return mostFrequentIndex + 'a';
}

int countTotalLetters(FILE * file){
    int counter = 0;
    char c;
    rewind(file);
    
    while (fscanf(file, "%c", &c) == 1) {
        if (isLowercase(c)) { // Count only lowercase letters
            counter++;
        }
    }
    return counter;
}

int countSpecificLetter(FILE * file, char targetLetter){
    int counter = 0;
    char currentLetter;
    rewind(file);
    
    while (fscanf(file, "%c", &currentLetter) == 1) {
        if (currentLetter == targetLetter) {
            counter++;
        }
    }
    return counter;
}

void extractSubsequence(FILE *input, FILE *output, int keyLength, int startOffset)
{
    rewind(input);
    char c;
    int position = 0;
    
    // Position at desired start and read every keyLength-th character
    while (fscanf(input, "%c", &c) == 1) {
        if (position >= startOffset && (position - startOffset) % keyLength == 0) {
            fprintf(output, "%c", c);
        }
        position++;
    }
}
