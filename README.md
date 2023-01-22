# Advent of code 2022
Learn some C!

# VSCode set up
Added the following extenions:
- C/C++ 
- C/C++ Extensions Pack
- Cpp-reference-and-documentation - adds display for library docs when highlighting code and press CTRL+SHIFT+A to show cpp documenation page.

# Exploring memory leaks
* [valgrind](https://valgrind.org/) - mentioned lots, not clear on mac compatability as `brew install valgrind` says "Linux is required for this software."
* [Leaks](https://developer.apple.com/library/archive/documentation/Performance/Conceptual/ManagingMemory/Articles/FindingLeaks.html), a Mac OS specific tool. Example usage:
```
leaks -atExit -- ./a.out
```

# Useful links

* [Everything I wish I knew when learning C](https://tmewett.com/c-tips/)
* [C Reference](https://en.cppreference.com/w/c)
* [C Programming tutorial](https://www.tutorialspoint.com/cprogramming/index.htm)
* [What every C programmer should know about undefined behaviour](http://blog.llvm.org/2011/05/what-every-c-programmer-should-know.html)
# [Make files - automatic variables](https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html)
# [Make files - pattern rules](https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html)