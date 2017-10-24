# how to reproduce the ethminer flamegraph

## obtain ethminer

- go to [github.com/ethereum-mining/ethminer](https://github.com/ethereum-mining/ethminer)
- obtain and build it
```
$ git clone https://github.com/ethereum-mining/ethminer
$ cd ethminer
$ mkdir build
$ cmake -DCMAKE_INSTALL_PREFIX=/somewhere/safe -DETHASHCUDA=ON -DHUNTER_JOBS_NUMBER=16 .. #note I had to limit the number of jobs
$ make install
```

## obtain flamegraph

- go to [github.com/brendangregg/FlameGraph](https://github.com/brendangregg/FlameGraph)
- obtain it and you are done
```
$ git clone https://github.com/brendangregg/FlameGraph
```

## run the profiling and produce the flamegraph

**NB** I assume your environment is set up correctly so that the ethminer binary and the flamegraph perl scripts are found!

Then on Linux do to collect samples of 120s of ethminer:

```
$ perf record -g timeout 120 ethminer --cuda -Z
$ perf script > ethminer.perf
$ stackcollapse-perf.pl ethminer.perf > ethminer.folded
$ flamegraph.pl ethminer.folded > ethminer.svg
```

