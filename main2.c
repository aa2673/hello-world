#include <stdio.h>

// A function to encode lowercase characters using pointer manipulation
void code(char *str) {
    char code1[] = "abcdefghijklmnopqrstuvwxyz";
    char code2[] = "bpduhijkaltxwmrzfoysvngeqc";

    while (*str != '\0') {
        // Only looks at lowercase characters
        if (*str >= 'a' && *str <= 'z') {
            for (int i = 0; code1[i] != '\0'; i++) {
                if (*str == code1[i]) {
                    *str = code2[i]; // Replaces a character with substitution mapping
                    break; // Loop ends once matched
                }
            }
        }
        str++; // Moves pointer to the next character
    }
}
int main(void) {
    char str[100];
    printf("Enter a string: ");
    fgets(str, 100, stdin);
    
    printf("String was: %s", str);
    code(str);
    printf("String is: %s", str);
    
    return 0;
}