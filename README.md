# Personal Repository Manager

- PRM, acronym for Personal Repository Manager, is an easy-to-use application that has, as a primary objective, a way of managing your repositories, giving various possibilities to the users, such as categorizing all the added repositories or controlling the To Do's of each repository.

## How PRM was made

- PRM is written in C++, using only [CLI11](https://github.com/CLIUtils/CLI11) to make the command line parsing process and [json](https://github.com/nlohmann/json) to read and write the user's storage file.

## How to use

### Add a repository

```
$ prm add <repository-name> -l <clone-link> -c <category>
```

### Remove a repository or a list of repositories

```
$ prm remove <repository-name>...
```

### Show your repositories

```
$ prm list
```

### Update the informations of a repository (at least one option is required)

```
$ prm update <repository-name> -n <new-name> -l <new-link> -c <new-category>
```

### Clone a repository

```
$ prm clone <repository-name>
```

### Add a To Do in a repository

```
$ prm todo <repository-name> add <goal>
```

### Remove a list of To Do's from a repository
```
$ prm todo <repository-name> remove <todo-number>...
```

### List the To Do's of a repository
```
$ prm todo <repository-name> list
```

### Update a To Do description
```
$ prm todo <repository-name> update <new-goal> -n <todo-number>
```

## How to install

- If you are using a Linux distro that uses the APT as a package manager you can directly install the latest version with `sudo apt install prm`, but if you are not using just let me know that I will try to add the package to your distro's package manager. If you are using Windows, macOS or any Linux distro that doesn't have APT you need to read the "How to build from source" section.

## How to build from source

- PRM uses [CMake](https://cmake.org/) 3.22.1 as the minimun required version and [make](https://www.gnu.org/software/make/) in the build process, so is extremely easy to build the project from the source code, you just need to execute the install.sh if you are in Linux distro or macOS or the install.ps1 if you are in Windows.

## Troubleshooting

- If after the manual installation process described above you can't execute the binary via terminal typing `prm` and you are a Linux or macOS user please first check if the `$HOME/.local/bin` is in your PATH. In case that your problem is not related with the PATH or you are a Windows user just open a issue and I will try to help you.

## Acknowledgments

- [Antonio Lucas](https://github.com/antoniolucas30) for the README and code typo review!