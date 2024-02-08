# Personal Repository Manager

- PRM, acronym for Personal Repository Manager, is an easy-to-use application that has, as a primary objective, a way of managing your repositories, giving various possibilities to the users, such as categorizing all the added repositories or controlling the To Do's of each repository.

## How PRM was made

- PRM is written in C++, using only [CLI11](https://github.com/CLIUtils/CLI11) to make the command line parsing process, [json](https://github.com/nlohmann/json) to read and write the user's storage file and [colors.hpp](https://github.com/hugorplobo/colors.hpp) to colorize some parts of the output.

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

### Clone a repository (git is needed)

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

### Get PRM's current version
```
$ prm version
```

## How to install

- PRM uses [CMake](https://cmake.org/) 3.22.1 as the minimun required version and [make](https://www.gnu.org/software/make/) in the installation process, so is extremely easy to install the project from the source code, you just need to have them and execute the install.sh if you are using a Linux distro or macOS or the install.ps1 if you are using Windows. After that you need to restart the session and everything should be fine. Also, remember to clone the repository with `--recurse-submodules`.

## Troubleshooting

- If after the installation described above in the "How to install" section you can not execute the binary via terminal just typing `prm` you can just open an issue and I will try to help.

## Uninstalling PRM

- If you are on Linux or macOS you just need to execute the following steps:

```sh
$ rm $HOME/.local/bin/prm
$ rm $HOME/.config/.prm.json
```

> You also have the option to open the .profile file located in your home directory and remove the `.local/bin` path inclusion that is automatically added by PRM if it does not exists. But keep in mind that if you remove it, you can broke other apps.

- But if you installed manually but are on Windows you just need to execute the following steps:
```ps1
> rm $env:ProgramFiles\prm
> rm $env:APPDATA\.prm.json
> $oldPath = [Environment]::GetEnvironmentVariable("PATH", "User")
> $newPath = $oldPath.Replace(";$env:ProgramFiles\prm", "")
> [Environment]::SetEnvironmentVariable("PATH", $newPath, "User")
```

## Acknowledgments

- [Antonio Lucas](https://github.com/antoniolucas30) for the README and code typo review!