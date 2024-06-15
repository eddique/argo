<p align="center">
    <img width="400" src="./docs/img/logo-light.png#gh-light-mode-only" alt="argo Logo">
    <img width="400" src="./docs/img/logo.png#gh-dark-mode-only" alt="argo Logo">
</p>
<h2 align="center">A build tool CLI for scaffolding C projects</h2>
<p align="center">
    <img src="https://img.shields.io/badge/version-0.1.0-000000">
    <img src="https://img.shields.io/badge/built%20with-C-00427A.svg">
    <img src="https://img.shields.io/badge/license-MIT-750014">
</p>

## Usage

### Format

```sh
argo [cmd] <oprion>
```

### Commands

```sh
# Usage:
argo [new] <project_name> # scaffolds a project
argo [add] <library_name> # adds a library
argo [build] <args> # builds the application
argo [run] <args> # runs the application with supplied args
argo [install] <args> # installs the argo managed binary
argo [uninstall] <bin_name> # uninstalls an argo manaaged binary
argo [ls] # lists argo manaaged binaries
argo [--version] # prints installed argo version
argo [--help] # displays usage menu
```

### Examples
```sh
argo new project
cd project
argo build
argo run
argo install
argo ls
argo uninstall project
cd ..
rm -rf project
argo --version
argo --help
```

### new

Create a new project in the current working directory

```sh
argo new my_project
```

This command scaffolds the project to the following:

```sh
my_project
├── .vscode
│   └── launch.json
│   └── settings.json
│   └── tasks.json
├── Dockerfile
├── README.md
├── include
│   └── common.h
├── makefile
└── src
    └── main.c
```

### add

```sh
argo add libev
```

### build

```sh
argo build
```

### install

```sh
argo install
```

### run

```sh
argo run
```
