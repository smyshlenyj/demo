# Calc

**Calc** — простой CLI-калькулятор с поддержкой базовых операций и факториала.  
Ввод осуществляется через JSON-файл, результаты логируются и выводятся в консоль.

Калькулятор сохраняет результаты вычислений в PostgreSQL и использует кэш в памяти, чтобы ускорять повторные вычисления одинаковых операций. При повторном запуске с теми же аргументами результат будет взят из кэша без повторного расчёта, а новые вычисления автоматически сохраняются в базу для будущего использования.

---

## Usage

```bash
calc <input.json>
```

JSON Format
```json
{
  "firstOperand": 5,
  "secondOperand": 3,
  "operation": "+"
}
```
**firstOperand** — целое число (int64_t)

**secondOperand** — целое число (int64_t, опционально для факториала !)

**operation** — один из поддерживаемых символов: +, -, x, /, ^, !

Для факториала используется только firstOperand.

**Notes:**

Деление на ноль — ошибка.

Факториал определяется только для неотрицательных чисел.

Поля JSON должны быть корректными, иначе приложение выбросит исключение и завершится с ошибкой.

## Operations
**Symbol Description**
```text
+	Addition
-	Subtraction
x	Multiplication
/	Division
^	Power (a ^ b)
!	Factorial (n!)
```
# Examples
```bash
calc input.json
```

input.json:
```json
{
  "firstOperand": 5,
  "secondOperand": 3,
  "operation": "+"
}
```

Output:
```text
Result: 8
```

Для факториала:
```json
{
  "firstOperand": 5,
  "operation": "!"
}
```

Output:
```text
Result: 120
```

# Logging

Все события программы логируются через spdlog.

Лог хранит:
* Запуск и завершение калькулятора
* Принятый JSON
* Ошибки и исключения (например, деление на ноль, неверный JSON)
* Ошибки и исключения дублируются в консоль.

Пример уровня логирования:

**trace** — вход в методы

**info** — успешное завершение операций

**error** — ошибки обработки JSON или валидации

**critical** — фатальные ошибки


# Unit Tests
Используется Google Test (gtest) для тестирования всех операций.

Тесты покрывают:
* Все базовые арифметические операции
* Деление на ноль
* Факториал и возведение в степень

##### Запуск тестов через CMake:

```bash
cmake -B build
cmake --build build
ctest --output-on-failure
```

# Build and Install
**Requirements:**
* CMake ≥ 3.16
* C++17 compiler

**Build**
```bash
cmake -B build
cmake --build build
```

**Install**
```bash
sudo cmake --build build --target install
```

## Инициализация тестовой базы PostgreSQL

Выполните следующие команды для настройки тестовой базы:

```bash
# Переключиться на пользователя postgres
sudo -i -u postgres

# Выполнить скрипт инициализации из проекта, например последовательно запустив команды из каталога проекта:
sudo cp db/init_db.sql /tmp/init_db.sql
sudo chown postgres:postgres /tmp/init_db.sql
sudo -i -u postgres
psql -f /tmp/init_db.sql
