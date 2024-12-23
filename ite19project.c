#include <stdio.h>  // Include standard I/O library
#include <string.h> // Include string handling library
#include <ctype.h>  // Include character handling library
#include <stdlib.h> // Include standard library for general utilities

// Function prototypes
int romanToDecimal(char *roman);                          // Converts Roman numerals to decimal
char *decimalToWords(int num);                            // Converts decimal number to word representation
int performOperation(int num1, int num2, char operation); // Performs arithmetic operations
void processLine(char *line, FILE *output);               // Processes a single line of input
int isValidRoman(char *roman);                            // Checks if a string is a valid Roman numeral

// Function to convert Roman numerals to decimal
int romanToDecimal(char *roman)
{
    int total = 0;         // Initialize total to store decimal value
    int i;                 // Loop variable
    int values[256] = {0}; // Array to store Roman numeral values (ASCII indexed)

    // Assign values to Roman numeral characters
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    // Loop through the Roman numeral string
    for (i = 0; roman[i] != '\0'; i++)
    {
        // If current value is less than the next value, subtract it
        if (values[roman[i]] < values[roman[i + 1]])
        {
            total -= values[roman[i]];
        }
        else // Otherwise, add the current value
        {
            total += values[roman[i]];
        }
    }
    return total; // Return the computed decimal value
}

// Function to convert decimal number to word representation
char *decimalToWords(int num)
{
    static char words[1024];                                                                                                        // Static buffer to store the result
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};                                    // Words for ones
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"}; // Words for teens
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};                          // Words for tens
    char *thousands[] = {"", "Thousand", "Million", "Billion"};                                                                     // Words for large number groups

    int part[4] = {0}; // Array to store groups of three digits
    int i = 0;         // Counter for groups
    char temp[256];    // Temporary buffer

    if (num == 0) // Handle zero case
    {
        strcpy(words, "Zero");
        return words;
    }

    if (num < 0) // Handle negative numbers
    {
        strcpy(words, "Negative ");
        num = -num; // Convert to positive for processing
    }
    else
    {
        words[0] = '\0'; // Initialize result buffer
    }

    // Split the number into groups of three digits
    while (num > 0)
    {
        part[i++] = num % 1000; // Store the last three digits
        num /= 1000;            // Remove the last three digits
    }

    // Convert each group into words
    for (int j = i - 1; j >= 0; j--)
    {
        int n = part[j]; // Get the current group

        if (n == 0) // Skip if the group is zero
            continue;

        if (n / 100 > 0) // Handle hundreds
        {
            sprintf(temp, "%s Hundred ", ones[n / 100]);
            strcat(words, temp);
        }

        n %= 100; // Remove the hundreds part

        if (n >= 10 && n <= 19) // Handle teens
        {
            strcat(words, teens[n - 10]);
        }
        else // Handle tens and ones
        {
            if (n / 10 > 0)
            {
                sprintf(temp, "%s ", tens[n / 10]);
                strcat(words, temp);
            }
            if (n % 10 > 0)
            {
                strcat(words, ones[n % 10]);
            }
        }

        if (j > 0) // Append the group label (e.g., Thousand, Million)
        {
            sprintf(temp, " %s ", thousands[j]);
            strcat(words, temp);
        }
    }

    return words; // Return the word representation
}

// Function to perform arithmetic operations
int performOperation(int num1, int num2, char operation)
{
    switch (operation) // Determine the operation based on the operator
    {
    case '+': // Addition
        return num1 + num2;
    case '-': // Subtraction
        return num1 - num2;
    case '*': // Multiplication
        return num1 * num2;
    case '/':          // Division
        if (num2 == 0) // Check for division by zero
        {
            printf("Error: Division by zero.\n");
            exit(1); // Exit if division by zero
        }
        return num1 / num2;
    default: // Invalid operator
        return 0;
    }
}

// Function to validate if a string is a valid Roman numeral
int isValidRoman(char *roman)
{
    for (int i = 0; roman[i] != '\0'; i++) // Loop through the string
    {
        if (!strchr("IVXLCDM", roman[i])) // Check if character is a valid Roman numeral
        {
            return 0; // Return 0 if invalid character is found
        }
    }
    return 1; // Return 1 if all characters are valid
}

// Function to process a single line of input
void processLine(char *line, FILE *output)
{
    char roman1[50], roman2[50], operation; // Variables to store input values
    int num1, num2, result;                 // Variables to store numbers and result
    char resultWords[1024];                 // Buffer to store result in words

    // Parse the line into two Roman numerals and an operator
    if (sscanf(line, "%s %c %s", roman1, &operation, roman2) != 3)
    {
        fprintf(output, "Invalid line: %s", line); // Handle invalid input format
        return;
    }

    // Validate the Roman numerals
    if (!isValidRoman(roman1) || !isValidRoman(roman2))
    {
        fprintf(output, "Invalid Roman numeral in line: %s", line);
        return;
    }

    // Convert Roman numerals to decimal
    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    // Perform the operation
    result = performOperation(num1, num2, operation);

    // Convert the result to words
    strcpy(resultWords, decimalToWords(result));

    // Write the result to the output file
    fprintf(output, "%s\n", resultWords);
}

// Main function
int main()
{
    FILE *input = fopen("Input.txt", "r");   // Open the input file for reading
    FILE *output = fopen("Output.txt", "w"); // Open the output file for writing
    char line[100];                          // Buffer to store each line of input

    if (!input || !output) // Check if files were opened successfully
    {
        printf("Error opening file.\n");
        return 1;
    }

    // Read each line from the input file and process it
    while (fgets(line, sizeof(line), input))
    {
        processLine(line, output); // Process the current line
    }

    fclose(input);  // Close the input file
    fclose(output); // Close the output file
    printf("Processing completed. Check Output.txt.\n");
    return 0; // Exit the program
}
