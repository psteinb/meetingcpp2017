# using gprof

1. Install [gcc](https://gcc.gnu.org) or clang 
2. make gperftools available through your environment
3. Compile your code and link to libprofiler.so/.dll. With this repo (Linux and macOS supported) do 
```
$ make 
$ ./vector_unroll_example
$ gprof ./vector_unroll_example gmon.out > analysis.txt
```


## Credits

- Peter Gottschling for allowing me to use [his code example](https://github.com/petergottschling/discovering_modern_cpp) from the book "Discovering Modern C++", Peter Gottschling, C++ In-Depth Series, Addison-Wesley, 2016
