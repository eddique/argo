#ifndef argo_cmd_h

int new_project(char *project_name);
int add_lib(char *lib_name);
int run_bin(int argc, char *argv[]);
int build_bin();
int install_bin();
int uninstall_bin(char *bin_name);
int list_binaries();
#endif