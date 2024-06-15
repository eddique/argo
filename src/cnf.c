#include "cnf.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
const char *MAIN_C =
    "#include \"common.h\"\n\n"
    "int main(int argc, char *argv[])\n"
    "{\n\n"
    "\tprintf(\"Hello, World!\\n\");\n\n"
    "\treturn 0;\n"
    "}";

const char *COMMON_H =
    "#ifndef %s_common_h\n\n"
    "#include <stdio.h>\n\n"
    "#endif";

const char *GITIGNORE =
    ".env\n"
    ".DS_STORE\n"
    "*.o\n"
    "bin\n"
    "obj\n";

const char *DOCKERFILE =
    "FROM alpine:latest AS build\n\n"
    "WORKDIR /app\n\n"
    "RUN apk update && apk add --no-cache make gcc musl-dev\n\n"
    "COPY src ./src\n\n"
    "COPY include ./include\n\n"
    "COPY makefile .\n\n"
    "RUN mkdir -p bin obj\n\n"
    "RUN make\n\n"
    "RUN chmod +x /app/bin/%s\n\n"
    "FROM scratch\n\n"
    "COPY --from=build /app/bin/%s /%s\n\n"
    "EXPOSE 8080\n\n"
    "ENTRYPOINT [ \"./%s\" ]\n";

const char *MAKEFILE =
    "CC = gcc\n"
    "CFLAGS = -Wall -Iinclude -Os -ffunction-sections -fdata-sections\n"
    "LDFLAGS = # -Wl,--gc-sections -static\n"
    "SRC_DIR = src\n"
    "OBJ_DIR = obj\n"
    "BIN_DIR = bin\n"
    "SOURCES = $(wildcard $(SRC_DIR)/*.c)\n"
    "OBJECTS = $(patsubst $(SRC_DIR)/%%.c, $(OBJ_DIR)/%%.o, $(SOURCES))\n"
    "EXECUTABLE = $(BIN_DIR)/%s\n"
    "all: dirs $(EXECUTABLE)\n"
    "	strip $(EXECUTABLE)\n"
    "dirs:\n"
    "	mkdir -p $(OBJ_DIR) $(BIN_DIR)\n"
    "$(EXECUTABLE): $(OBJECTS)\n"
    "	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@\n"
    "$(OBJ_DIR)/%%.o: $(SRC_DIR)/%%.c\n"
    "	$(CC) $(CFLAGS) -c $< -o $@\n"
    "clean:\n"
    "	rm -f $(OBJ_DIR)/*.o\n"
    "	rm -f $(BIN_DIR)/*\n"
    ".PHONY: all clean\n";

const char *README =
    "# %s\n\n"
    "## Quick Start\n\n"
    "## Usage\n\n";

const char *SETTINGS =
    "{\n"
    "    \"files.associations\": {\n"
    "        \"*.h\": \"c\",\n"
    "        \"*.c\": \"c\",\n"
    "        \"string\": \"c\",\n"
    "        \"string_view\": \"c\"\n"
    "    },\n"
    "    \"C_Cpp.default.includePath\": [\"${workspaceFolder}/**\"],\n"
    "    \"C_Cpp.default.intelliSenseMode\": \"gcc-x64\",\n"
    "    \"C_Cpp.default.compilerPath\": \"/usr/bin/gcc\",\n"
    "    \"C_Cpp.loggingLevel\": \"Debug\",\n"
    "    \"C_Cpp.default.configurationProvider\": \"ms-vscode.cmake-tools\",\n"
    "    \"lldb.consoleMode\": \"commands\"\n"
    "}";

const char *LAUNCH =
    "{\n"
    "    \"version\": \"0.2.0\",\n"
    "    \"configurations\": [\n"
    "        {\n"
    "            \"type\": \"lldb\",\n"
    "            \"request\": \"launch\",\n"
    "            \"name\": \"Launch Program\",\n"
    "            \"program\": \"${workspaceFolder}/bin/%s\",\n"
    "            \"args\": [],\n"
    "            \"cwd\": \"${workspaceFolder}\",\n"
    "            \"preLaunchTask\": \"build\"\n"
    "        }\n"
    "    ]\n"
    "}";

const char *TASKS = "{\n"
                    "    \"version\": \"2.0.0\",\n"
                    "    \"tasks\": [\n"
                    "        {\n"
                    "            \"label\": \"build\",\n"
                    "            \"type\": \"shell\",\n"
                    "            \"command\": \"make clean && make\",\n"
                    "            \"group\": {\n"
                    "                \"kind\": \"build\",\n"
                    "                \"isDefault\": true\n"
                    "            },\n"
                    "            \"presentation\": {\n"
                    "                \"reveal\": \"always\"\n"
                    "            },\n"
                    "            \"problemMatcher\": [\n"
                    "                \"$gcc\"\n"
                    "            ]\n"
                    "        }\n"
                    "    ]\n"
                    "}";
const char *LOGO =
    "\n"
    ".d8888b. 88d888b. .d8888b. .d8888b. \n"
    "88'  `88 88'  `88 88'  `88 88'  `88 \n"
    "88.  .88 88       88.  .88 88.  .88 \n"
    "`88888P8 dP       `8888P88 `88888P' \n"
    "                       .88          \n"
    "                    d8888P          \n"
    "\n";

void build_common_h(char **dest, const char *project_name)
{
    ssize_t size = strlen(COMMON_H) + strlen(project_name) - 2;
    char *name = strdup(project_name);
    for (int i = 0; name[i]; i++)
    {
        if (name[i] == '-')
            name[i] = '_';
    }
    *dest = (char *)realloc(*dest, (size * sizeof(char)) + 1);
    snprintf(*dest, size + 1, COMMON_H, name);
    free(name);
}
void build_makefile(char **dest, const char *project_name)
{
    ssize_t size = strlen(MAKEFILE) + strlen(project_name) - 2;
    *dest = (char *)realloc(*dest, (size * sizeof(char)) + 1);
    snprintf(*dest, size + 1, MAKEFILE, project_name);
}
void build_dockerfile(char **dest, const char *project_name)
{
    ssize_t size = strlen(DOCKERFILE) + (strlen(project_name) * 4) - 2;
    *dest = (char *)realloc(*dest, (size * sizeof(char)) + 1);
    snprintf(*dest, size + 1, DOCKERFILE, project_name, project_name, project_name, project_name);
}
void build_readme(char **dest, const char *project_name)
{
    ssize_t size = strlen(README) + strlen(project_name) - 2;
    *dest = (char *)realloc(*dest, (size * sizeof(char)) + 1);
    snprintf(*dest, size + 1, README, project_name);
}

void build_launch(char **dest, const char *project_name)
{
    ssize_t size = strlen(LAUNCH) + strlen(project_name) - 2;
    *dest = (char *)realloc(*dest, (size * sizeof(char)) + 1);
    snprintf(*dest, size + 1, LAUNCH, project_name);
}