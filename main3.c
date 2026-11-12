#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SONGS 100 //everytime 'MAX_SONGS' is used in my code replaces it with 100 before compiling
#define MAX_STR 100 //everytime 'MAX_STR' is used in my code replaces it with 100 before compiling
#define FILENAME "catalogue.txt"//everytime 'FILENAME' is used in my code 'catalogue.txt' will be used in its place before compiling

// creating a custom structure to store the details for one song, 'a struct' can group different types of data under a single name
struct Song {
    char band[MAX_STR]; //sets aside a row of 100 consecutive 1-byte memory slots specifically for the bands name
    char title[MAX_STR];//sets aside a row of 100 consecutive 1-byte memory slots specifically for the bands name
    float price;
    int year;
};

// Declaring functions
//using int count is to pass a copy of the number, where the functions only need to know how many songs exist so the loops can know when to stop.
//using int *count and char *str aka pointers is to pass the memory address of existing variables from main so the function can change the original data
void enterData(struct Song catalogue[], int *count); //'struct song catalogue[],'passes the array of song structures into the function so it can read or change the songs stored in memory
void readFromFile(struct Song catalogue[], int *count);
void sortCatalogueByPrice(struct Song catalogue[], int count);
void printCatalogue(struct Song catalogue[], int count);
void stripNewline(char *str);

int main() {
    struct Song catalogue[MAX_SONGS];
    int songCount = 0;
    int choice = 0;

    // creates a loop to display the menu until a number is entered
    do {
        printf("\n--- Song Catalogue Menu ---\n");
        printf("[1] Enter data\n");
        printf("[2] Save data to file\n");
        printf("[3] Read data from file\n");
        printf("[4] Print out the catalogue to screen\n");
        printf("[5] Exit Program\n");
        printf("Please enter your choice: ");

        if (scanf("%d", &choice) != 1) { //checks user inputted a valid whole number
            // makes sure input is valid
            while (getchar() != '\n'); //gets rid of leftover keystrokes from an invalid input, since every key is remembered until you press enter
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            continue;
        }
        getchar(); // consumes the leftover newline character from the input buffer

        switch (choice) {
            case 1:
                enterData(catalogue, &songCount);
                break;
            case 2:
                saveToFile(catalogue, songCount);
                break;
            case 3:
                readFromFile(catalogue, &songCount);
                break;
            case 4:
                printCatalogue(catalogue, songCount);
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please select 1-5.\n");
        }
    } while (choice != 5);

    return 0;
}

// Helper function to remove the trailing newline added by fgets()
void stripNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Option [1]: Enter song details from keyboard
void enterData(struct Song catalogue[], int *count) {
    int numToAdd = 0;
    printf("\nHow many songs would you like to enter? (Note: Assessment requires at least 5 in total): ");
    scanf("%d", &numToAdd);
    getchar(); // Clear newline from buffer

    for (int i = 0; i < numToAdd; i++) {
        if (*count >= MAX_SONGS) {
            printf("Catalogue is full!\n");
            break;
        }

        printf("\n--- Entering Song %d ---\n", *count + 1);

        printf("Enter Name of the Band or Singer: ");
        fgets(catalogue[*count].band, MAX_STR, stdin);
        stripNewline(catalogue[*count].band);

        printf("Enter Name of Song: ");
        fgets(catalogue[*count].title, MAX_STR, stdin);
        stripNewline(catalogue[*count].title);

        printf("Enter Price: ");
        scanf("%f", &catalogue[*count].price);

        printf("Enter Year of Release: ");
        scanf("%d", &catalogue[*count].year);
        getchar(); // Clear newline after reading integer so the next fgets() works

        (*count)++;
    }
    printf("\nSuccessfully added %d song(s) to memory.\n", numToAdd);
}

// Option [2]: Save current songs in memory to a text file
void saveToFile(struct Song catalogue[], int count) {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error: Could not open file %s for writing.\n", FILENAME);
        return;
    }

    // Write each song's 4 fields on separate lines for easy reading of spaces
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s\n", catalogue[i].band);
        fprintf(fp, "%s\n", catalogue[i].title);
        fprintf(fp, "%.2f\n", catalogue[i].price);
        fprintf(fp, "%d\n", catalogue[i].year);
    }

    fclose(fp);
    printf("Successfully saved %d song(s) to '%s'.\n", count, FILENAME);
}

// Option [3]: Retrieve song data from the text file into memory
void readFromFile(struct Song catalogue[], int *count) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Error: Could not open file '%s'. Have you saved data first?\n", FILENAME);
        return;
    }

    *count = 0;
    // Read lines in sets of 4 until End Of File (EOF) is reached
    while (*count < MAX_SONGS && fgets(catalogue[*count].band, MAX_STR, fp) != NULL) {
        stripNewline(catalogue[*count].band);

        if (fgets(catalogue[*count].title, MAX_STR, fp) == NULL) break;
        stripNewline(catalogue[*count].title);

        if (fscanf(fp, "%f\n", &catalogue[*count].price) != 1) break;
        if (fscanf(fp, "%d\n", &catalogue[*count].year) != 1) break;

        (*count)++;
    }

    fclose(fp);
    printf("Successfully loaded %d song(s) from '%s'.\n", *count, FILENAME);
}

// Bubble Sort algorithm to order songs by Price (Lower to Higher)
void sortCatalogueByPrice(struct Song catalogue[], int count) {
    struct Song temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (catalogue[j].price > catalogue[j + 1].price) {
                // Swap the two song structures
                temp = catalogue[j];
                catalogue[j] = catalogue[j + 1];
                catalogue[j + 1] = temp;
            }
        }
    }
}

// Option [4]: Sort and print the catalogue to the screen
void printCatalogue(struct Song catalogue[], int count) {
    if (count == 0) {
        printf("\nCatalogue is empty! Enter data [1] or Read from file [3] first.\n");
        return;
    }

    // Sort by price ascending before displaying
    sortCatalogueByPrice(catalogue, count);

    printf("\n================ SONG CATALOGUE (Ordered by Price: Low to High) ================\n");
    for (int i = 0; i < count; i++) {
        printf("Name of Band: %s\n", catalogue[i].band);
        printf("Name of Song: %s\n", catalogue[i].title);
        printf("Price: %.2f\n", catalogue[i].price);
        printf("Year of Release: %d\n\n", catalogue[i].year);
    }
}