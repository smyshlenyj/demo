# Calc

calc is a simple CLI-calculator.

## Usage
```
calc <number1> <operation> <number2>
calc <number> !
calc --help
```

## Operations
```
+   addition
-   subtraction
x   multiplication
/   division
^   power (a ^ b)
!   factorial (n!)
```

## Notes
- For factorial, only one operand is used
- Factorial is defined for non-negative integers
- Division by zero is an error

## Examples
```
calc 2 + 3
calc 5 !
calc 2 ^ 8
```

## Build and Install

### Requirements

- CMake ≥ 3.16
- C++ compiler with C++17 support

### Build

From the project root:

```sh
cmake -B build
cmake --build build
```
This will configure the project and build the calc executable in the build/ directory.

### Install

To install the binary into the system (default prefix: /usr/local):

```sh
sudo cmake --build build --target install
```