#include <stdio.h>
#include <string.h>

#ifndef ARGPARS_H
#define ARGPARS_H

typedef struct {
    const char small_option;
    const char *long_option;
    const char *description;
    const char *value_to_save; // bool, int, string, help
    char *value;
} argpars;

char *get_arg_value(argpars *args, int args_size, char *name);

void err_exit(char *msg, ...);
char *isint_str(char *string);

#define ARRAY_SIZE(parser) (sizeof(parser)/sizeof(*parser))
#define CMP_2_STR(_string_1, _string_2) (strcmp(_string_1, _string_2) == 0)

#define true 1
#define false 0

// void print_help(char *program_name, argpars parser[], int parser_size);

argpars *argpars_setup_env(int argc, char **argv, argpars parser[], int parser_size);

#endif // ARGPARS_H