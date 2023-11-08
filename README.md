# chroniclock

C++ implementation of Seqlock with benchmarks. 

A seqlock is a reader-writer lock but it is aimed at pretty much
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

I also benchmarked against an implementation of a readers-writer lock with writer
priority. Aka readers aren't allowed to read more if there's a waiting writer. It
uses condition variables and mutexes for synchronisation. It is SIGNIFICANTLY
slower - I would argue if you are able to use a Seqlock (e.g. your data is atomically
readable), I would use it in almost every scenario. 

# Checklist

TO DO:

DONE:
- Implement Seqlock
- Implement Tests
- Benchmark padding vs no padding
- Benchmark in general
- Implement Reader-Writers Lock
- Benchmark Reader-Writers Lock
