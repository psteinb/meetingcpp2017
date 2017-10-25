# using gprof

1. Install [gcc](https://gcc.gnu.org) or [clang](https://clang.llvm.org/) 
2. Compile your code and use the `-pg` flag. With this repo (Linux and macOS supported) do 
```
$ make 
$ ./vector_unroll_example
```
3. Convert the created `gmon.out` to something digestable
```
$ gprof ./vector_unroll_example gmon.out > analysis.txt
```

## Credits

- Peter Gottschling for allowing me to use [his code example](https://github.com/petergottschling/discovering_modern_cpp) from the book "Discovering Modern C++", Peter Gottschling, C++ In-Depth Series, Addison-Wesley, 2016
