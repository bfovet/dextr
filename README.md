# dextr

This is ```dextr```, a Distributed computing EXecuToR for pleasingly parallel workloads

## Launch tasks

```
Usage: 
  dextr [options] <command>
Commands:
  launch   execute tasks from taskfile
  filter   filter tasks logs from logfile
Options:
  -v [ --version ]      print version string
  -h [ --help ]         show this help message
```

with ```taskfile``` being a file containing tasks to be launched, one per line.

Run ```dextr``` with ```mpirun```:

    $ mpirun -n N dextr launch tasks.list > tasks.log

Tasks are scattered accross processors so all of them are workers.

Use the ```SIGUSR1``` signal to gracefully interrupt the execution.

## Filter logs

TODO

## Compilation

    $ mkdir build && cd build
    $ cmake ..
    $ make

## TODO

- [ ] Separate the tasks execution part from the logs filtering (```dextr launch```, ```dextr filter```)
- [ ] Add a ```--time-remaining``` option for graceful exit when run with a batch scheduler
- [ ] Use clang-tidy
- [ ] Implement it in Rust
