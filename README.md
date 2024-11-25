# Formal Quantum Algorithm Methodology (FQAM)

A methodology for formally deriving quantum algorithms. Inspired by [FLAME](https://shpc.oden.utexas.edu/index.html). 

Currently being worked on. The goal is to provide a framework to think visually about quantum algorithms in terms of the configuration space as opposed to the gate model. The conventional gate model for quantum computing is obtuse and unintuitive. It is much easier to conceptualize a quantum algorithm in terms of its behavior in state space configuration, observing how the program state changes and evolves over time.

## Requirements

- [Raylib](https://github.com/raysan5/raylib/releases)
- [Libflame](https://github.com/flame/libflame)
- [A BLAS implementation](https://github.com/flame/blis)

Theoretically, any BLAS implementation works, like [OpenBLAS](https://github.com/OpenMathLib/OpenBLAS) or LAPACK implementation. However, all have been untested. One just needs to figure out how to link the desired DLA backend with Libflame, and it should work in theory. See Libflame documentation for more details on linking [here.](https://github.com/flame/libflame/blob/master/docs/libflame/libflame.pdf)


## Credits

- [Arraylist implemention](https://github.com/marekweb/datastructs-c/tree/master?tab=readme-ov-file) by Marek Web
- [C Assert macro library](https://github.com/leiless/assertf.h) by Leiless

## TODO

- Move list.c, list.h too util and fix the makefile to compile it