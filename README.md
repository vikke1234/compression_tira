# Data compression TIRA
Programme: CS

## Dependencies
- [Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository)

## Build instructions
```sh
git clone https://github.com/vikke1234/compression_tira.git
cd compression_tira
mkdir build
cmake ..
make tira
```

NOTE: it might not work fully yet, it should work on "simple repetitive data", because the lengths of the paths may be too long (>16 bits) it will crash and burn. I'm working currently on a solution for it but it still requires testing.
## Documentation
Comparison of different compression algorithms (see below which will be
compared, there may be added more in the future).

Documentation will be written in English.

[Project specification](project_spec.md)

## Weekly reports
[Week 1](weekly_reports/week1.md)
[Week 2](weekly_reports/week2.md)

[Test coverage](index.html)
