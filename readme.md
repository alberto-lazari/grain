# 🌾 Grain

A C++ memory manager for grain-sized objects.

Developed as final project of the *Advanced C++* course for the Master in Computer Game Development.


## Overview

The project is based on Andrei Alexandrescu's `SmallObjectAllocator`,
described in his *Modern C++ Design* book.

The generic, long names he used in his implementation looked pretty ugly to me,
so I decided to give them a farming twist,
mapping each to a concrete item related to grains of wheat:

| Name      | Analogy                                  | Original Name          |
| --------- | ---------------------------------------- | ---------------------- |
| `Silo`    | Main storage of different kinds of grain | `SmallObjectAllocator` |
| `Sack`    | Set of grains of the same kind (size)    | `FixedAllocator`       |
| `Handful` | A small handful of grains                | `Chunk`                |
| `Grain`   | Smallest distributable unit              | Memory block           |

> [!NOTE]
> I'm well aware that this entire *grain metaphor*-thing makes the code pretty unreadable,
> but readability and usability in general are really not the point here.
>
> This is a simple University project, aiming to replicate an existing implementation,
> and is obviously not meant to appear in real-world codebases.
> I just wanted to have some fun while working on it.
