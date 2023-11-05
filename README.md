# chroniclock

C++ implementation of Seqlock with benchmarks. Compared and benchmark against
a generic reader-writer lock with writer priority.

A seqlock is also a reader-writer lock but it is aimed at pretty much
never starving writers. Readers will try to optimistically read the data, and
if the data has changed since reading it, it will retry again (note: writers do
not wait for readers).

Because of this optimistic reading, it performs much better when there are many
readers and few writers. 

Note: type T used must be trivially copy-assignable.

# Benchmarking

We will benchmark the amount of time it takes for N reads to occur within M threads.
This is happening while another thread is constantly updating/writing new data within
the seqlock.

# Checklist

TO-DO:
- Implement Seqlock
- Implement Read-Writers Lock/Queue
- Implement Tests
- Benchmark padding vs no padding
- Benchmark in general

DONE:
