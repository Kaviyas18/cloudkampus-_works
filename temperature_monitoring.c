
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_SENSORS 100
#define THRESHOLD 95.0

// Function prototypes
void read_csv(const char *filename, float temperatures[], int *count);
void calculate_metrics(const float temperatures[], int count, float *average, float *max, float *min);
void write_exceeding_values(const char *filename, const float temperatures[], int count);

int main() {
    char input_file[] = "temperature_data.csv";
    char output_file[] = "exceeding_temperatures.csv";

    float temperatures[MAX_SENSORS];
    int count = 0;
    float average, max, min;

    // Read temperature data from the CSV file
    read_csv(input_file, temperatures, &count);

    if (count == 0) {
        printf("No temperature data found in the file.\n");
        return 1;
    }

    // Calculate key metrics
    calculate_metrics(temperatures, count, &average, &max, &min);

    // Print metrics
    printf("Average Temperature: %.2f\n", average);
    printf("Maximum Temperature: %.2f\n", max);
    printf("Minimum Temperature: %.2f\n", min);

    // Write exceeding values to a new CSV file
    write_exceeding_values(output_file, temperatures, count);

    printf("Exceeding temperature values have been written to %s\n", output_file);

    return 0;
}


   void read_csv(const char *filename, float temperatures[], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file); // Skip the header line
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ","); // Read timestamp
        token = strtok(NULL, ",");      // Read temperature value
        if (token) {
            float temp = atof(token);
            if (temp > 0 || temp <= 0) { // Ensure valid numeric input
                temperatures[*count] = temp;
                (*count)++;
            }
        }
    }

    fclose(file);
}

void calculate_metrics(const float temperatures[], int count, float *average, float *max, float *min) {
    float sum = 0.0;
    *max = temperatures[0];
    *min = temperatures[0];

    for (int i = 0; i < count; i++) {
        sum += temperatures[i];
        if (temperatures[i] > *max) {
            *max = temperatures[i];
        }
        if (temperatures[i] < *min) {
            *min = temperatures[i];
        }
    }

    *average = sum / count;
}


   
   void write_exceeding_values(const char *filename, const float temperatures[], int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not create output file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Timestamp,Temperature\n");
    for (int i = 0; i < count; i++) {
        if (temperatures[i] > THRESHOLD) {
            fprintf(file, "%d,%.2f\n", i + 1, temperatures[i]);
        }
    }

    fclose(file);
}
