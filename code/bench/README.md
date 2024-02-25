# Benhchmark

## CPU Scaling

If you get a warning related to enabling CPU scaling, use the following commands to enable/disable CPU scaling:

```sh
sudo cpupower frequency-set --governor performance
```

Run your benchmark

```sh
./benchmark
```

Don't forget to disable CPU scaling

```sh
sudo cpupower frequency-set --governor powersave
```
