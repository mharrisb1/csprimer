```sh
 cc main.c -DPROC_COUNT=$(grep -c ^processor /proc/cpuinfo) -g -O0
```
