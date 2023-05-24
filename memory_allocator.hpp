#pragma once

#include <unistd.h>
#include <mutex>
#include <iostream>

// Node structure to store memory blocks
struct Block {
    size_t size;
    Block* next;
};

// Global variables
Block* free_blocks_head = nullptr;
std::mutex mtx;

// Function to allocate memory
void* _malloc(size_t size) {
    mtx.lock();

    Block* block = free_blocks_head;
    Block* prev = nullptr;

    // Find a free block with sufficient size
    while (block && (block->size < size)) {
        prev = block;
        block = block->next;
    }

    // If there is an already available block satisfying the need, use it
    if (block) {
        std::cout << "Found an existing free block. Making use of it." << std::endl;
        
        // If block size is larger than required, split the block and make the remaining memory available.
        if (block->size > size + sizeof(Block)) {
            std::cout << "Existing free block is larger than required. Splitting it." << std::endl;
            Block* newBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(block) + sizeof(Block) + size);
            newBlock->size = block->size - size - sizeof(Block);
            newBlock->next = block->next;
            block->size = size;
            block->next = newBlock;
        }

        // Remove the allocated block from the free list
        if (prev)
            prev->next = block->next;
        else
            free_blocks_head = block->next;
    }
    else {
        // No existing free blocks satisfying the need, allocate a new one using sbrk syscall
        std::cout << "No existing free block found satifying the required size. Creating a new one." << std::endl;
        block = reinterpret_cast<Block*>(sbrk(sizeof(Block) + size));
        block->size = size;
        block->next = nullptr;
    }

    mtx.unlock();
    return reinterpret_cast<void*>(reinterpret_cast<char*>(block) + sizeof(Block));
}

// Function to deallocate memory
void _free(void* ptr) {
    if (!ptr)
        return;

    mtx.lock();

    // Get the block header from the pointer
    Block* block = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr) - sizeof(Block));

    // Insert the freed block back into the free list
    block->next = free_blocks_head;
    free_blocks_head = block;

    mtx.unlock();
}
