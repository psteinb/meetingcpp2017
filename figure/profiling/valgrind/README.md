# using kcachegrind

1. Install [valgrind](http://valgrind.org/) and [kcachegrind](https://kcachegrind.github.io/html/Home.html)
2. Compile your code. With this repo (Linux and macOS supported) do 
```
$ make 
```
3. Run the code using valgrind
```
$ valgrind --tool=callgrind ./vector_unroll_example #wait for a long time
$ kcachegrind
```

## Credits

- Peter Gottschling for allowing me to use [his code example](https://github.com/petergottschling/discovering_modern_cpp) from the book "Discovering Modern C++", Peter Gottschling, C++ In-Depth Series, Addison-Wesley, 2016
