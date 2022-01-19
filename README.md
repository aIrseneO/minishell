# minishell
This project is to create a simple shell. None of the [exec](https://man7.org/linux/man-pages/man3/exec.3.html) family functions is used. Input parser, redirection, pipes and some signals are implemented from scratch.
## How to run it?
[GCC](https://gcc.gnu.org/ "gcc web page") is used to compile see **Makefile**.<br>

To create the executable and binaries run
```bash
make [minishell]
```
To launch the minishell run
```bash
./minishell
```
To clean object files run:
```bash
make clean
```
To clean library and object files run:
```bash
make fclean
```
To restart, meaning clean all and make the library run:
```bash
make re
```
To make the library and clean object files at once run
```bash
make f
```
## Known bug
```
cd < a
```
This command should not exit the shell as in bash
