# 🌾 Grain

A C++ memory manager for grain-sized objects.

Developed as final project of the *Advanced C++* course for the Master in Computer Game Development.


## Overview

The project is based on Andrei Alexandrescu's `SmallObjectAllocator`,
described in his *Modern C++ Design* book.

The generic, long names he used in his implementation looked pretty ugly to me,
so I gave them a farming twist,
mapping each to a concrete item related to grains of wheat
(symbolizing small objects in memory):
- `Silo`: main storage, serves memory allocations.
  Represents `SmallObjectAllocator`.
- `Sack`: fixed-size storage of wheat. Represents `FixedAllocator`.
- `Handful`: a small handful of grains. Represents `Chunk`.
