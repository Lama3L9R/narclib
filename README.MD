# narclib

narclib, or Native ARCaea LIBerator, a c++ library for liberate Arcaea's deepest secrete.

This project is not open-source. 
Any dissemination, distribution, copying, or use of this information to distribute, publish in any form is strictly prohibited 
without the explicit permission of the owner of this repository.

---

This project is made of 3 parts: narchook, narchash, and common.

## NArcHook

A library for hooking Arcaea with LSPosed.

## NArcHash

An ArcHash implementation in C++ and grants the ability to generate X-Random-Challenge.

### Usage
```c++
#include "narchash.h"

void main() {

  char*      path = ""; 
  char*      body = ""; 
  uint64_t   ts = 0LL; 
  arc_salt_t salt = { /* put salt here */ }; 
  char*      result;

  uint8_t error;
  if ((error = narchash::generate_challenge(path, body, ts, salt, &result)) != GENERATE_SUCCESS) {
    // on error
  }

  if ((error = narchash::verify_challenge(result, path, body, ts, salt)) != VERIFY_NO_ERROR) {
    // verify failed
  }
}
```
