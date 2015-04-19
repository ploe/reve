# psilocin

## What is psilocin?

![psilocin](https://ploe.co.uk/psilocin-logo.gif)

A Communal Game Dev Platform - **WIP**

The client is written in C and SDL 2.0, with [murp](https://github.com/ploe/murp) and libcurl.

The server is written in Ruby, with Sinatra.

## Compile Client

Point to your instance of libmurp. Mine is in my home dir. Make sure you have libcurl and SDL 2 installed.

Mac OS X

```bash
gcc-mp-4.9 *.c -o psi `pkg-config sdl2 --cflags --libs` `curl-config --cflags` -I/Users/myke/libmurp-osx -L/Users/myke/libmurp-osx -lmurp -lSDL2 -lcurl
```

Linux - Debian 64-bit

```bash
 cc *.c -o psi -g `pkg-config sdl2 libcurl --cflags --libs` -I/home/myke/libmurp-debian64 -L/home/myke/libmurp-debian64 -lmurp
```

## License (Simplified BSD - 2 Clause)

```
Copyright (c) 2015, Myke Atkinson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
