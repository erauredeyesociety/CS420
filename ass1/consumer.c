/*
CS 420 
Assignment 1: Shared Memory and Multi-Tasking
Group # <- just your group number in this line
Section # <- just your section number
OSs Tested on: Linux, Ubuntu, Mac, etc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

// Size of shared memory block
// Pass this to ftruncate and mmap
#define SHM_SIZE 4096

// Global pointer to the shared memory block
// This should receive the return value of mmap
// Don't change this pointer in any function
void* gShmPtr;

// You won't necessarily need all the functions below
void SetIn(int);
void SetOut(int);
void SetHeaderVal(int, int);
int GetBufSize();
int GetItemCnt();
int GetIn();
int GetOut();
int GetHeaderVal(int);
void WriteAtBufIndex(int, int);
int ReadAtBufIndex(int);

int main()
{
    const char *name = "OS_HW1_yourGroup#"; // Name of shared memory block to be passed to shm_open
    int bufSize; // Bounded buffer size
    int itemCnt; // Number of items to be consumed
    int in; // Index of next item to produce
    int out; // Index of next item to consume

    // Open the shared memory object created by producer
    int shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Error opening shared memory\n");
        exit(1);
    }

    // Map the shared memory to our process
    gShmPtr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (gShmPtr == MAP_FAILED) {
        printf("Error mapping shared memory\n");
        exit(1);
    }

    // Read the four integers from the header of the shared memory block 
    bufSize = GetBufSize();
    itemCnt = GetItemCnt();
    in = GetIn();
    out = GetOut();

    // Check that the consumer has read the right values
    printf("Consumer reading: bufSize = %d\n", bufSize);

    // Consume all items produced by the producer
    for (int i = 0; i < itemCnt; i++) {
        // Wait while buffer is empty
        do {
            in = GetIn();
            out = GetOut();
        } while (in == out);

        // Read value from buffer
        int val = ReadAtBufIndex(out);
        printf("Consuming Item %d with value %d at Index %d\n", i, val, out);
        
        // Update out pointer
        out = (out + 1) % bufSize;
        SetOut(out);
    }
    
    // remove the shared memory segment 
    if (shm_unlink(name) == -1) {
        printf("Error removing %s\n",name);
        exit(-1);
    }

    return 0;
}


// Set the value of shared variable "in"
void SetIn(int val)
{
    SetHeaderVal(2, val);
}

// Set the value of shared variable "out"
void SetOut(int val)
{
    SetHeaderVal(3, val);
}

// Get the ith value in the header
int GetHeaderVal(int i)
{
    int val;
    void* ptr = gShmPtr + i * sizeof(int);
    memcpy(&val, ptr, sizeof(int));
    return val;
}

// Set the ith value in the header
void SetHeaderVal(int i, int val)
{
    void* ptr = gShmPtr + i * sizeof(int);
    memcpy(ptr, &val, sizeof(int));
}

// Get the value of shared variable "bufSize"
int GetBufSize()
{       
    return GetHeaderVal(0);
}

// Get the value of shared variable "itemCnt"
int GetItemCnt()
{
    return GetHeaderVal(1);
}

// Get the value of shared variable "in"
int GetIn()
{
    return GetHeaderVal(2);
}

// Get the value of shared variable "out"
int GetOut()
{             
    return GetHeaderVal(3);
}


// Write the given val at the given index in the bounded buffer 
void WriteAtBufIndex(int indx, int val)
{
    // Skip the four-integer header and go to the given index 
    void* ptr = gShmPtr + 4 * sizeof(int) + indx * sizeof(int);
    memcpy(ptr, &val, sizeof(int));
}

// Read the val at the given index in the bounded buffer
int ReadAtBufIndex(int indx)
{
    int val;
    void* ptr = gShmPtr + 4 * sizeof(int) + indx * sizeof(int);
    memcpy(&val, ptr, sizeof(int));
    return val;
}