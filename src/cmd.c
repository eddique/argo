#include "cmd.h"
#include "fs.h"
#include "cnf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <zlib.h>
#define CHUNK 16384
static void print_usage()
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
void print_help(int show_logo)
{
    if (show_logo == 1)
        printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
    print_usage();
}

int new_project(int argc, char *argv[])
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
        project_name = argv[2];
    printf("\n\x1b[38;5;240mcreating project %s\x1b[0m\n\n", project_name);
    scaffold(project_name);
    return 0;
}

int add_lib(int argc, char *argv[])
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
    printf("adding lib %s...\n", lib_name);
    char buffer[256];
    snprintf(buffer, strlen(lib_name) * sizeof(char) + 12, "brew install %s", lib_name);
    system(buffer);
    return 0;
}
int run_bin(int argc, char *argv[])
{
    printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
    printf("\x1b[38;5;39mbuilding binary...\x1b[0m\n\n");
    system("make clean && make");

    printf("\n\x1b[38;5;39mrunning running binary...\x1b[0m\n\n");
    char buffer[1024] = "binary=$(ls bin | head -n 1);./bin/$binary";
    for (int i = 2; i < argc; i++)
    {
        strcat(buffer, " ");
        strcat(buffer, argv[i]);
    }
    system(buffer);

    printf("\n");
    return 0;
}
int build_bin()
{
    printf("\x1b[38;5;39m%s\x1b[0m", LOGO);
    printf("\x1b[38;5;39mbuilding binary...\x1b[0m\n\n");
    system("make clean && make");
    printf("\n\x1b[38;5;39mbuild finished\x1b[0m\n\n");
    return 0;
}
int install_bin()
{
    printf("\n\x1b[38;5;39mbuilding project...\x1b[0m\n\n");
    system("make clean && make");

    printf("\n\x1b[38;5;39minstalling binary...\x1b[0m\n\n");
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
    printf("\x1b[38;5;39madded %s to $PATH\x1b[0m\n\n", binary);
    return 0;
}

int uninstall_bin(int argc, char *argv[])
{
    char *bin_name;
    if (argc < 3)
    {
        printf("\n\x1b[38;5;240mplease provide a bin name to remove:\x1b[0m ");
        char buffer[256];
        fgets(buffer, 256, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        bin_name = buffer;
    }
    else
    {
        bin_name = argv[2];
    }
    printf("\n\x1b[38;5;39muninstalling %s...\x1b[0m\n\n", bin_name);
    const char *home = getenv("HOME");
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s/.argo/%s", home, bin_name);
    if (remove(file_path) != 0)
    {
        printf("error removing %s\n", bin_name);
        return -1;
    }
    printf("\x1b[38;5;39muninstalled %s\x1b[0m\n\n", bin_name);
    return 0;
}

int list_binaries()
{
    char *home = getenv("HOME");
    char dir[512];
    sprintf(dir, "%s/.argo", home);
    printf("\n\x1b[38;5;39minstalled argo binaries:\x1b[0m\n\n");
    list_entries(dir);
    printf("\n");
    return 0;
}

int compress_file(int argc, char *argv[])
{
    char *in;
    char out[] = "index.h";
    if (argc < 3)
    {
        printf("\x1b[38;5;240mplease provide a file name:\x1b[0m ");
        char buffer[256];
        fgets(buffer, 256, stdin);
        printf("\n");
        buffer[strcspn(buffer, "\n")] = 0;
        in = buffer;
    }
    else
        in = argv[2];
    FILE *source = fopen(in, "rb");
    if (!source)
    {
        return -1;
    }

    char temp_filename[] = "/tmp/compressedXXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1)
    {
        fclose(source);
        return -1;
    }

    gzFile gz = gzdopen(temp_fd, "wb");
    if (!gz)
    {
        fclose(source);
        close(temp_fd);
        return -1;
    }

    char inbuf[CHUNK];
    int len;
    while ((len = fread(inbuf, 1, CHUNK, source)) > 0)
    {
        gzwrite(gz, inbuf, len);
    }

    gzclose(gz);
    fclose(source);

    FILE *temp = fopen(temp_filename, "rb");
    if (!temp)
    {
        close(temp_fd);
        return -1;
    }

    fseek(temp, 0, SEEK_END);
    long size = ftell(temp);
    fseek(temp, 0, SEEK_SET);

    FILE *dest = fopen(out, "w");
    if (!dest)
    {
        fclose(temp);
        return -1;
    }
    char *filename = strdup(in);
    for (int i = 0; filename[i]; i++)
    {
        if (filename[i] == '.')
        {
            filename[i] = '_';
        }
    }
    fprintf(dest, "//File: %s.gz, Size: %ld\n", in, size);
    fprintf(dest, "#define %s_gz_len %ld\n", filename, size);
    fprintf(dest, "const uint8_t %s_gz[] = {\n  ", filename);

    while ((len = fread(inbuf, 1, CHUNK, temp)) > 0)
    {
        for (int i = 0; i < len; i++)
        {
            if (i + 1 == len)
                fprintf(dest, "0x%02X\n", (unsigned char)inbuf[i]);
            else
                fprintf(dest, "0x%02X, ", (unsigned char)inbuf[i]);
            if (i % 16 == 15)
                fprintf(dest, "\n  ");
        }
    }

    fprintf(dest, "};\n");

    fclose(temp);
    fclose(dest);
    unlink(temp_filename);
    free(filename);
    
    printf("\n\x1b[38;5;39mcreated %s from %s\x1b[0m\n\n", out, in);
    return 0;
}
