# RC16 - a 16bit RISC computer

RC16 is a 16bit RISC computer which features a well-designed hardware, implemented in Logisim, and its own assembler, written in C++.

![Screenshot](imgs/cpu.png)

## Usage

First of all run `make` in order to install the RC16 Compiler. Next, you need to write a working program. Some examples can be found in the relative folder. Once done that, run `rcc -i <file>.rc [-o <file>.bin]` to compile your program. Finally, open Logisim and load the generated `<file>.bin` fine into the RAM module. To execute the program, toggle the `power` switch in the main view and hit `Ctrl-K`. For further details head to this repository's wiki.

## License

This software is licensed under the [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 License](https://creativecommons.org/licenses/by-nc-sa/4.0/). This means that you are allowed to remix, transform, adapt, and build upon the software included in this repository, you can copy and redistribute it in any medium or format, under the following terms:

1. **Attribution** &ndash; you must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

2. **NonCommercial** &ndash; you may not use the software included in this repository for commercial purposes.

3. **ShareAlike** &ndash; if you remix, transform, or build upon the software included in this repository, you must distribute your contributions under the same license as the original.

![CC BY-NC-SA](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)
