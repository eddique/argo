#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"

int matches_arg(char *cli_arg, char *arg)
{
    if (strncmp(cli_arg, arg, strlen(arg)) == 0)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("\n\x1b[38;5;39mno arguments given...\x1b[0m\n\n");
        printf("\x1b[38;5;39msee usage...\x1b[0m\n\n");
        print_help(0);
        return -1;
    }
    char *cmd = argv[1];
    if (matches_arg(cmd, "new") == 1)
        new_project(argc, argv);
    else if (matches_arg(cmd, "add") == 1)
        add_lib(argc, argv);
    else if (matches_arg(cmd, "build") == 1)
        build_bin();
    else if (matches_arg(cmd, "run") == 1)
        run_bin(argc, argv);
    else if (matches_arg(cmd, "install") == 1)
        install_bin();
    else if (matches_arg(cmd, "uninstall") == 1)
        uninstall_bin(argc, argv);
    else if (matches_arg(cmd, "ls") == 1)
        list_binaries();
    else if (matches_arg(cmd, "--version") == 1)
        printf("\x1b[38;5;39margo v0.1.0\x1b[0m\n");
    else if (matches_arg(cmd, "--help") == 1)
        print_help(1);
    else
    {
        printf("\n\x1b[38;5;39munrecognized command: \"%s\"\x1b[0m\n\n", cmd);
        printf("\x1b[38;5;39msee usage...\x1b[0m\n\n");
        print_help(0);
    }

    return 0;
}