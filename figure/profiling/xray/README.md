# using xray

1. Install [llvm](https://llvm.org) and [clang](https://clang.llvm.org/) of version 5 or higher 
2. Compile your code and use the `-fxray-instrument` flag. With this repo (Linux and macOS supported) do 
```
$ CXX=clang++ make 
$ XRAY_OPTIONS="patch_premain=true xray_mode=xray-basic verbosity=1" ./vector_unroll_example
```
3. Convert the created `gmon.out` to something digestable
```
$ llvm-xray stack ./xray-log.vector_unroll_example.Ne9vcp -instr_map=./vector_unroll_example 

Unique Stacks: 4
Top 10 Stacks by leaf sum:

Sum: 49050436
lvl   function                                                            count              sum
#0    main                                                                    1      26265129195
#1    void my_axpy<6u, vector<float>, vector<float>, vector<flo...         1365         49050436

Sum: 35871421
lvl   function                                                            count              sum
#0    main                                                                    1      26265129195
#1    void my_axpy<8u, vector<float>, vector<float>, vector<flo...         1105         35871421

Sum: 33140052
lvl   function                                                            count              sum
#0    main                                                                    1      26265129195
#1    void my_axpy<4u, vector<float>, vector<float>, vector<flo...          979         33140052

Sum: 30445134
lvl   function                                                            count              sum
#0    main                                                                    1      26265129195
#1    void my_axpy<2u, vector<float>, vector<float>, vector<flo...          941         30445134
```

## Credits

- Peter Gottschling for allowing me to use [his code example](https://github.com/petergottschling/discovering_modern_cpp) from the book "Discovering Modern C++", Peter Gottschling, C++ In-Depth Series, Addison-Wesley, 2016
