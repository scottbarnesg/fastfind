# FastFind

A fast memory-backed file serach tool. It provides constant-time lookup via a memory-efficient search index (MARISA-Trie).

## Installation

1. Install dependencies: `make install-deps`
1. Compile: `make`

## Useage

1. Start the background service: `bin/fastfind-service`
1. Search for a file: `bin/fastfind <search-term>`

** NOTE: It may take a few minutes for the search index to build the first time your run `fastfind`.

## Performance

In the following example, I've created a file called "test.txt" and would like to search for it. `fastfind` completes >25x faster than `find`:

```
$ time bin/fastfind test.txt
/home/<user>/test.txt

real	0m0.457s
user	0m0.367s
sys	0m0.072s
```

```
time find / -name test.txt
...
/home/<user>/test.txt

real	0m12.957s
user	0m0.575s
sys	0m2.139s
```