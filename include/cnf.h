#ifndef argo_cnf_h
extern const char *MAIN_C;
extern const char *COMMON_H;
extern const char *GITIGNORE;
extern const char *README;
extern const char *DOCKERFILE;
extern const char *MAKEFILE;
extern const char *SETTINGS;
extern const char *LAUNCH;
extern const char *TASKS;
extern const char *LOGO;
void build_common_h(char **dest, const char *project_name);
void build_makefile(char **dest, const char *project_name);
void build_dockerfile(char **dest, const char *project_name);
void build_readme(char **dest, const char *project_name);
void build_launch(char **dest, const char *project_name);
#endif