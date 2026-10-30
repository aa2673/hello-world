#include <stdio.h>
// Function that replaces spaces with dashes using pointer arithmetic
int replace(char *str) {
    int count = 0;
    
    while (*str != '\0') {
        if (*str == ' ') {
            *str = '-';
            count++;
        }
        str++; // Moves pointer to the next character
    }
    
    return count;
}


int main(void) {
    //Declared as an array  instead of a literal pointer to make sure that the memory is able to be changed
    char str[100];
    printf("Enter a string: ");
    fgets(str, 100, stdin);
    
    printf("String was: %s", str);
    int num_replaced = replace(str);
    printf("String is: %s", str);
    printf("Number replaced %d", num_replaced);
    return 0;
}