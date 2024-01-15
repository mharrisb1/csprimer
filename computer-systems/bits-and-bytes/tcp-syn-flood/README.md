# TCP SYN Flood

## Running the code

From project root:

```sh
cd syn-flood/ && cargo build && cd .. && cat synflood.pcap | syn-flood/target/debug/syn-flood
```

**Warning**: This code make the assumption that a file is not malformed or invalid and 
does not need to be byte swapped. Use with caution.

## Links

1. [SYN Flood Wiki](https://en.wikipedia.org/wiki/SYN_flood)

## Man Pages

1. `man pcap-savefile`
