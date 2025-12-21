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

## Build
cmake -B build
cmake --build build
sudo cmake --build build --target install