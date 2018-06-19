# Report

## Design
Base on sample codes.
### mmap supports for master device
* Implement mmap interfaces to enable kernel space buffer sharing with user space programs.
* Implement ioctl functionality to let user space programs make a request to device driver in order to trigger device driver to send out the data in shared buffer through a socket.  
### mmap supports for slave device
* Implement mmap interfaces to enable kernel space buffer sharing with user space programs.
* Implement ioctl functionality to let user space programs make a request to device driver in order to trigger device driver to receive data from a socket and write the data to shared buffer. 

## Execution Results
```
----- Master method (fcntl), Slave method (fcntl) -----

File: file1_in
Master:
Transmission time: 0.032800 ms, File size: 32 bytes
Slave:
Transmission time: 0.137300 ms, File size: 32 bytes

File: file2_in
Master:
Transmission time: 0.024900 ms, File size: 4619 bytes
Slave:
Transmission time: 0.155300 ms, File size: 4619 bytes

File: file3_in
Master:
Transmission time: 0.107500 ms, File size: 77566 bytes
Slave:
Transmission time: 0.249800 ms, File size: 77566 bytes

File: file4_in
Master:
Transmission time: 7.208000 ms, File size: 12022885 bytes
Slave:
Transmission time: 8.977100 ms, File size: 12022885 bytes

----- Master method (fcntl), Slave method (mmap) -----

File: file1_in
Master:
Transmission time: 0.016000 ms, File size: 32 bytes
Slave:
Transmission time: 0.080100 ms, File size: 32 bytes

File: file2_in
Master:
Transmission time: 0.018400 ms, File size: 4619 bytes
Slave:
Transmission time: 0.087500 ms, File size: 4619 bytes

File: file3_in
Master:
Transmission time: 0.090500 ms, File size: 77566 bytes
Slave:
Transmission time: 0.267200 ms, File size: 77566 bytes

File: file4_in
Master:
Transmission time: 10.603500 ms, File size: 12022885 bytes
Slave:
Transmission time: 15.642100 ms, File size: 12022885 bytes

----- Master method (mmap), Slave method (fcntl) -----

File: file1_in
Master:
Transmission time: 0.017200 ms, File size: 32 bytes
Slave:
Transmission time: 0.073200 ms, File size: 32 bytes

File: file2_in
Master:
Transmission time: 0.017800 ms, File size: 4619 bytes
Slave:
Transmission time: 0.084700 ms, File size: 4619 bytes

File: file3_in
Master:
Transmission time: 0.024700 ms, File size: 77566 bytes
Slave:
Transmission time: 0.116300 ms, File size: 77566 bytes

File: file4_in
Master:
Transmission time: 4.003000 ms, File size: 12022885 bytes
Slave:
Transmission time: 12.219500 ms, File size: 12022885 bytes

----- Master method (mmap), Slave method (mmap) -----

File: file1_in
Master:
Transmission time: 0.017100 ms, File size: 32 bytes
Slave:
Transmission time: 0.078200 ms, File size: 32 bytes

File: file2_in
Master:
Transmission time: 0.024200 ms, File size: 4619 bytes
Slave:
Transmission time: 0.096000 ms, File size: 4619 bytes

File: file3_in
Master:
Transmission time: 0.024200 ms, File size: 77566 bytes
Slave:
Transmission time: 0.118500 ms, File size: 77566 bytes

File: file4_in
Master:
Transmission time: 4.795900 ms, File size: 12022885 bytes
Slave:
Transmission time: 15.718200 ms, File size: 12022885 bytes
```

## File I/O V.S. Memory-mapped I/O
### Master
Memory-mapped I/O is faster than file I/O.
#### Why?
The number of system calls required for memory-mapped I/O is less than file I/O.
### Slave
Memory-mapped I/O is slightly slower than file I/O for small files but it is significantly slower than file I/O for large files.
#### Why?
Since the file size can not be known beforehand, we can not directly map the entire output file. Instead, we have to allocate file space, map and unmap the file for each write operation. This will pose a lot of overheads compared to a write system call, and the overheads are propotional to the file size.

## Member contributions
B05902036 夏瑜： implementation, report  
B05302248 翁茂慶： supports  
B05902084 吳俊飛： ?  
B05902132 林敬文： ?  