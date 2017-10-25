# using gperftools

1. Install the project from [github.com/gperftools/gperftools](https://github.com/gperftools/gperftools)
2. make gperftools available through your environment
3. Compile your code and link to libprofiler.so/.dll. With this repo (Linux and macOS supported) do 
```
$ make 
$ export CPUPROFILE=my.prof
$ ./vector_unroll_example
$ pprof --web ./vector_unroll_example my.prof
```


## Credits

- Peter Gottschling for allowing me to use [his code example](https://github.com/petergottschling/discovering_modern_cpp) from the book "Discovering Modern C++", Peter Gottschling, C++ In-Depth Series, Addison-Wesley, 2016
