#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "cnf.h"

int matches_arg(char *cli_arg, char *arg)
{
    if (strncmp(cli_arg, arg, strlen(arg)) == 0)
        return 1;
    return 0;
}

void print_usage()
{
    printf("usage:\n");
    printf("argo [command] <option>\n\n");
    printf("commands:\n");
    printf("    [new] <project_name> # scaffolds a project\n");
    printf("    [add] <library_name> # adds a library\n");
    printf("    [build] <args> # builds the application\n");
    printf("    [run] <args> # runs the application with supplied args\n");
    printf("    [install] <args> # installs the argo managed binary\n");
    printf("    [uninstall] <args> # uninstalls an argo manaaged binary\n");
    printf("    [ls] # lists argo manaaged binaries\n");
    printf("    [--version] # displays installed argo version\n");
    printf("    [--help] # displays usage menu\n\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage();
        return -1;
    }
    char *cmd = argv[1];
    if (matches_arg(cmd, "new") == 1)
    {
        printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
        char *project_name;
        if (argc < 3)
        {
            printf("\x1b[38;5;240mplease provide a project name:\x1b[0m ");
            char buffer[256];
            fgets(buffer, 256, stdin);
            printf("\n");
            buffer[strcspn(buffer, "\n")] = 0;
            project_name = buffer;
        }
        else
        {
            project_name = argv[2];
        }
        new_project(project_name);
    }
    else if (matches_arg(cmd, "add") == 1)
    {
        char *lib_name;
        if (argc < 3)
        {
            printf("\x1b[38;5;240mplease provide a library name:\x1b[0m ");
            char buffer[256];
            fgets(buffer, 256, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            lib_name = buffer;
        }
        else
        {
            lib_name = argv[2];
        }
        add_lib(lib_name);
    }
    else if (matches_arg(cmd, "build") == 1)
    {
    printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
        build_bin();
    }
    else if (matches_arg(cmd, "run") == 1)
    {
    printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
        run_bin(argc, argv);
    }
    else if (matches_arg(cmd, "install") == 1)
    {
        install_bin();
    }
    else if (matches_arg(cmd, "uninstall") == 1)
    {
        char *bin_name;
        if (argc < 3)
        {
            printf("\x1b[38;5;240mplease provide a bin name to remove:\x1b[0m ");
            char buffer[256];
            fgets(buffer, 256, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            bin_name = buffer;
        }
        else
        {
            bin_name = argv[2];
        }
        uninstall_bin(bin_name);
    }
    else if (matches_arg(cmd, "ls") == 1)
    {
        list_binaries();
    }
    else if (matches_arg(cmd, "--version") == 1)
    {
        printf("\x1b[38;5;39margo v0.1.0\x1b[0m\n");
    }
    else if (matches_arg(cmd, "--help") == 1)
    {

        printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
        print_usage();
    }
    else
    {
        printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
        printf("unrecognized command: \"%s\"\n", cmd);
        printf("see usage\n\n");
        print_usage();
    }

    return 0;
}