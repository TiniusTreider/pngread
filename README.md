# PNGREAD

This tool reads **PNG** files. It makes it easy to get the metadata of a **PNG** file or figure out why it is corrupted.

Works on all major operating systems.

# Build

<<<<<<< HEAD
You need CMake 3.1 or above and GCC / Clang / MSVC etc.
=======
You need **CMake 3.1** or above and **GCC** / **Clang** / **MSVC**.
>>>>>>> 590855c (better formatting and gitignored tests)

```bash
pngread> cd build
pngread/build> cmake ..
pngread/build> make
```

On non-Unix systems, replace 'make' with your build tool (e.g. 'ninja').

# Usage

```bash
pngread/build> ./pngread <path/to/png>
```

# Example

```bash
pngread/build> ./pngread ~/cow.png
```
(I/O under construction)

