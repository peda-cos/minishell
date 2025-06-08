# 🐚 Minishell

[![42 Project](https://img.shields.io/badge/42-Project-blue)](https://42.fr)
[![C](https://img.shields.io/badge/C-99-orange)](https://en.wikipedia.org/wiki/C99)
[![Norminette](https://img.shields.io/badge/Norminette-Passing-green)](https://github.com/42School/norminette)

> **A fully functional Unix shell implementation in C that mimics bash behavior.** 🚀

Built as part of the 42 School curriculum, this project recreates the core functionality of a Unix shell from scratch, providing an educational deep-dive into system programming, process management, and command-line interfaces.

---

## 🌟 Features

### Core Shell Functionality
- 🖥️ **Interactive prompt** with colored display
- 📜 **Command history** with persistent storage
- 🔍 **Command execution** from PATH or absolute/relative paths
- 🔧 **Environment variable** management
- 📁 **Working directory** navigation

### Built-in Commands
- `echo` - Display text with `-n` option support
- `cd` - Change directory (supports `~`, `-`, and absolute/relative paths)
- `pwd` - Print working directory
- `export` - Set environment variables with bash-style display
- `unset` - Remove environment variables
- `env` - Display environment variables
- `exit` - Exit shell with optional exit code
- `history` - Command history management

### Advanced Features
- 🔗 **Pipes** (`|`) - Chain commands together
- 📥 **Input redirection** (`<`) - Redirect input from files
- 📤 **Output redirection** (`>`) - Redirect output to files
- 📝 **Heredoc** (`<<`) - Multi-line input with delimiter
- 🔤 **Quote handling** - Single (`'`) and double (`"`) quotes
- 🌿 **Variable expansion** - Environment variable substitution (`$VAR`)
- ⚡ **Signal handling** - Ctrl+C, Ctrl+D, Ctrl+\ support

---

## 🛠️ Installation

### Prerequisites

Ensure you have the following installed on your Linux system:

```bash
# Essential build tools
sudo apt update && sudo apt install -y \
  build-essential \
  gcc \
  make \
  libreadline-dev \
  valgrind

# Or on macOS with Homebrew
brew install readline valgrind
```

### Quick Start

```bash
# Clone the repository
git clone https://github.com/peda-cos/minishell.git
cd minishell

# Build the project
make

# Run the shell
./minishell
```

### Build Options

```bash
# Clean build (removes object files)
make clean

# Full clean (removes objects and executable)
make fclean

# Rebuild everything
make re

# Build without progress bar
make progress-bar=false

# Run with memory leak detection
make valgrind
```

---

## 🚦 Usage

### Starting the Shell

```bash
# Interactive mode (default)
./minishell

# Non-interactive mode (from file or pipe)
echo "ls -la" | ./minishell
./minishell < script.sh
```

### Basic Commands

```bash
# Navigation
minishell$ pwd
/home/user/minishell
minishell$ cd ..
minishell$ cd ~
minishell$ cd -

# File operations
minishell$ ls -la
minishell$ cat file.txt
minishell$ echo "Hello World"

# Environment variables
minishell$ export NAME="John Doe"
minishell$ echo $NAME
John Doe
minishell$ env | grep NAME
NAME=John Doe
minishell$ unset NAME
```

### Advanced Usage

#### Pipes and Redirection
```bash
# Simple pipe
minishell$ ls | grep minishell

# Multiple pipes
minishell$ cat file.txt | grep "pattern" | wc -l

# Input redirection
minishell$ wc -l < input.txt

# Output redirection
minishell$ ls > output.txt
minishell$ echo "append this" >> output.txt

# Heredoc
minishell$ cat << EOF
> This is line 1
> This is line 2
> EOF
This is line 1
This is line 2
```

#### Quote Handling and Variable Expansion
```bash
# Single quotes (literal)
minishell$ echo 'Hello $USER'
Hello $USER

# Double quotes (with expansion)
minishell$ echo "Hello $USER"
Hello john

# Variable expansion
minishell$ NAME="World"
minishell$ echo "Hello $NAME!"
Hello World!

# Exit status
minishell$ echo $?
0
```

---

## 📁 Project Structure

```
minishell/
├── 📄 Makefile                 # Build configuration
├── 📄 README.md               # This file
├── 📄 leaks.supp              # Valgrind suppressions
├── 📁 includes/               # Header files
│   ├── minishell.h           # Main header
│   ├── structs.h             # Data structures
│   └── macros.h              # Macro definitions
├── 📁 libft/                  # Custom C library
├── 📁 sources/                # Source code
│   ├── main.c                # Entry point
│   ├── utils.c               # Utility functions
│   ├── 📁 builtin/           # Built-in commands
│   ├── 📁 executor/          # Command execution
│   ├── 📁 expansion/         # Variable expansion
│   ├── 📁 handler/           # Signal handling
│   ├── 📁 history/           # Command history
│   ├── 📁 parser/            # Command parsing
│   ├── 📁 tokenizer/         # Input tokenization
│   └── 📁 validator/         # Input validation
```

---

## 📚 Learning Resources

### Understanding the Code
- **Tokenizer**: Breaks input into tokens (words, operators, etc.)
- **Parser**: Converts tokens into command structures
- **Executor**: Runs commands with proper process management
- **Expansion**: Handles variable substitution and quote removal
- **Signals**: Manages Ctrl+C, Ctrl+D, and other interrupts

### Key Concepts
- 🔄 **Process creation** with `fork()` and `execve()`
- 🔗 **Pipe communication** between processes
- 📂 **File descriptor** management
- 🌐 **Environment variable** handling
- ⚡ **Signal handling** for interactive use

### Related Reading
- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/)

---

## 📄 License

This project is part of the 42 School curriculum. Feel free to use it for educational purposes.

---

## 🎯 Project Goals Achieved

- ✅ **Shell prompt** with readline integration
- ✅ **Command history** with persistent storage
- ✅ **Built-in commands** (echo, cd, pwd, export, unset, env, exit)
- ✅ **PATH resolution** and executable search
- ✅ **Pipes** and **redirections** (`|`, `<`, `>`, `<<`)
- ✅ **Quote handling** (single and double quotes)
- ✅ **Variable expansion** (`$VAR`, `$?`)
- ✅ **Signal handling** (Ctrl+C, Ctrl+D, Ctrl+\)
- ✅ **Memory management** (no leaks)
- ✅ **Error handling** with proper exit codes

---

### Authors
- **Pedro Monteiro** (peda-cos) - *Developer*
- **Jonnathan Lacerda** (jlacerda) - *Developer*

---

<div align="center">

**Built with ❤️ for 42 School São Paulo**

*Happy coding! 🚀*

</div>
