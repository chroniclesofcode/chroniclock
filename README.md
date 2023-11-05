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

Note: I have also benchmarked against Erik Rigtorp's implementation of a Seqlock.
I have also taken some inspiration from his code, and benchmarked with/without
padding that is meant to reduce the chances of false sharing. From my tests however,
the padding doesn't seem to make much of an impact.

# Checklist

TO DO:

DONE:
- Implement Seqlock
- Implement Read-Writers Lock/Queue
- Implement Tests
- Benchmark padding vs no padding
- Benchmark in general
