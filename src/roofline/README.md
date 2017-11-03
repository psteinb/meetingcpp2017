# Creating a roofline plot for vector operation

## Goal

Use the vector unrolling example from Peter Gottschling's book, [c++11/vector_unroll_example.cpp](https://github.com/petergottschling/discovering_modern_cpp/blob/master/c%2B%2B11/vector_unroll_example.cpp) and [c++03/vector_unroll_example2.cpp](https://github.com/petergottschling/discovering_modern_cpp/blob/master/c%2B%2B03/vector_unroll_example2.cpp). This code wants to perform the operation:

$u = 3*v + w$

where $u,v,w$ are 1D vectors of arbitrary length. Each vector holds double precision floating point numbers. In other terms, this code does nothing else than:

```
double* u,v,w; // pointers to 1D vectors
for(int i = 0;i<len;++i){
    u[i] = 3.*v[i] + w[i];
}
```

## Hardware

| model  | Lenovo T430s                      | tool/file                    |
|--------|-----------------------------------|------------------------------|
| CPU    | Intel Core i7-3520M CPU @ 2.90GHz | `lscpu`, `cat /proc/cpuinfo` |
| memory | 2x8GB DDR3 SODIMM @ 1600 MT/s     | `sudo dmidecode -t memory`   |

- according to the [specs](http://www.cpu-world.com/CPUs/Core_i7/Intel-Core%20i7-3520M%20(BGA)%20Mobile%20processor.html):
    -  a single core can clock to 3.6 GHz
    -  both cores can clock to 3.4 GHz

### Theoretical Memory Bandwidth

- bandwidth per DIMM
    + 1600 MT/s * 64Bit data width = 12.8 Gb/s

- bytes per cycle
    + at 2.9GHz, 1 cycle takes = 0.35 ns = .35 * 10^-9 sec
    + one second yields 2.9*10^9 cycles
    + [12.8 Gb/s] / [2.9 GHz] = 4.4137 bytes/cycle
    
- double check with [likwid-bench](https://github.com/RRZE-HPC/likwid/wiki/Likwid-Bench) (version 4.2.1):
```
# likwid-bench  -t copy -w S0:1GB
Allocate: Process running on core 0 (Domain S0) - Vector length 62500000/500000000 Offset 0 Alignment 512
Allocate: Process running on core 0 (Domain S0) - Vector length 62500000/500000000 Offset 0 Alignment 512
--------------------------------------------------------------------------------
LIKWID MICRO BENCHMARK
Test: copy
--------------------------------------------------------------------------------
Using 1 work groups
Using 4 threads
--------------------------------------------------------------------------------
Running without Marker API. Activate Marker API with -m on commandline.
--------------------------------------------------------------------------------
Group: 0 Thread 0 Global Thread 0 running on core 0 - Vector length 15625000 Offset 0
Group: 0 Thread 1 Global Thread 1 running on core 1 - Vector length 15625000 Offset 15625000
Group: 0 Thread 3 Global Thread 3 running on core 3 - Vector length 15625000 Offset 46875000
Group: 0 Thread 2 Global Thread 2 running on core 2 - Vector length 15625000 Offset 31250000
--------------------------------------------------------------------------------
Cycles:			15031303467
CPU Clock:		2893232666
Cycle Clock:		2893232666
Time:			5.195332e+00 sec
Iterations:		256
Iterations per thread:	64
Inner loop executions:	3906250
Size (Byte):		1000000000
Size per thread:	250000000
Number of Flops:	0
MFlops/s:		0.00
Data volume (Byte):	64000000000
MByte/s:		12318.75
Cycles per update:	3.757826
Cycles per cacheline:	30.062607
Loads per update:	1
Stores per update:	1
Load bytes per element:	8
Store bytes per elem.:	8
Load/store ratio:	1.00
Instructions:		11000000016
UOPs:			14000000000
--------------------------------------------------------------------------------
```
    
### CPU performance

- more info on [cpu-world.com](http://www.cpu-world.com/CPUs/Core_i7/Intel-Core%20i7-3520M%20(BGA)%20Mobile%20processor.html)
- obtain the [reference manual](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-optimization-manual.html)

    + on Oct 30, 2017, the above mentioned manual reports on page C-7 (table C-8) in units of cycles
    
    | operation | latency | throughput |     | flops/cycle |
    |-----------|---------|------------|-----|-------------|
    | VADDPD    |       3 |          1 | AVX |           4 |
    | VMULPD    |       5 |          1 | AVX |           4 |
    |           |         |            |     |             |

    + cross checks with double precision likwid-bench runs
    
    | benchmark |      cycles | number of flops | flops/cycle |                 |
    |-----------|-------------|-----------------|-------------|-----------------|
    | daxpy_avx | 15189031180 |      7999991808 |        0.53 | Y = const*X + Y |
    |           |             |                 |             |                 |

- obtain [Agner Fog's instruction latency reference](http://www.agner.org/optimize/instruction_tables.pdf) (version May 2, 2017)

    + as AVX is yields the highest possible bandwidth on this machine, we use this as the basis
    + on this CPU (similar to Sandy Bridge), the document above yields the following latencies
    | operation |                                    | latency | throughput | flops/cycle | page |
    |-----------|------------------------------------|---------|------------|-------------|------|
    | VMOVAPD   | load 256bits                       |       4 |          1 |           0 |  194 |
    | VMOVAPD   | store 256bits (2 times 128 stores) |       4 |          2 |           0 |  194 |
    | VADDPD    | addition                           |       3 |          1 |           4 |  196 |
    | VMULPD    | multiplication                     |       5 |          1 |           4 |  197 |
    + for the operation at hand that means, we need 2 cycles to process 1 loop iteration with AVX
        1. LOAD + 50% STORE + MUL + ADD
        2. LOAD + 50% STORE
    + that means for an entire 64Byte cache-line of 8 `float64` values, we need 2*2 cy = 4 cycles minimum
        
- What is the maximum CPU performance we can obtain per core for this vector operation on a single core?
$ 3.4 * 10^9 cy/s * 4 updates (as this is AVX) * 2 flops/update / 2 cycles = 13.6 Gflops/s $

- What is the peak performance of the CPU for a single core? (note, 1 AVX multiplication and 1 AVX addition can run in parallel as it's 2 distinct ports on Ivy Bridge)
$ 3.4 * 10^9 cy/s * (4 + 4) flops/cy  = 27.2 Gflops/s $ (this algorithm could max to 50% of peak performance)
        
- What is the maximum memory bandwidth we can obtain? 
    + 12.8 GB/s
    + in theory, this should be cross checked by the number of updates that the system can perform per second (TODO)

# Code variants

## naiive 

- what is the arithmetic intensity per loop iteration?
    + all operations are assume to be in cache 
    + 1+1 word for the result vector element of u (assume write-allocate policy here)
    + 2 words for the input vector element of v,w
    + 2 flops per item
    + 4 words / 2 flops = 2 W/Flop = code balance
    + arithmetic intensity = (code balance)^-1 = .5 Flop/W = .0625 Flop/B

- measured that with likwid

## hardwired unrolled 

- what is the arithmetic intensity per loop iteration? Let's assume we unroll 8 iterations (2 AVX lines)
    + all operations are assume to be in cache 
    + 8*(1+1) word for the result vector element of u (assume write-allocate policy here)
    + 8*2 words for the input vector element of v,w
    + 8*2 flops per item
    + 8*4 words / 8*2 flops = 2 W/Flop = code balance
    + arithmetic intensity = (code balance)^-1 = .5 Flop/W = .0625 Flop/B

- measured that with likwid



# References

+ http://spiral.net/software/roofline.html
    - related: http://spiral.ece.cmu.edu:8080/pub-spiral/abstract.jsp?id=177
    - related: http://dx.doi.org/10.3929/ethz-a-007305123
    
+ https://moodle.rrze.uni-erlangen.de/pluginfile.php/10660/mod_resource/content/1/04_Roofline_Jacobi.pdf
