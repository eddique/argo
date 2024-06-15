#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cnf.h"
#define PATH_BUFFER 256

static void git_init(char *buffer, const char *project_name)
{
    buffer = realloc(buffer, sizeof(char) * strlen(project_name) + 33);
    snprintf(buffer, sizeof(char) * (strlen(project_name) + 33), "cd %s && git init > /dev/null 2>&1", project_name);
    system(buffer);
}

static void set_path(char *dest, const char *name, const char *dir, const char *file_name)
{
    if (dir != NULL && file_name != NULL)
        snprintf(dest, PATH_BUFFER * sizeof(char), "%s/%s/%s", name, dir, file_name);
    else if (dir != NULL && file_name == NULL)
        snprintf(dest, PATH_BUFFER * sizeof(char), "%s/%s", name, dir);
    else if (dir == NULL && file_name == NULL)
        snprintf(dest, PATH_BUFFER * sizeof(char), "%s", name);
    else
        snprintf(dest, PATH_BUFFER * sizeof(char), "%s/%s", name, file_name);
}

static int make_directory(const char *path)
{
    struct stat st = {0};

    if (stat(path, &st) == -1)
    {
        if (mkdir(path, 0700) == -1)
        {
            return -1;
        }
    }
    return 0;
}
static int make_directories(const char *path)
{
    char *copy = strdup(path);
    char *ptr = copy;
    while ((ptr = strchr(ptr, '/')) != NULL)
    {
        *ptr = '\0';
        if (make_directory(copy) == -1)
        {
            free(copy);
            return -1;
        }
        *ptr = '/';
        ++ptr;
    }
    free(copy);
    return make_directory(path);
}
static void mkdirs(char *project_name)
{
    char buffer[PATH_BUFFER];

    set_path(buffer, project_name, NULL, NULL);
    make_directories(buffer);

    set_path(buffer, project_name, "src", NULL);
    make_directories(buffer);

    set_path(buffer, project_name, "include", NULL);
    make_directories(buffer);

    set_path(buffer, project_name, ".vscode", NULL);
    make_directories(buffer);
}

static int write_file(const char *name, const char *dir, const char *file_name, const char *content)
{
    char path[PATH_BUFFER];
    set_path(path, name, dir, file_name);
    FILE *file;
    file = fopen(path, "w");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }
    fprintf(file, "%s", content);
    fclose(file);
    return 0;
}

int copy_file(const char *src, const char *dest)
{
    FILE *srcFile = fopen(src, "rb");
    if (srcFile == NULL)
        return -1;

    FILE *destFile = fopen(dest, "wb");
    if (destFile == NULL)
    {
        fclose(srcFile);
        return -1;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), srcFile)) > 0)
    {
        if (fwrite(buffer, 1, bytes, destFile) != bytes)
        {
            fclose(srcFile);
            fclose(destFile);
            return -1;
        }
    }

    fclose(srcFile);
    fclose(destFile);
    return 0;
}

int list_entries(const char *directory)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
                printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return 0;
}

int scaffold(char *project_name)
{
    mkdirs(project_name);
    char *buffer = NULL;

    printf("\x1b[38;5;39mcreating %s/src/main.c...\x1b[0m\n", project_name);
    write_file(project_name, "src", "main.c", MAIN_C);

    printf("\x1b[38;5;39mcreating %s/include/common.h...\x1b[0m\n", project_name);
    build_common_h(&buffer, project_name);
    write_file(project_name, "include", "common.h", buffer);

    printf("\x1b[38;5;39mcreating %s/makefile...\x1b[0m\n", project_name);
    build_makefile(&buffer, project_name);
    write_file(project_name, NULL, "makefile", buffer);

    printf("\x1b[38;5;39mcreating %s/Dockerfile...\x1b[0m\n", project_name);
    build_dockerfile(&buffer, project_name);
    write_file(project_name, NULL, "Dockerfile", buffer);

    printf("\x1b[38;5;39mcreating %s/.vscode/settings.json...\x1b[0m\n", project_name);
    write_file(project_name, ".vscode", "settings.json", SETTINGS);

    printf("\x1b[38;5;39mcreating %s/.vscode/tasks.json...\x1b[0m\n", project_name);
    write_file(project_name, ".vscode", "tasks.json", TASKS);

    printf("\x1b[38;5;39mcreating %s/.vscode/launch.json...\x1b[0m\n", project_name);
    build_launch(&buffer, project_name);
    write_file(project_name, ".vscode", "launch.json", buffer);

    printf("\x1b[38;5;39mcreating %s/.gitignore...\x1b[0m\n", project_name);
    write_file(project_name, NULL, ".gitignore", GITIGNORE);

    printf("\x1b[38;5;39mcreating %s/README.md...\x1b[0m\n", project_name);
    build_readme(&buffer, project_name);
    write_file(project_name, NULL, "README.md", buffer);

    printf("\x1b[38;5;39minitializing git repo\x1b[0m\n");
    git_init(buffer, project_name);

    free(buffer);

    printf("\n\x1b[38;5;240mnext steps:\x1b[0m\n\n");
    printf("    cd %s\n", project_name);
    printf("    argo run\n\n");

    return 0;
}