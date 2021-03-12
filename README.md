# Data compression TIRA
Programme: CS
## Note
Doesn't work on binary files, this may be due to the larger alphabet size.
Though it should work fine on text files. May be fixed at a later date.

## Test Dependencies
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

# Usage
Compression
```shell
./tira -c filename
```
Decompression
```shell
./tira -d filename
```
The filename after compression will be `output`

[Project specification](project_spec.md)

## Weekly reports
[Week 1](weekly_reports/week1.md)
[Week 2](weekly_reports/week2.md)
[Week 3](weekly_reports/week3.md)
[Week 4](weekly_reports/week4.md)
[Week 5](weekly_reports/week5.md)
[Week 7](weekly_reports/week7.md)

[Test coverage](index.html) may be outdated
