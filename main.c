#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tools.h"

void normalizeFile(FILE* input, FILE* output)
{
    char c;
    rewind(input);

    while (fscanf(input, "%c", &c) == 1) {
        if (isUppercase(c)) {
            fprintf(output, "%c", upperToLower(c));
        } else if (isLowercase(c)) {
            fprintf(output, "%c", c);
        }
        else {
            // Ignore non-alphabetic characters
        }
    }
}


double calculateCoincidenceIndex(FILE *input) {
    long totalLetters = countTotalLetters(input);
    long letterOccurrences = 0;
    double coincidenceIndex = 0.0;

    if (totalLetters <= 1) {
        return 0.0;
    }

    for (int i = 0; i < 26; i++) {
        letterOccurrences = countSpecificLetter(input, 'a' + i);
        coincidenceIndex += (double)(letterOccurrences * (letterOccurrences - 1));
    }

    return coincidenceIndex / (totalLetters * (totalLetters - 1));
}

int findPasswordLength(FILE *input)
{
    FILE *subsequenceFile;
    double coincidenceIndex = 0.0;

    for (int i = 1; i <= MAX_PASSWORD_LENGTH; i++) {
        subsequenceFile = fopen("subsequence.txt", "w+");
        if (subsequenceFile == NULL) {
            perror("Error opening subsequence file");
            return -1;
        }

        extractSubsequence(input, subsequenceFile, i, 0);
        fclose(subsequenceFile);

        subsequenceFile = fopen("subsequence.txt", "r");
        if (subsequenceFile == NULL) {
            perror("Error opening subsequence file for reading");
            return -1;
        }

        coincidenceIndex = calculateCoincidenceIndex(subsequenceFile);
        fclose(subsequenceFile);

        printf("Key Length %d: Coincidence Index = %.6f\n", i, coincidenceIndex);

        if (coincidenceIndex >= MIN_COINCIDENCE_INDEX && coincidenceIndex <= MAX_COINCIDENCE_INDEX) {
            printf("Possible key length found: %d\n", i);
            return i;

        }
    }

    return 0; // No valid key length found
}

void findPasswordCharacters(FILE *normalizedText, FILE *subsequenceFile, char *password, int keyLength) {
    int i;
    int shift;

    // Find each character of the password
    printf("Finding password characters...\n");
    subsequenceFile = fopen("subsequence.txt", "w+");

    for (i=0; i< keyLength; i++) {
        rewind(normalizedText);
        rewind(subsequenceFile);

        // Extract the i-th subsequence
        extractSubsequence(normalizedText, subsequenceFile, keyLength, i);
        rewind(subsequenceFile);

        // Find the most frequent letter in the subsequence
        password[i] = getMostFrequentLetter(subsequenceFile);
    }
    fclose(subsequenceFile);

    // Calculate decryption key assuming 'e' is the most frequent letter
    printf("Calculating decryption key...\n");
    for (i=0; i< keyLength; i++) {
        shift = password[i] - 'e';
        if (shift < 0) {
            shift += 26;
        }
        password[i] = 'a' + shift;
    }

    printf("Derived password: ");
    for (i=0; i< keyLength; i++) {
        printf("%c", password[i]);
    }

    printf("\n\n");
}

void decrypt(FILE* input, FILE* output, char *password, int keySize) {
    rewind(input);
    char c;
    int letterIndex = 0;

    while(fscanf(input, "%c", &c) == 1) {
        int shift;

        if (isUppercase(c)) {
            shift = password[letterIndex % keySize] - 'a';
            fprintf(output, "%c", ((c - 'A' - shift + 26) % 26) + 'A');
            letterIndex++;
        }
        else if (isLowercase(c)) {
            shift = password[letterIndex % keySize] - 'a';
            fprintf(output, "%c", ((c - 'a' - shift + 26) % 26) + 'a');
            letterIndex++;
        }
        else {
            fprintf(output, "%c", c); // Non-alphabetic characters are copied as is
        }
    }
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n");
        return EXIT_FAILURE;
    }

    FILE* inputFile;
    FILE* normalizeText;
    FILE* subsequenceFile;
    FILE* outputFile;
    char password[MAX_PASSWORD_LENGTH];
    int keyLength;

    //open files
    inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    normalizeText = fopen("normalized.txt", "w+");
    if (normalizeText == NULL) {
        perror("Error opening normalized file");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    outputFile = fopen(argv[2], "w+");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        fclose(normalizeText);
        remove("normalized.txt");
        fclose(subsequenceFile);
        return EXIT_FAILURE;
    }

    // Step 1: Normalize the input file
    printf("Normalizing the input file...\n");
    normalizeFile(inputFile, normalizeText);

    // Step 2: Determine the key length
    printf("Determining the key length...\n");
    keyLength = findPasswordLength(normalizeText);
    printf("Determined key length: %d\n", keyLength);

    // Step 3: Find the password characters
    printf("Finding password characters...\n");
    findPasswordCharacters(normalizeText, subsequenceFile, password, keyLength);

    // Step 4 : Decrypt the text
    printf("Decrypting the text...\n");
    decrypt(inputFile, outputFile, password, keyLength);

    // Clean up
    fclose(inputFile);
    fclose(normalizeText);
    fclose(outputFile);
    remove("normalized.txt");
    remove("subsequence.txt");
    printf("Decryption complete. Output written to %s\n", argv[2]);
    return EXIT_SUCCESS;
}