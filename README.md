# Arena_allocator

## Introduction

This code provides a simple memory allocation library called `mavalloc` along with test cases implemented using the `tinytest` framework. The library allows dynamic memory allocation and deallocation, using different allocation strategies such as BEST_FIT, FIRST_FIT, NEXT_FIT, and WORST_FIT.

## Getting Started

To use this code, follow the instructions below:

1. Include the necessary headers: `mavalloc.h` and `tinytest.h`.
2. Make sure to have `stdio.h` and `string.h` included as well.
3. Compile the code along with the `mavalloc.c` and `tinytest.c` files.
4. Run the executable to execute the provided test cases.

## Memory Allocation Strategies

The `mavalloc` library supports the following allocation strategies:

- `BEST_FIT`: Allocates memory using the best-fit strategy, where the smallest free block that can accommodate the requested size is selected.
- `FIRST_FIT`: Allocates memory using the first-fit strategy, where the first free block that can accommodate the requested size is selected.
- `NEXT_FIT`: Allocates memory using the next-fit strategy, where the next free block that can accommodate the requested size is selected.
- `WORST_FIT`: Allocates memory using the worst-fit strategy, where the largest free block that can accommodate the requested size is selected.

## Test Cases

The code includes several test cases implemented using the `tinytest` framework to verify the correctness of the memory allocation library. Each test case focuses on a specific aspect of the library's functionality.

The test cases cover the following scenarios:

1. Initializing the allocator and performing a single allocation.
2. Testing coalescing of free blocks when the second block is freed second.
3. Testing coalescing of free blocks when the second block is freed first.
4. Testing the first-fit strategy and splitting of free blocks.
5. Testing the next-fit strategy and splitting of free blocks.
6. Testing the worst-fit strategy.
7. Testing the best-fit strategy.
8. Testing allocation of more memory than available using the best-fit strategy.
9. Testing allocation of more memory than available using the first-fit strategy.
10. Testing allocation of more memory than available using the worst-fit strategy.
11. Testing the next-fit strategy and splitting of free blocks (additional scenario).
12. Testing the `mavalloc_destroy` function.
13. Testing the next-fit strategy and splitting of free blocks (additional scenario).
14. Testing the best-fit strategy and splitting of free blocks (additional scenario).

## Running the Test Cases

To run the test cases, compile the code and execute the resulting executable. The test cases will be executed, and the results will be displayed. Each test case should pass without any assertion failures.

## Troubleshooting

If any of the test cases fail, please review the code implementation and compare it with the expected behavior described in the comments of each test case.
