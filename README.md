# RC16 - a 16bit RISC computer

RC16 is a 16bit RISC computer which features a well-designed hardware, implemented in Logisim, and its own assembler, written in C++.

![](imgs/cpu.png)

# Usage

First of all run `make` in order to install the RC16 Compiler. Next, you need to write a working program. Some examples can be found in the relative folder. Once done that, run `rcc -i <file>.rc [-o <file>.bin]` to compile your program. Finally, open Logisim and load the generated `<file>.bin` fine into the RAM module. To execute the program, toggle the `power` switch in the main view and hit `Ctrl-k`. For further details head to this repository's wiki.

# License

This software is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 License.

![CC BY-NC-SA](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)