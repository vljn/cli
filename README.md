# C++ Command Line Interpreter

A modular C++ command line interpreter application with support for:

- pluggable commands
- command parsing and validation
- batch execution with nested streams
- command pipelining
- input and output redirection

## Example
```
$ echo "Hello"  
Hello  
$ echo "Hello" | wc -c  
5  
$ time >out.txt  
12:00  
$ touch new.txt
```
  
## Quick start

### Build
```
git clone https://github.com/vljn/cli
cd cli
cmake -S . -B build
cmake --build build
```

### Run

**Windows**:  
`./build/Debug/cli.exe`

**Linux / macOS**:  
`./build/cli`
