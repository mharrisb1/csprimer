# Basic Hashmap

## Unit testing

The `./main.c` contains the tests for the hashmap implementation, but I also needed to
add unit tests for the linkset implementation.

```sh
ssfw -p linkset.c -c 'cc -g -DUNIT_TESTING linkset.c && ./a.out && leaks --atExit -- ./a.out'
````

> [!INFORMATION]
> Makes use of my [ssfw](https://github.com/mharrisb1/ssfw) utility.
