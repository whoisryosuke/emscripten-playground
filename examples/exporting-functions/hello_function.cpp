// emcc hello_function.cpp -o function.html -sEXPORTED_FUNCTIONS=_int_sqrt -sEXPORTED_RUNTIME_METHODS="ccall,cwrap"
// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <math.h>

extern "C" {

int int_sqrt(int x) {
  return sqrt(x);
}

}