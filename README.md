# dextr

This is ```dextr```, a Distributed computing EXecuToR for pleasingly parallel workloads

## How to use it

```
Usage: dextr [options] taskfile
Arguments:
  taskfile         file containing tasks to launch
Options:
  -v [ --version ]      print version string
  -h [ --help ]         show this help message
```

with ```taskfile``` being a file containing tasks to be launched, one per line.

Run ```dextr``` with ```mpirun```:

    $ mpirun -n N dextr tasks.list > tasks.log

Tasks are scattered accross processors so all of them are workers.

## Compilation

    $ mkdir build && cd build
    $ cmake ..
    $ make

## TODO

- [ ] Separate the tasks execution part from the logs filtering (```dextr launch```, ```dextr filter```)
- [ ] Add a ```--time-remaining``` option for graceful exit when run with a batch scheduler
- [ ] Implement it in Rust