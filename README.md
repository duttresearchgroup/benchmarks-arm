# Overview
Pointers to benchmark sources and precompiled binaries for arm

# Content

* *fetch.sh*: fetches:
  * *benchmarks*: contains compiled arm binaries and inputs
    * *mibench*: compiled from https://github.com/tiagormk/mibench.git
    * *parsec*: compile from http://parsec.cs.princeton.edu/download/3.0/parsec-3.0.tar.gz + the parsec-arm-static.diff patch (from https://n.ethz.ch/~pfistchr/download/parsec-arm-static.diff)
    * *parsec/x264/bin/x264_mod*: alternative binary compiled from https://github.com/tiagormk/x264.git
  * *benchmarks.img*: disc image to use with user-level qemu+chroot to compile/recompile the benchmarks. Contains the source of all benchmark at benchmarks.img/benchmarks/
* *qemu-\*.sh*: scripts related to benchmarks.img

