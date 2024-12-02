#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the maximum container width and number of rectangles
#define CONTAINER_WIDTH 10
#define NUMBER 7000

// Function to generate random integers within range [min, max]
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    // Set random seed
    srand((unsigned int)time(NULL));

    // Open the output files for short and high
    FILE *fileShort = fopen("size_7000_short.txt", "w");
    FILE *fileHigh = fopen("size_7000_high.txt", "w");

    if (fileShort == NULL || fileHigh == NULL) {
        printf("Failed to create one or both of the files!\n");
        return 1;
    }

    // Write container_width and number to both files
    fprintf(fileShort, "%d\n", CONTAINER_WIDTH);
    fprintf(fileShort, "%d\n", NUMBER);

    fprintf(fileHigh, "%d\n", CONTAINER_WIDTH);
    fprintf(fileHigh, "%d\n", NUMBER);

    // Generate rectangle data for both files
    for (int i = 0; i < NUMBER; i++) {
        int width = random_int(1, CONTAINER_WIDTH);  // Ensure width does not exceed CONTAINER_WIDTH
        int shortHeight = random_int(1, 5);          // Short height is within range [1, 5]
        
        int highHeight = random_int(1, 50);
        
        // Write data to the short file
        fprintf(fileShort, "%d %d\n", width, shortHeight);

        // Write data to the high file (height is independently random within [shortHeight*10, shortHeight*10 + 9])
        fprintf(fileHigh, "%d %d\n", width, highHeight);
    }

    // Close the files
    fclose(fileShort);
    fclose(fileHigh);

    printf("Data has been generated for both short and high files\n");
    return 0;
}
