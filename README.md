# minishell
In this project I create a simple shell. A command parser is implemented as the project restrictions require. Redirection, Pipes and some signals are implemented as well, see [project initial restrictions](https://drive.google.com/file/d/1HNAZDhHzQ4zPMkp8eE4SVfP4wlUWlWLZ/view?usp=sharing).
## How to run it?
[GCC](https://gcc.gnu.org/ "gcc web page") is used to compile see **Makefile**.<br>
To get set the absolute path for new binaries run
```bash
make path
```
this will update the absolute path in the header file to the current directory.<br>
This can be reverse with
```bash
make nopath
```
<br>
To create the executable and binaries run
```bash
make
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
