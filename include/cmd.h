#ifndef argo_cmd_h

int new_project(int argc, char *argv[]);
int add_lib(int argc, char *argv[]);
int run_bin(int argc, char *argv[]);
int build_bin();
int install_bin();
int uninstall_bin(int argc, char *argv[]);
int list_binaries();
void print_help(int show_logo);
#endif