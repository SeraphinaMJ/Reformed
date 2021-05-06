#pragma once

#include <IComponentRepository.hpp>
#include <unordered_map>
#include <vector>
/*!
track active system doing allocation too
overloads -> operator
private constructors, though a duplicate function
*/
class componentRepository;
void shutdownAllManagers();
componentRepository* newRepository();
// TODO(coseo) have a list of componenst who have not had their game object set yet - garbage collect them 
// if not set by cleanup time 
#if MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
class componentRepositoryPageManager;
class componentRepository : public icomponentRepository
{
public:
	virtual ~componentRepository() override;
  void initializeMemoryManagers();
  void getMemoryDebugData(std::vector<memoryDebugData>& vectorToFill) override;
  virtual void establishOffset(int id, int offset) override;
  virtual bool offsetIsEstablished(int id) override;
private:
	void* createComponentInternal(unsigned componentID,
										 size_t size
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    ,
										 const char * file,
										 const char * func,
										 int line
#endif
  ) override;
	void freeComponentInternal(
		component* component
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    ,
		const char * file,
		const char * func,
		int line
#endif
  ) override;
  std::vector<componentRepositoryPageManager*> managers;
};
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD

class componentRepository : public icomponentRepository
{
public:
  virtual ~componentRepository() override
  {
  }
private:
  void* createComponentInternal(size_t size
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    ,
    const char * file,
    const char * func,
    int line
#endif
  ) override
  {
    // TODO: debug info if enabled here ??
    return nullptr;
  }
  void freeComponentInternal
  (
    component* component
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    ,
    const char * file,
    const char * func,
    int line
#endif
  ) override
  {
    // TODO: debug info if enabled here?
  }
};
#elif MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
class componentRepository : public icomponentRepository
{
public:
	virtual ~componentRepository() override;
private:
	blockHandle* createComponentInternal(unsigned componentID,
										 size_t size,
										 const char * file,
										 const char * func,
										 int line) override;
	void freeComponentInternal(
		component* component,
		blockHandle* handle,
		const char * file,
		const char * func,
		int line) override;
	virtual void returnBlockHandle(blockHandle* handle) override;

};
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD

class componentRepository : public icomponentRepository
{
public:
  virtual ~componentRepository() override
  {
  }
private:
  void* createComponentInternal(size_t size
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    ,
    const char * file,
    const char * func,
    int line
#endif
  ) override
  {
    // TODO: debug info if enabled here ??
    return nullptr;
  }
  void freeComponentInternal(
    component* component
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    ,
    const char * file,
    const char * func,
    int line
#endif
  ) override
  {
    // TODO: debug info if enabled here?
  }
};

/* nothing goes here - no internal allocator ? */
#endif
