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

    // creates a loop to display the menu
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
        getchar(); // gets rid of the leftover enter key

        switch (choice) { //instead of writing seperate if statements switch looks at the number inside choice and jumps to the matching case
            case 1:
                enterData(catalogue, &songCount);//runs the function enterData and breaks the loop
                break;
            case 2:
                saveToFile(catalogue, songCount);//runs the function saveToFile and breaks the loop
                break;
            case 3:
                readFromFile(catalogue, &songCount);//runs the function readFromFile and breaks the loop
                break;
            case 4:
                printCatalogue(catalogue, songCount);//runs the function printCatalogue and breaks the loop
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");//exits menu
                break;
            default:
                printf("Invalid choice! Please select 1-5.\n");//
        }
    } while (choice != 5);//this will keep the menu on a loop until option 5 is selected

    return 0;//closes program
}

// function to remove the leftover enter key added by fgets()
void stripNewline(char *str) {//accepts a piece of text
    size_t len = strlen(str);//counts how many characters are in the text
    if (len > 0 && str[len - 1] == '\n') {//checks that the text isnt empty and the last character is an enter
        str[len - 1] = '\0';//erases the enter key and replaces it with '\0' the end of text marker or null terminator
    }
}

// function to enter song details
void enterData(struct Song catalogue[], int *count) {//brings song catalogue structure and pointer is used so the total of songs can be updated as you add new tracks
    int numToAdd = 0;//sets number of songs to be added as 0
    printf("\nHow many songs would you like to enter?: ");
    scanf("%d", &numToAdd);//gets the number of songs you want to add
    getchar(); // clear leftover enter key

    for (int i = 0; i < numToAdd; i++) {//creates a loop that lets you add how many songs you want to add
        if (*count >= MAX_SONGS) {//if you have already reached max songs it will not allow you to add more
            printf("Catalogue is full!\n");
            break;
        }

        printf("\n--- Entering Song %d ---\n", *count + 1);//updates the song count

        printf("Enter Name of the Band or Singer: ");
        fgets(catalogue[*count].band, MAX_STR, stdin);//use fgets because it will read a full line of text from the keyboard including spaces unlike scanf
        stripNewline(catalogue[*count].band);//gets rid of the enter key from fgets

        printf("Enter Name of Song: ");
        fgets(catalogue[*count].title, MAX_STR, stdin);//gets song name
        stripNewline(catalogue[*count].title);

        printf("Enter Price: ");
        scanf("%f", &catalogue[*count].price);//gets price, uses float so you can input a decimal

        printf("Enter Year of Release: ");//gets year of realase
        scanf("%d", &catalogue[*count].year);
        getchar(); // gets rid of leftover enter after reading integer so the next fgets() works

        (*count)++;//adds 1 to the counter in main
    }
    printf("\nSuccessfully added %d song(s) to memory.\n", numToAdd);
}

// function to save songs to file
void saveToFile(struct Song catalogue[], int count) {
    FILE *fp = fopen(FILENAME, "w");//creates a text file in write mode *fp is for file pointer "w" erases old content to write a fresh list
    if (fp == NULL) {//safety check for if the computer cant open the file
        printf("Error: Could not open file %s for writing.\n", FILENAME);
        return;
    }

    // writes each detail of the song on separate lines
    for (int i = 0; i < count; i++) {//creates a loop to display all songs so far
        fprintf(fp, "%s\n", catalogue[i].band);
        fprintf(fp, "%s\n", catalogue[i].title);
        fprintf(fp, "%.2f\n", catalogue[i].price);
        fprintf(fp, "%d\n", catalogue[i].year);
    }

    fclose(fp);//saves changes and closes files
    printf("Successfully saved %d song(s) to '%s'.\n", count, FILENAME);
}

// function to retrieve song data from file into memory
void readFromFile(struct Song catalogue[], int *count) {
    FILE *fp = fopen(FILENAME, "r");//opens the saved text file in reading mode 
    if (fp == NULL) {//checks if the file exists to be opened
        printf("Error: Could not open file '%s'. Have you saved data first?\n", FILENAME);
        return;
    }

    *count = 0;//resets active song list to 0, it clears out current memory, to load a fresh copy.
    // reads lines 4 at a time until the end of the file
    while (*count < MAX_SONGS && fgets(catalogue[*count].band, MAX_STR, fp) != NULL) {//creates a while loop, checks there is room in the catalogue and whether or not the band name is in the limit, when there are no more lines left loop stops.
        stripNewline(catalogue[*count].band);//takes away the leftover enter key at the end of the band name

        if (fgets(catalogue[*count].title, MAX_STR, fp) == NULL) break;//checks song name is within limit
        stripNewline(catalogue[*count].title);//strips away leftover enter key at the end of the title

        if (fscanf(fp, "%f\n", &catalogue[*count].price) != 1) break;//checks if price is a float
        if (fscanf(fp, "%d\n", &catalogue[*count].year) != 1) break;//checks if year is a number

        (*count)++;//updates the counter by 1
    }

    fclose(fp);//closes the file
    printf("Successfully loaded %d song(s) from '%s'.\n", *count, FILENAME);
}

// function to sort prices from lowest to highest
void sortCatalogueByPrice(struct Song catalogue[], int count) {
    struct Song temp;//creates a temporary empty song structure as a holding spot for when you are swapping songs
    for (int i = 0; i < count - 1; i++) {//creates a loop for the amount of passes you make through the list to make sure everything is sorted
        for (int j = 0; j < count - i - 1; j++) {//another loop that compares songs next to each other during the current pass.
            if (catalogue[j].price > catalogue[j + 1].price) {
                temp = catalogue[j];//copies the song structure to the temporary empty song structure
                catalogue[j] = catalogue[j + 1];//moves the cheaper song into the earlier slot
                catalogue[j + 1] = temp;//puts the more expensive song into the later slot
            }
        }
    }
}

//function to sort the catalogue and print it
void printCatalogue(struct Song catalogue[], int count) {
    if (count == 0) {//checks if any songs are in memory
        printf("\nCatalogue is empty! Please enter data or read from file first.\n");
        return;
    }

    // sorts songs by price low to high
    sortCatalogueByPrice(catalogue, count);

    printf("\n================ SONG CATALOGUE (Ordered by Price: Low to High) ================\n");
    for (int i = 0; i < count; i++) {//loops from 0 to the current count
        printf("Name of Band: %s\n", catalogue[i].band);
        printf("Name of Song: %s\n", catalogue[i].title);
        printf("Price: %.2f\n", catalogue[i].price);
        printf("Year of Release: %d\n\n", catalogue[i].year);
    }
}