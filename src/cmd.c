#include "cmd.h"
#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int new_project(char *project_name)
{
    printf("\x1b[38;5;240mcreating project %s\x1b[0m\n\n", project_name);
    scaffold(project_name);
    return 0;
}

int add_lib(char *lib_name)
{
    printf("adding lib %s...\n", lib_name);
    char buffer[256];
    snprintf(buffer, strlen(lib_name) * sizeof(char) + 12, "brew install %s", lib_name);
    system(buffer);
    return 0;
}
int run_bin(int argc, char *argv[])
{
    printf("\x1b[38;5;39mrunning application...\x1b[0m\n");
    char buffer[1024] = "make clean && make;binary=$(ls bin | head -n 1);./bin/$binary";
    for (int i = 2; i < argc; i++)
    {
        strcat(buffer, " ");
        strcat(buffer, argv[i]);
    }
    system(buffer);
    return 0;
}
int build_bin()
{
    printf("building binary...\n");
    system("make clean && make");
    return 0;
}
int install_bin()
{
    printf("\x1b[38;5;39mbuilding project...\x1b[0m\n");
    system("make clean && make");

    printf("\x1b[38;5;39minstalling binary...\x1b[0m\n");
    char *home = getenv("HOME");
    char dir[512];
    sprintf(dir, "%s/.argo", home);

    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        mkdir(dir, 0700);
    }
    FILE *pipe = popen("ls bin | head -n 1", "r");
    char binary[256];
    fgets(binary, sizeof(binary), pipe);
    binary[strcspn(binary, "\n")] = 0;
    pclose(pipe);

    char src[1024], dest[1024];
    sprintf(src, "bin/%s", binary);
    sprintf(dest, "%s/%s", dir, binary);
    if (copy_file(src, dest) != 0)
    {
        printf("unable to copy %s to %s\n", binary, dir);
        return -1;
    }
    if (chmod(dest, S_IRWXU) != 0)
        return -1;
    printf("\x1b[38;5;39madded %s to $PATH\x1b[0m\n", binary);
    return 0;
}

int uninstall_bin(char *bin_name)
{
    printf("uninstalling %s...\n", bin_name);
    const char *home = getenv("HOME");
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s/.argo/%s", home, bin_name);
    if (remove(file_path) != 0)
    {
        printf("error removing %s\n", bin_name);
        return -1;
    }
    printf("\x1b[38;5;39muninstalled %s\x1b[0m\n", bin_name);
    return 0;
}

int list_binaries()
{
    char *home = getenv("HOME");
    char dir[512];
    sprintf(dir, "%s/.argo", home);
    printf("\x1b[38;5;39minstalled argo binaries:\x1b[0m\n\n");
    list_entries(dir);
    return 0;
}