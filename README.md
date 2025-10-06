# 🐚 Minishell

> *A journey back in time to when Windows didn't exist, and talking to a computer using aligned 1/0 switches was seriously annoying.*

A minimal yet functional Unix shell implementation written in C as part of the 42 School curriculum. Minishell recreates the core functionality of bash, providing an interactive command-line interface for executing commands, managing processes, and handling I/O redirections.

## 📋 Table of Contents

- [Project Description](#-project-description)
- [Technology Stack](#-technology-stack)
- [Project Architecture](#-project-architecture)
- [Key Features](#-key-features)
- [Getting Started](#-getting-started)
- [Project Structure](#-project-structure)
- [Development Workflow](#-development-workflow)
- [Built-in Commands](#-built-in-commands)
- [Testing](#-testing)
- [Contributing](#-contributing)
- [Authors](#-authors)
- [License](#-license)

## 🎯 Project Description

Minishell is a simple shell implementation that demonstrates fundamental concepts of Unix systems programming, including:

- Process management and execution
- Signal handling
- File descriptor manipulation
- Input/output redirection
- Pipe communication between processes
- Environment variable management
- Command parsing and tokenization

The shell operates in both **interactive** and **non-interactive** modes, providing a functional command-line interface with history support and proper signal handling.

## 🛠️ Technology Stack

### Core Technologies
- **Language**: C (C99 standard)
- **Build System**: GNU Make
- **Libraries**:
  - GNU Readline (for interactive input)
  - Custom `libft` (personal C utility library)
- **Compiler**: Clang with flags: `-Wall -Wextra -Werror -O2`

### System Calls & Functions
The project is restricted to using specific POSIX functions including:
- Process control: `fork`, `execve`, `wait`, `waitpid`, `wait3`, `wait4`
- File operations: `open`, `read`, `write`, `close`, `access`, `dup`, `dup2`, `pipe`
- Directory operations: `opendir`, `readdir`, `closedir`, `getcwd`, `chdir`
- Signal handling: `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`
- Readline functions: `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`
- Terminal control: `isatty`, `ttyname`, `ttyslot`, `ioctl`, `tcsetattr`, `tcgetattr`
- Termcap functions: `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

## 🏗️ Project Architecture

Minishell follows a modular architecture with clearly separated components:

### Core Pipeline

```
Input → Tokenizer → Validator → Parser → Expander → Executor
  ↓                                                    ↓
History                                             Built-ins
```

### Module Breakdown

1. **Main Loop** (`sources/main.c`)
   - Entry point and REPL (Read-Eval-Print Loop)
   - Interactive and non-interactive mode handling

2. **Tokenizer** (`sources/tokenizer/`)
   - Lexical analysis of input
   - Token classification (WORD, PIPE, REDIRECT_IN/OUT, APPEND, HEREDOC)
   - Quote handling (single and double quotes)

3. **Validator** (`sources/validator/`)
   - Syntax validation
   - Token sequence verification
   - Error detection for unclosed quotes and invalid operators

4. **Parser** (`sources/parser/`)
   - Builds command linked list from tokens
   - Handles redirections and file descriptors
   - Command argument assembly

5. **Expander** (`sources/expansion/`)
   - Variable expansion (`$VAR`, `$?`)
   - Quote removal
   - Environment variable substitution

6. **Executor** (`sources/executor/`)
   - Command execution pipeline
   - Process management (fork, exec)
   - Pipe chain handling
   - I/O redirection setup
   - Heredoc implementation

7. **Built-ins** (`sources/builtin/`)
   - Internal command implementations
   - Environment variable management
   - Directory navigation

8. **Signal Handler** (`sources/handler/`)
   - `Ctrl-C` (SIGINT) handling
   - `Ctrl-D` (EOF) handling
   - `Ctrl-\` (SIGQUIT) handling

9. **History Manager** (`sources/history/`)
   - Command history storage
   - History file persistence
   - Readline integration

### Data Structures

**Token Structure** (`t_token`):
- Type classification (WORD, PIPE, redirection operators)
- Linked list for token chain
- Quote state tracking
- Content management

**Command Structure** (`t_command`):
- Argument array
- Input/output file management
- Heredoc delimiter and file descriptor
- Redirection flags (append mode)
- Linked list for pipeline commands

**Expansion Context** (`t_expansion_ctx`):
- Environment variable array
- Token content reference
- Exit status tracking
- Expansion state flags

## ✨ Key Features

### ✅ Implemented Features

- **Interactive Prompt**: Displays a colorful, rotating pastel-colored prompt
- **Command History**: Full history support with persistent storage
- **Executable Search**: Resolves commands using PATH or absolute/relative paths
- **Quote Handling**:
  - Single quotes (`'`) prevent all metacharacter interpretation
  - Double quotes (`"`) preserve literal values except `$`
- **Redirections**:
  - `<` - Input redirection
  - `>` - Output redirection (overwrite)
  - `>>` - Output redirection (append)
  - `<<` - Heredoc (delimiter-based input)
- **Pipes**: Chain multiple commands with `|` operator
- **Variable Expansion**:
  - `$VAR` - Environment variable expansion
  - `$?` - Last exit status expansion
- **Signal Handling**:
  - `Ctrl-C` - Display new prompt on new line
  - `Ctrl-D` - Exit shell
  - `Ctrl-\` - Ignored (no action)

### 🚀 Advanced Features

- **Multiple Redirections**: Supports multiple input/output redirections per command
- **Pipe Chaining**: Unlimited pipeline length
- **Heredoc**: Multi-line input with delimiter matching
- **Colored Output**: Rainbow prompt with pastel colors
- **Memory Management**: Proper cleanup and leak prevention
- **File Descriptor Management**: Centralized FD tracking and cleanup

## 🚀 Getting Started

### Prerequisites

- Clang compiler
- GNU Make
- GNU Readline library
- Linux/Unix operating system

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/peda-cos/minishell.git
   cd minishell
   ```

2. **Compile the project**
   ```bash
   make
   ```

3. **Run the shell**
   ```bash
   ./minishell
   ```

### Build Options

- `make` - Compile the project with progress bar
- `make progress-bar=false` - Compile without progress bar
- `make clean` - Remove object files
- `make fclean` - Remove all generated files
- `make re` - Recompile from scratch
- `make valgrind` - Run with Valgrind memory leak detection

### Usage Examples

```bash
# Basic command execution
minishell$ ls -la

# Pipes
minishell$ cat file.txt | grep "pattern" | wc -l

# Redirections
minishell$ echo "Hello World" > output.txt
minishell$ cat < input.txt > output.txt
minishell$ ls -la >> log.txt

# Heredoc
minishell$ cat << EOF
> Line 1
> Line 2
> EOF

# Variable expansion
minishell$ echo $HOME
minishell$ export VAR=value
minishell$ echo $VAR

# Exit status
minishell$ ls /nonexistent
minishell$ echo $?
```

## 📁 Project Structure

```
minishell/
├── includes/              # Header files
│   ├── minishell.h       # Main header
│   ├── structs.h         # Data structure definitions
│   └── macros.h          # Macro definitions
├── sources/              # Source code
│   ├── main.c            # Entry point
│   ├── builtin/          # Built-in commands
│   ├── executor/         # Command execution
│   ├── expansion/        # Variable expansion
│   ├── handler/          # Signal handling
│   ├── history/          # Command history
│   ├── parser/           # Command parsing
│   ├── tokenizer/        # Input tokenization
│   └── validator/        # Syntax validation
├── libft/                # Custom C library
├── annotations/          # Project documentation (Portuguese)
├── Makefile              # Build configuration
├── leaks.supp            # Valgrind suppressions
└── LICENSE               # MIT License
```

### Source Code Organization

- **`builtin/`**: Implementation of shell built-in commands
- **`executor/`**: Process creation, pipe management, and command execution
- **`expansion/`**: Variable and special parameter expansion
- **`handler/`**: Signal management and terminal control
- **`history/`**: Command history storage and retrieval
- **`parser/`**: Token-to-command conversion
- **`tokenizer/`**: Input string lexical analysis
- **`validator/`**: Syntax error detection

## 🔄 Development Workflow

### Build Process

1. **Header Display**: Shows project banner and authors
2. **Libft Compilation**: Builds the custom C library
3. **Source Compilation**: Compiles all source files with progress tracking
4. **Linking**: Links object files with readline library
5. **Cleanup**: Removes temporary build artifacts

### Development Cycle

1. **Edit** source files
2. **Compile** with `make`
3. **Test** functionality manually or with test scripts
4. **Debug** with `make valgrind` for memory issues
5. **Iterate** until feature is complete

### Code Organization Principles

- **Modular Design**: Each module has a specific responsibility
- **Header Separation**: Structs, macros, and function declarations in separate headers
- **Error Handling**: Comprehensive error checking and reporting
- **Memory Safety**: All allocations paired with proper cleanup
- **Const Correctness**: Extensive use of `const` for immutable data

## 💻 Built-in Commands

Minishell implements the following built-in commands:

| Command  | Description                   | Options                              |
| -------- | ----------------------------- | ------------------------------------ |
| `echo`   | Display a line of text        | `-n` (no trailing newline)           |
| `cd`     | Change directory              | Accepts relative or absolute path    |
| `pwd`    | Print working directory       | No options                           |
| `export` | Set environment variable      | No options (displays all if no args) |
| `unset`  | Remove environment variable   | No options                           |
| `env`    | Display environment variables | No options or arguments              |
| `exit`   | Exit the shell                | Optional exit code                   |

### Built-in Command Details

**`echo [-n] [string ...]`**
- Displays arguments separated by spaces
- `-n` flag suppresses the trailing newline

**`cd [directory]`**
- Changes current working directory
- Updates `PWD` and `OLDPWD` environment variables
- No arguments: changes to `$HOME`

**`pwd`**
- Prints the absolute path of the current directory

**`export [name[=value] ...]`**
- Without arguments: displays all exported variables (sorted)
- With arguments: creates or updates environment variables
- Format: `KEY=VALUE`

**`unset [name ...]`**
- Removes environment variables
- Accepts multiple variable names

**`env`**
- Displays all environment variables in the format `KEY=VALUE`

**`exit [n]`**
- Exits the shell with optional status code
- Without argument: exits with last command's status
- With argument: exits with specified numeric status

## 🧪 Testing

### Manual Testing

Run the shell and test features interactively:

```bash
./minishell
```

### Memory Leak Testing

Use the provided Valgrind configuration:

```bash
make valgrind
```

This runs minishell with:
- Full leak checking
- File descriptor tracking
- Reachable memory analysis
- Readline suppressions (known leaks in readline library)

### Test Scenarios

The project has been tested against various scenarios documented in `annotations/Problemas.md`:

- ✅ Variable expansion with quotes
- ✅ Multiple redirections
- ✅ Export variable updates
- ✅ Heredoc functionality
- ✅ Pipe chains
- ✅ Signal handling
- ✅ Edge cases (empty commands, special characters)

### Known Limitations

- Readline library may report memory leaks (suppressed in Valgrind)
- Some advanced bash features are intentionally not implemented
- Bonus features (`&&`, `||`, wildcards) are not implemented

## 🤝 Contributing

This is an educational project from 42 School. Contributions are welcome for:

- Bug fixes
- Code optimization
- Documentation improvements
- Additional test cases

### Coding Standards

- **Norm Compliance**: Code follows 42 School's Norm (similar to Linux Kernel style)
- **Function Length**: Maximum 25 lines per function
- **Variable Declarations**: At the beginning of functions
- **Error Handling**: All system calls checked for errors
- **Memory Management**: No memory leaks (except readline)
- **Global Variables**: Only one global variable for signal handling

### Development Guidelines

1. **Modular Code**: Keep functions focused on a single task
2. **Descriptive Names**: Use clear, meaningful variable and function names
3. **Documentation**: Comment complex logic and data structures
4. **Error Messages**: Provide helpful error messages
5. **Testing**: Test thoroughly before committing

## 👥 Authors

- **Pedro Monteiro** ([@peda-cos](https://github.com/peda-cos))
- **Jonnathan Lacerda** ([@jonnathan-ls](https://github.com/jonnathan-ls))

*42 São Paulo - 2025*

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<div align="center">

**Made with ❤️ at [42 São Paulo](https://www.42sp.org.br/)**

*"As beautiful as a shell"* 🐚

</div>
