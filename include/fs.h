#ifndef argo_fs_h
int scaffold(char *project_name);
int copy_file(const char *src, const char *dest);
int list_entries(const char *directory);
#endif