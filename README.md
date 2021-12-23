# FastFind

A fast memory-backed file serach tool. It provides constant-time lookup via a memory-efficient search index (MARISA-Trie).

## Installation

1. Install dependencies: `make install`
1. Compile: `make`

## Useage

1. Start the background service: `bin/fastfind-service`
1. Search for a file: `bin/fastfind <search-term>`

** NOTE: It may take a few minutes for the search index to build the first time your run `fastfind`.