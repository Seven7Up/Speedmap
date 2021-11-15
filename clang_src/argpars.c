#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "argpars.h"
#include "log.h"


void print_help(char *program_name, argpars parser[], int parser_size) {
    printf("Usage: %s", program_name);
    for (int i = 0; i < parser_size; i++) {
        if (parser[i].small_option)
            printf(" [-%c|--%s]", parser[i].small_option, parser[i].long_option);
        else printf(" [--%s]", parser[i].long_option);
    }
    
    printf("\n\nArguments: \n");
    for (int i = 0; i < parser_size; i++) {
        if (parser[i].small_option)
            printf("  -%c    --%-26s\t%s\n",
                parser[i].small_option, parser[i].long_option, parser[i].description);
        else printf("        --%-26s\t%s\n", parser[i].long_option, parser[i].description);
    }
    exit(0);
}

char *isint_str(char *string) {
    char ints[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int _need_break = false;

    int i;
    char *new_string = string;
    while (*string) {
        i = 0;
        while (i < ARRAY_SIZE(ints)) {
            if (*string == ints[i]) {
                break;
            }
            i++;
            if (i == ARRAY_SIZE(ints)) _need_break = true;
        }
        if (_need_break) {
            err_exit("Input error: '%s', please enter an int!", string);
        }
        string++;
    }
    return new_string;

}

int value_to_save(int argv_i, char *argv[], argpars *parser, int parser_size, int parser_i) {
    if CMP_2_STR(parser[parser_i].value_to_save, "bool") {
        parser[parser_i].value = "1";
        return 0;
    }
    else if CMP_2_STR(parser[parser_i].value_to_save, "int") {
        parser[parser_i].value = isint_str(argv[argv_i+1]);
        return 1;
    }
    else if CMP_2_STR(parser[parser_i].value_to_save, "string") {
        parser[parser_i].value = argv[argv_i+1];
        return 1;
    }
    else if CMP_2_STR(parser[parser_i].value_to_save, "help") print_help(argv[0], parser, parser_size);
    else err_exit("Unknown type value: '%s', check your code or contact admin!", parser[parser_i].value_to_save);
}

argpars *argpars_setup_env(int argc, char *argv[], argpars parser[], int parser_size) {
    if (argc <= 1) {
        print_help(argv[0], parser, parser_size);
    }
    int _need_break = false;
    int argv_i = 1;

    while (argv_i < argc) {
        while (true) {
            for (int parser_i = 0; parser_i < parser_size; parser_i++) {
                if ((strlen(argv[argv_i]) == 2) && (argv[argv_i][1] == parser[parser_i].small_option)) {
                    argv_i += value_to_save(argv_i, argv, parser, parser_size, parser_i);
                    _need_break = true;
                    break;
                }
                else if ((argv[argv_i][1] == '-') && CMP_2_STR(&argv[argv_i][2], parser[parser_i].long_option)) {
                    argv_i += value_to_save(argv_i, argv, parser, parser_size, parser_i);
                    _need_break = true;
                    break;
                }
                /* TODO: add an option to pass 2 small_args or more in 1 arg like 'speedmap -dt 3 -H 192.168.1.1'
                * wich equal to 'speedmap --debug --timeout 3 --host 192.168.1.1'
                */
            }
            if (_need_break) { _need_break = false; argv_i++; break; }
            err_exit("Unknown option: '%s', type '%s --help' to print help!", argv[argv_i], argv[0]);
        }
    }
    return parser;
}


char *get_arg_value(argpars *args, int args_size, char *name) {
    char *value = NULL;
    for (int args_i = 0; args_i < args_size; args_i++) {
        if CMP_2_STR(args[args_i].long_option, name) value = args[args_i].value;
    }
    return value;
}