#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_LENGTH 2000

char *generate_random_text(size_t length) {
    char *text = malloc(length + 1);
    for (size_t i = 0; i < length; i++) {
        text[i] = 'a' + rand() % 26;
    }
    text[length] = '\0';
    return text;
}

void run_test(const char *input_file, const char *key, const char *output_file) {
    // Generate random input text
    srand(time(NULL));
    size_t length = rand() % MAX_LENGTH/2 + 1;
    if (strcmp(input_file, "long_input_file.txt") == 0) {
        length = MAX_LENGTH + rand() % MAX_LENGTH + 1;
    }
    char *input_text = generate_random_text(length);

    // Create input file
    FILE *file = fopen(input_file, "w");
    if (file == NULL) {
        printf("Failed to create %s\n", input_file);
        return;
    }
    fwrite(input_text, 1, length, file);
    fclose(file);
    printf("Created %s with length %ld\n", input_file, length);

    // Encrypt the input file
    printf("Encrypting %s...\n", input_file);
    char encrypt_command[100];
    sprintf(encrypt_command, "cat %s | ./coder %s -e > %s", input_file, key, output_file);
    system(encrypt_command);
    printf("Encryption complete, output written to %s\n", output_file);

    // Decrypt the encrypted file
    printf("Decrypting %s...\n", output_file);
    char decrypt_command[100];
    sprintf(decrypt_command, "cat %s | ./coder %s -d > decrypted_%s", output_file, key, input_file);
    system(decrypt_command);
    printf("Decryption complete, output written to decrypted_%s\n", input_file);

    // Compare the original and decrypted files
    printf("Comparing %s and decrypted_%s...\n", input_file, input_file);
    char diff_command[100];
    sprintf(diff_command, "diff %s decrypted_%s", input_file, input_file);
    int diff_result = system(diff_command);
    if (diff_result == 0) {
        printf("\033[0;32mTest passed: The contents of %s and decrypted_%s are the same.\033[0m\n", input_file, input_file);
    } else {
        printf("\033[0;31mTest failed: The contents of %s and decrypted_%s are not the same.\033[0m\n", input_file, input_file);
    }

    free(input_text);
}

int main() {
    // Run tests
    run_test("input_file1.txt", "3", "encrypted_file1.txt");
    run_test("input_file2.txt", "5", "encrypted_file2.txt");
    run_test("input_file3.txt", "7", "encrypted_file3.txt");
    run_test("long_input_file.txt", "9", "encrypted_file4.txt");

    return 0;
}
