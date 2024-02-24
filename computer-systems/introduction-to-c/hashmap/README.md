# Basic Hashmap

## Unit testing

The `./main.c` contains the tests for the hashmap implementation, but I also needed to
add unit tests for the linkset implementation.

```sh
ssfw -p linkset.c -c 'cc -g -DUNIT_TESTING linkset.c && ./a.out && leaks --atExit -- ./a.out'
````

To unit test all of the code:

```sh
ssfw -p '*.c' -c 'cc -g main.c && ./a.out && leaks --atExit -- ./a.out'
```

To run debugger on the tricky part:

```sh
lldb -o 'b 35' -o 'b linkmap.c:55' -o 'display i' -o 'display key' -o 'display h->maps[7]->head->key' -o 'display h->maps[7]->head->value' ./a.out
```

> [!IMPORTANT]
> Relies on my [ssfw](https://github.com/mharrisb1/ssfw) utility.
