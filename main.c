/**
 * Copyright (c) 2022
 * @file    main.c
 * @author  alx.rey
 * @brief
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_FILENAME_LENGTH (20)

/* Function Definitions ------------------------------------------------------*/
/**
 * @brief check if two strings are identical
 *
 * @param[in] str1  first string
 * @param[in] str2  second string
 * @return          true if the string are the same, false otherwise
 */
bool same_string(const char *str1, const char *str2);

/**
 * @brief display error "caesar-cipher: error: " header on stdout
 */
void print_error_header();

/**
 * @brief display an error message with the problematic argument
 *
 * @param[in] arg problematic argument
 */
void print_arg_error(const char *arg);

/**
 * @brief import in RAM (dynamic allocation) the text contained in a file
 *
 * @param[in] filename  name of the file to import
 * @return              pointer on the text saved in the RAM
 */
char *import_file(char *filename);

/**
 * @brief export a text into a file
 *
 * @param[in] filename  name of the file in which the text should be written
 * @param[in] text      text to write
 */
void export_file(char *filename, char *text);

/**
 * @brief encrypt a text with a given key following the caesar cypher
 *
 * @param[in] text  pointer on the text to encrypt
 * @param[in] key   cypher key to use
 */
void encrypt_file(char *text, int key);

/**
 * @brief edecrypt a text with a given key following the caesar cypher
 *
 * @param[in] text  pointer on the text to decrypt
 * @param[in] key   cypher key to use
 */
void decrypt_file(char *text, int key);

/**
 * @brief display the version of the software on stdout
 */
void print_version();

/**
 * @brief display the help of the software on stdout
 */
void print_help();

/******************************************************************************/
/*      _____ _           __  __      _                                       */
/*     |_   _| |_  ___   |  \/  | ___(_)_ _                                   */
/*       | | | ' \/ -_)  | |\/| |/ _ | | ' \                                  */
/*       |_| |_||_\___|  |_|  |_|\___|_|_||_|                                 */
/*                                                                            */
/******************************************************************************/
int main(int argc, char const *argv[])
{
    bool encrypt;
    int key;
    char input_filename[MAX_FILENAME_LENGTH];
    char output_filename[MAX_FILENAME_LENGTH];

    switch (argc)
    {
    case 1:
    case 3:
    case 4:
        print_error_header();
        printf("missing arguments");
        exit(EXIT_FAILURE);
        break;

    case 2:
        // display version
        if (same_string(argv[1], "--version") || same_string(argv[1], "-v"))
        {
            print_version();
            exit(EXIT_SUCCESS);
        }
        // display help
        if (same_string(argv[1], "--help") || same_string(argv[1], "-h"))
        {
            print_help();
            exit(EXIT_SUCCESS);
        }
        // unkown parameter
        print_arg_error(argv[1]);
        exit(EXIT_FAILURE);
        break;

    case 5:
        // check validity of all arguments
        if (sscanf(argv[2], "%d", &key) != 1)
        {
            print_error_header();
            printf("invalid key");
            exit(EXIT_FAILURE);
        }
        if (sscanf(argv[3], "%19s", input_filename) != 1)
        {
            print_error_header();
            printf("filename %s invalid", argv[3]);
            exit(EXIT_FAILURE);
        }
        if (sscanf(argv[4], "%19s", output_filename) != 1)
        {
            print_error_header();
            printf("filename %s invalid", argv[4]);
            exit(EXIT_FAILURE);
        }
        if (!(same_string(argv[1], "--encrypt") ||
              same_string(argv[1], "--decrypt") ||
              same_string(argv[1], "-e") ||
              same_string(argv[1], "-d")))
        {
            print_arg_error(argv[1]);
            exit(EXIT_FAILURE);
        }

        // import text
        char *text = import_file(input_filename);

        // encrypt text
        if (same_string(argv[1], "--encrypt") || same_string(argv[1], "-e"))
        {
            encrypt_file(text, key);
            export_file(output_filename, text);
            printf("file successfully encrypted\n");
        }
        // decrypt text
        else if (same_string(argv[1], "--decrypt") || same_string(argv[1], "-d"))
        {
            decrypt_file(text, key);
            export_file(output_filename, text);
            printf("file successfully decrypted\n");
        }

        free(text);
        exit(EXIT_SUCCESS);
        break;

    default:
        print_error_header();
        printf("too much arguments");
        exit(EXIT_FAILURE);
        break;
    }

    print_error_header();
    printf("unexpected error");
    exit(EXIT_FAILURE);
}
/******************************************************************************/

/* Function Declarations -----------------------------------------------------*/
/**
 * @brief check if two strings are identical
 */
bool same_string(const char *str1, const char *str2)
{
    return (strcmp(str1, str2) == 0);
}

/**
 * @brief display error "caesar-cipher: error: " header on stdout
 */
void print_error_header()
{
    printf("caesar-cipher: error: ");
}

/**
 * @brief display an error message with the problematic argument
 */
void print_arg_error(const char *arg)
{
    print_error_header();
    printf("unrecognized arguments %s\n", arg);
}

/**
 * @brief import in RAM (dynamic allocation) the text contained in a file
 */
char *import_file(char *filename)
{
    // open file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        print_error_header();
        printf("the file %s does not exist\n", filename);
        exit(EXIT_FAILURE);
    }

    // count number of character in the file
    int nb_char_in_file = 0;
    for (int c = getc(fp); c != EOF; c = getc(fp))
    {
        nb_char_in_file++;
    }
    rewind(fp);

    // allocate memory to store the file in RAM
    char *text = malloc((nb_char_in_file + 1) * sizeof(char));
    if (text == NULL)
    {
        print_error_header();
        printf("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    // copy file in RAM
    fread(text, sizeof(char), nb_char_in_file, fp);

    // close file
    fclose(fp);
    return text;
}

/**
 * @brief export a text into a file
 */
void export_file(char *filename, char *text)
{
    // open file
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        print_error_header();
        printf("impossible to write in file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // write in file
    fwrite(text, sizeof(char), strlen(text), fp);

    // close file
    fclose(fp);
}

/**
 * @brief encrypt a text with a given key following the caesar cypher
 */
void encrypt_file(char *text, int key)
{
    for (int i = 0; i < strlen(text); i++)
    {
        text[i] = text[i] + key;
    }
}

/**
 * @brief edecrypt a text with a given key following the caesar cypher
 */
void decrypt_file(char *text, int key)
{
    for (int i = 0; i < strlen(text); i++)
    {
        text[i] = text[i] - key;
    }
}

/**
 * @brief display the version of the software on stdout
 */
void print_version()
{
    printf("caesar-cipher version 1.0\n");
}

/**
 * @brief display the help of the software on stdout
 */
void print_help()
{
    printf("Usage: ./main [-e | -d] key input_file output_file\n\
              -d, --decrypt     decrypt input_file with the given key\n\
              -e, --encrypt     encrypt input_file with the given key\n\
              -h, --help        display this help and exit\n\
              -v, --version     display version and exit\n\
            ");
}
/* End of main.c -------------------------------------------------------------*/