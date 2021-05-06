#pragma once

struct ownerBlock;
struct genericMemoryBlock
{
  ownerBlock* owner;
};

template<typename T>
struct memoryBlock : public genericMemoryBlock
{
  char chunk[sizeof(T)];
};

