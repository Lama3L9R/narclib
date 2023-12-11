# `daemonc` Code style
 
A C/C++ coding style by lamadaemon. The reason for making this guild is that C++ is 95% garbage.

When you are contributing to this project, please make sure that you have followed these rules.

You should use `clang-format` with provided `.clang-format` config file to format all the files first.
You have to double-check if clang-format does it correctly, because there is some rules that clang-format doesn't support.
This guild included all the rules that clang-format cannot handle.

## Vocabularies

- `Should`: You must do it.
- `Should not`: You must not do it.
- `Encourage`: If you don't want to do it that's alright. However, the best choice is to do it.
- `✅`: Good / You **should** do it
- `❌`: Bad / You **should not** do it

## Special Rules Clang-Format Can't handle

1. Add space after the struct name during struct declaration.
    - ```C
      // ✅. The following comment will temporally turn off clang-format
      // clang-format off 
      my_struct_t { 
          ..., // You must add this comma or clang-format will mass up your struct
      };
      ```
   - ```C
      // ❌
      my_struct_t{
          ...};
      ```
   - Since Clang-Format will automatically format your good struct to bad one. You have to disable it temporally.

## Basic Code style

1. You are **encouraged** to not use C++. If you need advanced typing / abstraction, use Rust instead.
2. You **should** follow C code style even if you are using C++.
    - You **should not** use `opearation new`.  You **should** use `malloc` instead.
3. You are **encouraged** to not use c++ std library.
    - Since c++ std library is using classes everywhere, this will make "sizing a class" difficult
    - A few simple questions for you if you claim that you'd like to use c++ std library: 
        + How many bytes do `std::vector` take? What's the memory structure be like?
        + How many bytes do `std::string` take? What's the memory structure be like?
    - The above questions have no answers because I didn't give out the context.
    - Different compilers will have different implementations for c++ std library.
    - Many C/C++ programmers don't know about these.
    - Different implementations will confuse people if they have never written C++ code on your platform / environment.
    - However, c++ std library still provides a lot of convenient features. You have to decide for this.
    - But if you can avoid it, then avoid it.
4. You are **encouraged** to not use classes, templates, crazy abstract layers, since this is not an OOP language.
   - For explanations, please check above.
5. You **should not** use OOP even C++ support OOP, since this is not an OOP language.
6. If you are using C++, you are **encouraged** use namespaces.
7. You **should not** use `nullptr` to represent `0`
8. You **should not** use C23. You are **encouraged** to use C11.
9. You **should** always use cstdint to represent primitive types.
   - Exceptions are: string (use `char*` or `char[]`), generic pointers (`void*`)
   - Example:
   - ✅ `uint32_t my_var;`
   - ✅ `size_t my_var;`
   - ✅ `char* my_string;` 
   - ✅ `uint8_t* my_byte_array;`
   - ✅ `void* my_buffer = malloc(10);`
   - ❌ `unsigned long long my_var;`
   - ❌ `char* my_byte_array;`
10. You **should** consider pointers are types.
   - `[data_type]*` is a type. You **should not** let `*` connected with symbol name.
   - Example:
   - ✅ `void* my_buffer = malloc(10);`
   - ❌ `void *my_buffer = malloc(10);`
11. You are **encouraged** not to override operators in C++.
   - Becuase some of the operator overrides are bad and confusing.
   - Example:
   - ✅ `my_vec += my_element;`
   - ❌ `std::cout << "hello world";`
12. You **should** add brackets around all bitwise operations even they have operation priority.
13. You **should** declare only one variable in one line.
   - Example:
   - ✅
     - ```c
       uint32_t my_var;
       uint32_t my_var2;
       ``` 
   - ❌ `uint32_t my_var, my_var2;`

## Naming

1. You **should** use snake_case everywhere.
   - If your program needs specific names that are not snake_case, it is allowed.
   - For example, JNI functions need to name like this:
      + ✅ `Java_com_example_myapp_myFunction`
2. You **should** add `_t` after type declaration.
    - Example: `typedef struct my_struct {...} my_struct_t;`
3. If you are using C, functions are one of the exceptions to using snake_case. You **should** use function name as a alternative of namespace.
    - To make namespaces clearer in C, you **should** use Big_Snake_Case followed by snake_case function name.
    - Example: 
      + ✅ In C++: `void myapp::utils::my_fn();`
      + ✅ In C: `void MyApp_Utils_my_fn();` 
4. Constant macros are one of the exceptions to using snake_case. You **should** use UPPER_SNAKE_CASE.