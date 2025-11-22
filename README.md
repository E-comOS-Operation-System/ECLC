# ECLC - E-comOS C/C++ Language Compiler
![License: GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)<br>
![Platform](https://img.shields.io/badge/Platform-E--comOS-orange) <br>
This compiler is suitable for use when you want to run or compile C/C++ programs on E-comOS. It is more than just a compiler; for convenience, we have integrated a runtime (C/C++) into the compiler.<br>
However, how you actually write your program is not the compiler's responsibility, but the editor's. E-comOS currently doesn't have any compatible editors, not even nano (GNU probably doesn't care much about us either).
## How to install it
You can use EPM (In E-comOS) to download it 
```
epm install eclc
```
In Nov 22 , 2025 , we **only support E-comOS** 'cause macOS , Linux and Windows , they have themselves compiler (e.g., Clang , GCC)
## How to use it 
### C programs
If you want to compilation a C programs , type 
```
eclc  <file_name.c> --c-code
```
Or you want to compilation a folder of files ,  maybe you want to use CMake , but , **we are happy to inform you that we do not support CMake either.**<br>
But my friends, don't worry, because I myself am stuck developing with clang on macOS for the same reason (damn it, typing gcc results in clang being called).<br>
We need a solution!<br>
Now , you only need type:
```
eclc -f <folder_name>
```
### C++ programs
Sometimes our coder must use C++ to work but I don't need 'cause I'm C coder, but sometimes C do not support string , so I must use C++ , how to compilation your C++ code ? Only need
```
eclc <file_name.cpp> --cpp-code
``` 
If you want to compilation a projects code , you only need
```
eclc -f <folder_name> --cpp-code
```
That's all of it!
## License
We use GNU GPL Version 3 , 'cause myself like GNU license , I'm used Apache license in a RUI apps , but ... I don't like that

---------------

Music:I love—love GNU—U oh oh oh oh oh (rising key)—but they don't even glance at me—eye—.<br>
```bash
ddd@saladin-macbookair ~/ECLC (main)> man gcc
No manual entry for gcc
```
