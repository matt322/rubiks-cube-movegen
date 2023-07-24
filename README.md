# rubiks-cube-movegen
Fast 3x3 rubik's cube move generation

I used <a href="https://github.com/martinus/robin-hood-hashing">this hashset</a> rather than the STL one as I found it to be much faster.

This works by storing the state of the cube in an array of 6 ints, where each 4 bits correspond to one square on the cube. This makes move generation pretty fast because it can mostly be done with just bit manipulation.
