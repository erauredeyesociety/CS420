# CS 420 Assignment 1: Step-by-Step Completion Guide

## Overview
Complete a producer-consumer program using shared memory and bounded buffer synchronization.

## Step 1: Prepare Files
1. Download the provided `producer.c` and `consumer.c` files
2. Update the header comments in both files:
   - Replace `Group #` with your actual group number
   - Replace `Section #` with your section number
   - List all operating systems you tested on (must include Linux/Ubuntu)

## Step 2: Implement Missing Functions

### In `producer.c`:
1. **Update shared memory name**: Replace `"OS_HW1_yourGroup#"` with your actual group number (e.g., `"OS_HW1_5"`)
2. **Complete `InitShm()` function**:
   - Create shared memory using `shm_open()`
   - Set size using `ftruncate()`
   - Map memory using `mmap()`
   - Initialize header values
3. **Complete `Producer()` function**:
   - Implement bounded buffer producer logic with busy waiting
   - Generate random numbers using `GetRand(0, 2500)`
   - Write to buffer using provided functions
4. **Complete helper functions**:
   - `SetHeaderVal()`: Copy value to shared memory location
   - `ReadAtBufIndex()`: Read value from buffer index
5. **Add command line validation**: Check that buffer size is between 2-800

### In `consumer.c`:
1. **Update shared memory name**: Use the same name as producer
2. **Complete `main()` function**:
   - Open existing shared memory using `shm_open()`
   - Map memory using `mmap()`
   - Read header values
   - Implement consumer logic with busy waiting
3. **Complete helper functions**:
   - `SetHeaderVal()`: Copy value to shared memory location
   - `ReadAtBufIndex()`: Read value from buffer index

## Step 3: Implement Synchronization Logic

### Producer Logic:
```text
for each item to produce:
    wait while buffer is full: ((in + 1) % bufSize == out)
    generate random value
    write value to buffer[in]
    update in = (in + 1) % bufSize
```

### Consumer Logic:
```text
for each item to consume:
    wait while buffer is empty: (in == out)
    read value from buffer[out]
    update out = (out + 1) % bufSize
```

## Step 4: Compile and Test
1. **Compile both programs**:
   ```bash
   gcc producer.c -lrt -o producer
   gcc consumer.c -lrt -o consumer
   ```

2. **Test with sample arguments**:
   ```bash
   ./producer 5 100 10
   ```
   - Buffer size: 5
   - Items to produce/consume: 100
   - Random seed: 10

3. **Verify output**:
   - Producer should show: "Producing Item X with value Y at Index Z"
   - Consumer should show: "Consuming Item X with value Y at Index Z"
   - Values should match between producer and consumer

## Step 5: Validation Checklist
- [ ] Buffer size validation (2-800) implemented
- [ ] Shared memory name matches between producer and consumer
- [ ] All required functions implemented
- [ ] Proper synchronization (no race conditions)
- [ ] Programs compile without errors
- [ ] Test runs successfully with various buffer sizes and item counts
- [ ] Header comments completed with group info
- [ ] Code tested on Linux/Ubuntu

## Step 6: Test Different Scenarios
1. **Small buffer, many items**: `./producer 3 50 123`
2. **Large buffer, few items**: `./producer 100 10 456`
3. **Equal buffer and items**: `./producer 20 20 789`

## Step 7: Submit
- Submit both `producer.c` and `consumer.c` files
- Ensure filenames are exactly as specified
- Verify all code compiles and runs on Linux systems

## Common Issues to Avoid
- Forgetting to update shared memory name in both files
- Missing `-lrt` flag during compilation
- Not implementing busy waiting correctly
- Forgetting to validate command line arguments
- Using incorrect buffer synchronization logic