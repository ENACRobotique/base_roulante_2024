set pagination off
target extended-remote /dev/bmp_gdb
monitor connect en
monitor version
monitor swdp_scan
attach 1
monitor vector_catch disable hard
load
detach
quit

