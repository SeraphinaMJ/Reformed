#include "Component.hpp"
#include "ComponentRepository.hpp"
#include <algorithm>
#include <Logger.hpp>
#include <IComponentMemoryClasses.hpp>

#undef max
/*
Implementations:
   - No overhead allocator
   - Page based allocator
   - Component-type-based contiguous allocator

side thing to do: stack allocator
*/
#define DEFAULT_PAGE_SIZE 2048u

#if MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
// Each actual block gets a header block

/*
header block points to the memory block that manages you



header | memory | header | memory
|                /
|               /
|              /
|             /
|            /
|           /
|          /
|         /
ownerBlock

*/

// header for a memory block
class componentRepositoryPage;
struct ownerBlock;
class componentRepositoryPageManager;
// pool of free pages to grab for any manager
static std::vector<componentRepositoryPage*> pagePool;
static std::vector<componentRepository*> repositoryList;
// pool of free owner-blocks to grab for any manager
std::vector<ownerBlock*> blockPool;
ownerBlock* newBlock(unsigned blockSize,
  unsigned blockCount,
  char* dataStart,
  char* dataEnd);
componentRepositoryPage* newPage(unsigned pageSize, unsigned chunkSize,
                                 componentRepositoryPageManager* owner);


struct ownerBlock
{
  	unsigned blockSize;
		unsigned blockCount = 1;
		char* dataStart;
		char* dataEnd;
    // only used if in free list
    ownerBlock* nextBlock = 0;
    ownerBlock* prevBlock = 0;
    componentRepositoryPage* page;
};

static int getStartToChunkOffset(componentRepositoryPageManager* ptr);

class componentRepositoryPage
{
public:
  componentRepositoryPage(unsigned pageSize_,
                          unsigned blockSize_,
                          componentRepositoryPageManager* owner_) : blockSize(blockSize_),
                                                                     pageSize(pageSize_)
  {
    owner = owner_;
    internalMemory = reinterpret_cast<char*>(malloc(pageSize));
    freeList = newBlock(blockSize_,
      pageSize / blockSize_,
      internalMemory,
      internalMemory + blockSize_ * pageSize / blockSize_);
    freeList->page = this;
    nextPage = 0;
    prevPage = 0;
    // set up the allocated block - only need the page pointer
    allocatedBlock.page = this;
  }
  void initializePage(unsigned pageSize_, unsigned blockSize_, componentRepositoryPageManager* owner_)
  {
    owner = owner_;
    pageSize = pageSize_;
    blockSize = blockSize_;
    internalMemory = reinterpret_cast<char*>(malloc(pageSize));
    freeList = newBlock(blockSize_,
      pageSize / blockSize_,
      internalMemory,
      internalMemory + blockSize_ * pageSize / blockSize_);
    freeList->page = this;
    nextPage = 0;
    prevPage = 0;
  }
  ~componentRepositoryPage()
  {
    cleanPage();
  }
  void cleanPage()
  {
    // this should only get called if the page is entirely empty (in which case we should have
    // one merged megablock and this should be O(1) and be very fast)
    // or at shutdown (and it might take a bit then to do all of them)
    // returns all ownerBlocks to the block pool
    ownerBlock* curBlock = freeList;
    while (curBlock != nullptr)
    {
      blockPool.push_back(curBlock);
      curBlock = curBlock->nextBlock;
    }
    freeList = 0;
    free(internalMemory);
    internalMemory = 0;
    owner = 0;
  }
  void* allocate()
  {
    // TODO(coseo) implement this
    // this should only be getting called if it is available for allocations
    // grab first block off the free list
    ownerBlock* allocationBlock = freeList;
    if (allocationBlock->blockCount == 1)
    {
      reinterpret_cast<genericMemoryBlock*>(allocationBlock->dataStart)->owner = &allocatedBlock;
      freeList = freeList->nextBlock;
      if (freeList)
        freeList->prevBlock = nullptr;
      blockPool.push_back(allocationBlock);
      // returns pointer to the giant memory chunk
      return reinterpret_cast<char*>(allocationBlock->dataStart) + getStartToChunkOffset(owner);
    }
    else
    {
      // break off a chunkF
      char* brokenChunk = freeList->dataEnd - blockSize;
      freeList->dataEnd = brokenChunk;
      freeList->blockCount--;
      reinterpret_cast<genericMemoryBlock*>(brokenChunk)->owner = &allocatedBlock;
      return (brokenChunk + getStartToChunkOffset(owner));
    }
  }

  void returnBlock(genericMemoryBlock* block)
  {
    // TODO(coseo) merge adjacent expects both blocks to have an owner,
    // so you should perform the "owner assignment" before the merge operation
    // otherwise merging will never work

    ownerBlock* newOwner =
      newBlock(blockSize,
               1,
               reinterpret_cast<char*>(block),
               reinterpret_cast<char*>(block) + blockSize);
    // inserting at front of freeList
    newOwner->nextBlock = freeList;
    if (freeList)
      freeList->prevBlock = newOwner;
    freeList = newOwner;
    newOwner->prevBlock = 0;
    block->owner = newOwner;
    newOwner->page = this;

    // steps:
    // try merging left
    bool mergeSuccessful = false;
    if (reinterpret_cast<char*>(block) != internalMemory)
    {
      genericMemoryBlock* prevBlock = reinterpret_cast<genericMemoryBlock*>(
        reinterpret_cast<char*>(block) - blockSize);
      // if we merge successfully, we done
      mergeSuccessful = (mergeAdjacentBlocks(reinterpret_cast<genericMemoryBlock*>(
        reinterpret_cast<char*>(block) - blockSize)->owner,
        block->owner));
    }
    // try merging right - this might not successfully merge at the end of the array,
    // but that won't break anything, and thats a tiny edge case
    if (reinterpret_cast<char*>(block) + blockSize < internalMemory + pageSize)
    {
      // if we merge successfully, we done
      mergeSuccessful = mergeSuccessful |
        (mergeAdjacentBlocks(block->owner, reinterpret_cast<genericMemoryBlock*>(
          reinterpret_cast<char*>(block) + blockSize)->owner
          ));
    }
  }

  ownerBlock* defaultblock()
  {
    return &allocatedBlock;
  }
  bool isFull()
  {
    if (freeList == 0)
    {
      return true;
    }
    return false;
  }
  componentRepositoryPage* nextPage = 0;
  componentRepositoryPage* prevPage = 0;
  bool mergeAdjacentBlocks(ownerBlock* b1, ownerBlock* b2);
private:
  int blockSize;
  // free list
  ownerBlock* freeList = 0;
  // pointer to start of internal memory
  char* internalMemory;
  // size of the page
  int pageSize = 0;
  componentRepositoryPageManager* owner;
  // block that represents an allocated piece
  ownerBlock allocatedBlock;
};


class componentRepositoryPageManager
{
public:
  componentRepositoryPageManager(int componentOffsetFromChunkStart,
                                 int componentBlockSize_,
                                 int baseToMostDerived_,
                                 unsigned reservedAmount = 0);
  void* allocate()
  {
    if (usablePages == nullptr)
    {
      currentPageSize = currentPageSize * 2;
      usablePages = newPage(currentPageSize, chunkSize, this);
    }
    void* result = usablePages->allocate();
    // if that filled it, move it over
    if (usablePages->isFull())
    {
      componentRepositoryPage* movedPage = usablePages;
      usablePages = movedPage->nextPage;
      if (usablePages)
        usablePages->prevPage = 0;
      movedPage->nextPage = fullPages;
      if (fullPages)
        fullPages->prevPage = movedPage;
      movedPage->prevPage = 0;
      fullPages = movedPage;
    }
    ++blocksInUse;
    return result;
  }
  void returnChunk(void* componentBase)
  {
    // this *will* cause heap corruption if allowed to occur.
    if (!baseToMostDerivedEstablished)
    {
      std::abort();
    }
    void* chunk = reinterpret_cast<char*>(componentBase) + baseToMostDerived;
    genericMemoryBlock* blockHeader =
      reinterpret_cast<genericMemoryBlock*>(reinterpret_cast<char*>(chunk) + offsetFromChunkToStart);
    componentRepositoryPage* alleviatedPage;
    alleviatedPage = blockHeader->owner->page;
    bool wasFull = false;
    if (alleviatedPage->isFull())
    {
      wasFull = true;
    }
    // since we're allocated all we can use right now is the page
    alleviatedPage->returnBlock(blockHeader);
    if (wasFull)
    {
      if (alleviatedPage->prevPage)
        alleviatedPage->prevPage->nextPage = alleviatedPage->nextPage;
      if (alleviatedPage->nextPage)
        alleviatedPage->nextPage->prevPage = alleviatedPage->prevPage;
      if (fullPages == alleviatedPage)
        fullPages = alleviatedPage->nextPage;

      alleviatedPage->prevPage = 0;
      if (usablePages)
        usablePages->prevPage = alleviatedPage;
      alleviatedPage->nextPage = usablePages;
      usablePages = alleviatedPage;
    }
    --blocksInUse;
  }
  int getStartToChunkOffset()
  {
    return -1 * offsetFromChunkToStart;
  }

  ~componentRepositoryPageManager()
  {
    // TODO(coseo) implement cleanup here that gets all pages cleaned up or moved
    // to the page pool
    componentRepositoryPage* curPage = fullPages;
    while (curPage != nullptr)
    {
      componentRepositoryPage* nextPage = curPage->nextPage;
      curPage->cleanPage();
      pagePool.push_back(curPage);
      curPage = nextPage;
    }
    curPage = usablePages;
    while (curPage != nullptr)
    {
      componentRepositoryPage* nextPage = curPage->nextPage;
      curPage->cleanPage();
      pagePool.push_back(curPage);
      curPage = nextPage;
    }
    fullPages = 0;
    usablePages = 0;
  }

  unsigned getAllocatedCount()
  {
    return blocksInUse;
  }
  unsigned getChunkSize()
  {
    return chunkSize;
  }
  int getComponentID()
  {
    return componentID;
  }
  void setComponentID(int id)
  {
    componentID = id;
  }

  void setBaseToMostDerivedOffset(int offset)
  {
    baseToMostDerivedEstablished = true;
    baseToMostDerived = offset;
  }

  bool baseToMostDerivedOffsetEstablished()
  {
    return baseToMostDerivedEstablished;
  }

private:
  unsigned currentPageSize;
  // vector of pages - this is a self-managed max-heap sorted by amount free
  componentRepositoryPage* fullPages = 0;
  componentRepositoryPage* usablePages = 0;
  unsigned totalData = 0; // total amount of memory in use
  unsigned blocksInUse = 0;
  unsigned chunkSize = 0; //
  int offsetFromChunkToStart = 0;
  int baseToMostDerived = -1;
  bool baseToMostDerivedEstablished = false;
  int componentID = 0;

};

static int getStartToChunkOffset(componentRepositoryPageManager* ptr)
{
  return ptr->getStartToChunkOffset();
}

// TODO(coseo) Cache alignment?
componentRepositoryPageManager::componentRepositoryPageManager(int componentOffsetFromChunkStart,
                                 int componentBlockSize_,
                                 int baseToMostDerived_,
                                 unsigned reservedAmount) :
  chunkSize(componentBlockSize_)
{
  	// page size is max of: default page size, chunk size,  reserved amount
	currentPageSize = std::max(DEFAULT_PAGE_SIZE, 10 * chunkSize);
  currentPageSize = std::max(reservedAmount, currentPageSize);
	// make sure it can be evenly divided into chunks
	unsigned remainder = currentPageSize % chunkSize;
	if (remainder != 0)
		currentPageSize += chunkSize - remainder;
  if (reservedAmount != 0)
  {
    usablePages = newPage(chunkSize, reservedAmount, this);
  }

  offsetFromChunkToStart = componentOffsetFromChunkStart;
}




ownerBlock* newBlock(unsigned blockSize,
              unsigned blockCount,
              char* dataStart,
              char* dataEnd)
{
  ownerBlock* resultBlock;
  if (blockPool.size() != 0)
  {
    resultBlock = blockPool.back();
    blockPool.pop_back();
  }
  else
  {
    resultBlock = new ownerBlock;
  }

  // set up owner pointers
  for (unsigned i = 0; i < blockCount; ++i)
  {
    reinterpret_cast<genericMemoryBlock*>(dataStart + i * blockSize)->owner = resultBlock;
  }

  resultBlock->blockSize = blockSize;
  resultBlock->blockCount = blockCount;
  resultBlock->dataStart = dataStart;
  resultBlock->dataEnd = dataEnd;
  resultBlock->nextBlock = 0;
  resultBlock->prevBlock = 0;
  resultBlock->page = 0;

  return resultBlock;
}

componentRepositoryPage* newPage(unsigned pageSize, unsigned chunkSize, componentRepositoryPageManager* owner)
{
  if (pagePool.size() == 0)
  {
    return new componentRepositoryPage(pageSize, chunkSize, owner);
  }
  else
  {
    componentRepositoryPage* page = pagePool.back();
    pagePool.pop_back();
    page->initializePage(chunkSize, chunkSize, owner);
    return page;
  }
}

// cleans up excess blocks
void clearBlockPool()
{
  for (unsigned i = 0; i < blockPool.size(); ++i)
  {
    delete blockPool[i];
  }
  blockPool.clear();
}

// cleans up excess pages
void clearPagePool()
{
// TODO(coseo) remove the following bandaid code
// there are duplicate pointers in the page pool....
  std::vector<componentRepositoryPage*> pagesSeen;

  for (unsigned i = 0; i < pagePool.size(); ++i)
  {
	  bool isDuplicate = false;
	for (unsigned j = 0; j < pagesSeen.size(); ++j)
	{
	  if (pagesSeen[j] == pagePool[i])
	  {
	    isDuplicate = true;
	    break;
	  }
	}
	if (isDuplicate)
	  continue;
	else
	{
	  delete pagePool[i];
	  pagesSeen.push_back(pagePool[i]);
	}
  }
  pagePool.clear();
}

// assumes b2 starts at the end of b1
bool componentRepositoryPage::mergeAdjacentBlocks(ownerBlock* b1, ownerBlock* b2)
{
  if (b1 != b1->page->defaultblock() &&
    b2 != b2->page->defaultblock())
  {
    ownerBlock* blockToRemove;
    ownerBlock* replacement;
    if (b1->blockCount < b2->blockCount)
    {
      blockToRemove = b1;
      replacement = b2;
      b2->dataStart = b1->dataStart;
      b2->blockCount += b1->blockCount;
    }
    else
    {
      blockToRemove = b2;
      replacement = b1;
      b1->blockCount += b2->blockCount;
      b1->dataEnd = b2->dataEnd;
    }

    for (unsigned i = 0; i < blockToRemove->blockCount; ++i)
    {
      reinterpret_cast<genericMemoryBlock*>(blockToRemove->dataStart +
        i * blockToRemove->blockSize)->owner = replacement;
    }

    // remove b2 from the free list
    if (blockToRemove->nextBlock)
    {
      blockToRemove->nextBlock->prevBlock = blockToRemove->prevBlock;
    }

    if (blockToRemove->prevBlock)
    {
      blockToRemove->prevBlock->nextBlock = blockToRemove->nextBlock;
    }
    if (freeList == blockToRemove)
    {
      freeList = blockToRemove->nextBlock;
    }
    blockPool.push_back(blockToRemove);

    return true;
  }
  // otherwise couldnt merge
  return false;
}

void componentRepository::initializeMemoryManagers()
{
  // for each component ID, create a memory manager for that component type
  // make sure the vector is big enough
  // make sure its clear, then fill it with null pointers for each component id
  managers.clear();
  for (int i = 0; i < component::getComponentTypes(); ++i)
  {
    managers.push_back(0);
  }
  // for each id, replace the nullptr with an actual entry
  for (int i = 0; i < component::getComponentTypes(); ++i)
  {
    // for each component type....
    auto& componentSizeMap = component::getComponentSizeMap();

    for (auto entry : componentSizeMap)
    {
      int componentID = entry.first;
      int componentSize = entry.second;
      int componentChunkOffset = component::componentMemoryBlockOffsets()[componentID];
      int componentBaseToMostDerived = -1;
      int componentMemoryBlockSize = component::getComponentMemoryBlockSizeMap()[componentID];
      managers[componentID] = new componentRepositoryPageManager(
        componentChunkOffset,
        componentMemoryBlockSize,
        componentBaseToMostDerived);
      managers[componentID]->setComponentID(componentID);
    }
  }
}

void componentRepository::getMemoryDebugData(std::vector<memoryDebugData>& vectorToFill)
{
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
  std::vector<int> top5Indices;
  for (unsigned i = 0; i < managers.size(); ++i)
  {
    if (top5Indices.size() < 5)
    {
      top5Indices.push_back(i);
    }
    else
    {
      auto& curManager = managers[i];
      unsigned chunkSize = curManager->getChunkSize();
      unsigned allocCount = curManager->getAllocatedCount();
      unsigned memoryInUse = chunkSize * allocCount;

      unsigned worstOfTop = 0;
      unsigned worstScore = managers[top5Indices[0]]->getChunkSize() * 
                            managers[top5Indices[0]]->getAllocatedCount();
      for (unsigned j = 1; j < top5Indices.size(); ++j)
      {
        auto& comparisonManager = managers[top5Indices[j]];
        unsigned comparisonChunkSize = comparisonManager->getChunkSize();
        unsigned comparisonAllocCount = comparisonManager->getAllocatedCount();
        unsigned comparisonMemoryInUse = comparisonChunkSize * comparisonAllocCount;
        if (comparisonMemoryInUse < worstScore)
        {
          worstScore = comparisonMemoryInUse;
          worstOfTop = j;
        }
      }
      if (memoryInUse > worstScore)
      {
        top5Indices[worstOfTop] = top5Indices.back();
        top5Indices[top5Indices.size() - 1] = i;
      }
    }
  }
  // try sorting... this is bubble sort
  bool sorted = false;
  while (!sorted)
  {
    sorted = true;
    for (unsigned i = 0; i < top5Indices.size() - 1; ++i)
    {
      auto& curManager = managers[top5Indices[i]];
      unsigned curChunkSize = curManager->getChunkSize();
      unsigned curAllocCount = curManager->getAllocatedCount();
      unsigned curMemoryInUse = curChunkSize * curAllocCount;

      auto& swapManager = managers[top5Indices[i+1]];
      unsigned swapChunkSize = swapManager->getChunkSize();
      unsigned swapAllocCount = swapManager->getAllocatedCount();
      unsigned swapMemoryInUse = swapChunkSize * swapAllocCount;
      // if the memory use for the index we are looking at is less than the next - we need to swap
      // the two indices 
      if (curMemoryInUse < swapMemoryInUse)
      {
        int tempIndex = top5Indices[i];
        top5Indices[i] = top5Indices[i + 1];
        top5Indices[i + 1] = tempIndex;
      }
    }
    for (unsigned i = 0; i < top5Indices.size() - 1; ++i)
    {
      auto& curManager = managers[top5Indices[i]];
      unsigned curChunkSize = curManager->getChunkSize();
      unsigned curAllocCount = curManager->getAllocatedCount();
      unsigned curMemoryInUse = curChunkSize * curAllocCount;

      auto& swapManager = managers[top5Indices[i+1]];
      unsigned swapChunkSize = swapManager->getChunkSize();
      unsigned swapAllocCount = swapManager->getAllocatedCount();
      unsigned swapMemoryInUse = swapChunkSize * swapAllocCount;
      // if the memory use for the index we are looking at is less than the next - we need to swap
      // the two indices 
      if (curMemoryInUse < swapMemoryInUse)
      {
        sorted = false;
        break;
      }
    }
  }
  // now sorted greatest to least
  // push them into the vector
  for (unsigned i = 0; i < top5Indices.size(); ++i)
  {
    auto& curManager = managers[i];
    unsigned chunkSize = curManager->getChunkSize();
    unsigned allocCount = curManager->getAllocatedCount();
    unsigned memoryInUse = chunkSize * allocCount;
    memoryDebugData newData;
    std::string componentName = component::getComponentNameMap()[curManager->getComponentID()];
    newData.name = componentName;
    newData.memoryUsed = memoryInUse;
    newData.allocationCount = allocCount;
    vectorToFill.push_back(newData);
  }
#endif
}


void componentRepository::freeComponentInternal(
	component* comp
 #if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
  ,
	const char * file,
	const char * func,
	int line
#endif
  )
{

  componentRepositoryPageManager* manager = managers[comp->type().getComponentID()];
	// TODO(coseo) enable actual memory management, debug logging
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
  logger tempLogger("ComponentRepository");
  tempLogger.debug() << "Deallocation Requested for component \"" <<
      component::getComponentNameMap()[comp->type().getComponentID()] << "\"";
  tempLogger.debug() << "  Current allocation count: " << manager->getAllocatedCount() << "";
  tempLogger.debug() << "  Address being freed: "
                    << reinterpret_cast<std::intptr_t>(dynamic_cast<void*>(comp)) << "";
#endif
	// Gee I sure hope you wrote a virtual destructor
  comp->~component();
  manager->returnChunk(comp);
}

void* componentRepository::createComponentInternal(unsigned componentID,
  size_t size
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
  ,
  const char * file,
  const char * func,
  int line
#endif
)
{
  componentRepositoryPageManager* manager = managers[componentID];
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    logger tempLogger("ComponentRepository");
    tempLogger.debug() << "Allocation Requested for component \"" <<
      component::getComponentNameMap()[componentID] << "\"";
    tempLogger.debug() << "  Current allocation count: " << manager->getAllocatedCount() << "";
#endif
  void* result = manager->allocate();
#if MEMORY_DEBUG_INFORMATION_COLLECTED
  tempLogger.debug() << "    Allocation successful, returning pointer to address "
      << reinterpret_cast<std::intptr_t>(result) << "";
#endif
  return result;
}

componentRepository::~componentRepository()
{
  for (unsigned i = 0; i < managers.size(); ++i)
  {
    delete managers[i];
    managers[i] = nullptr;
  }
}

// shuts down all memory managers
void shutdownAllManagers()
{
  for (unsigned i = 0; i < repositoryList.size(); ++i)
  {
    // this should clean up repositories, which should in turn
    // clean up or return all pages they own
    delete repositoryList[i];
  }
  repositoryList.clear();
  // clean up pages first - some may linger, others were pushed here when their repositories
  // cleaned up
  clearPagePool();
  // now clean up blocks - cleaning up pages should push a lot in here
  clearBlockPool();
}

componentRepository* newRepository()
{
  componentRepository* repo = new componentRepository();
  repositoryList.push_back(repo);
  return repo;
}

void componentRepository::establishOffset(int id, int offset)
{
  managers[id]->setBaseToMostDerivedOffset(offset);
}
bool componentRepository::offsetIsEstablished(int id)
{
  return managers[id]->baseToMostDerivedOffsetEstablished();
}

#elif MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
componentRepository::~componentRepository()
{
	// TODO(coseo) clean up memory management here
}

blockHandle* componentRepository::createComponentInternal(unsigned componentID,
														  size_t size,
														  const char * file,
														  const char * func,
														  int line)
{
	// TODO(coseo) enable actual memory management, debug logging
	void* new_component = malloc(size);
	blockHandle* handle = new blockHandle;
  // its not actually the component pointer but that will be fixed at the next level up
	handle->data = reinterpret_cast<component*>(new_component);
	handle->page = 0;
	handle->refCount = 0;
	return handle;
}

void componentRepository::freeComponentInternal(
	component* comp,
	blockHandle* handle,
	const char * file,
	const char * func,
	int line)
{
	// Gee I sure hope you wrote a virtual destructor
	comp->~component();
	// TODO(coseo) enable actual memory management, debug logging
	free(handle->data);
	handle->data = nullptr;
	handle->page = nullptr;
}

void componentRepository::returnBlockHandle(blockHandle* handle)
{
	// TODO(coseo) manage memory
	delete handle;
}

void baseComponentHandle::cleanup()
{
	if (bHandle != nullptr && owner != nullptr)
	{
		if (--bHandle->refCount <= 0)
		{
			// clean up memory internally
			owner->returnBlockHandle(bHandle);
		}
	}
}
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD

#endif


