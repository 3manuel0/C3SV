## C3SV

> [!WARNING]
> This is my personal project, it's not near complete, and it's mostly for learning purposes.

### About

C3SV is C library for parsing csv files that I'm working on. I had to start over because I didn’t have an optimal start, but now I have [Lib3man](https://github.com/3manuel0/lib3man)(my C library) to help me simplify things and make my work easier.

### Goals

- The ability to parse csv as a struct in memory.
- Have support for most types used in csv (doubles, ints, strings etc...).
- Printing and formating the file in a human readable way.
- Change the data in the csv struct and save it as a file.
- Create a new CSV file from some data.

### Current Progress

- Load csv into a struct in memory (CSV).
- Supports "value, with comma" inside a column.
- Automatic type inference (only int64, float64 and string_view).
- Parsing values into native C types (only int64, float64, string_view).
- Write a json from a csv file.
- Write a csv struct into a csv file.
