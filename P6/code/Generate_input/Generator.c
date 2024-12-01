#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the maximum container width and number of rectangles
#define CONTAINER_WIDTH 50
#define NUMBER 5000

// Function to generate random integers within range [min, max]
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    // Set random seed
    srand((unsigned int)time(NULL));

    // Open the output file
    FILE *file = fopen("size_5000_short.txt", "w");
    if (file == NULL) {
        printf("Failed to create the file!\n");
        return 1;
    }

    // Write container_width and number to the file
    fprintf(file, "%d\n", CONTAINER_WIDTH);
    fprintf(file, "%d\n", NUMBER);

    // Generate rectangle data
    for (int i = 0; i < NUMBER; i++) {
        int width = random_int(1, CONTAINER_WIDTH); // Ensure width does not exceed CONTAINER_WIDTH
        int height = random_int(1, 4);            // Height can be any value within range
        fprintf(file, "%d %d\n", width, height);   // Write width and height to the file
    }

    // Close the file
    fclose(file);

    printf("Data has been generated\n");
    return 0;
}


