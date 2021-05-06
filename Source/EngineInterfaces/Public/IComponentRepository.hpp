#pragma once
#include <cstddef>
#include <IComponentMemoryClasses.hpp>
#include <Component.hpp>

// flags - set these to 0 if you want 
#define MEMORY_DEBUG_INFORMATION_COLLECTED 1

#define MEMORY_MODE_NO_OVERHEAD 0
#define MEMORY_MODE_POOL_BASED_ALLOCATOR 1
#define MEMORY_MODE_CONTIGUOUS_ALLOCATOR 2

// This is the memory mode we will actually use 
#define MEMORY_MODE MEMORY_MODE_POOL_BASED_ALLOCATOR

/*
Implementations:
   - No overhead allocator
   - Page based allocator
   - Component-type-based contiguous allocator
*/

// TODO(coseo): 
// implement remove
// which uses the contiguous memory handling functionality 
// (or the reference-tracking-type-stuff you get from pool components
// if necessary)
// in order to clean up

class component;
class componentType;
#define STRINGIFY(x) #x

#define ALLOCATE(repositoryPtr, type) \
	repositoryPtr->createComponent<type>(__FILE__, __func__, __LINE__)
#define DEALLOCATE(repositoryPtr, handle) \
	repositoryPtr->freeComponent(handle, __FILE__, __func__, __LINE__)


#if MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR

// could probably remove page pointer?
struct blockHandle
{
	//!< pointer to internal data block
	component* data = 0;
	//!< count of references to this handler - clean up when it reaches 0
	int refCount = 0;
	//!< page that holds our block
	void* page = 0;
};

class icomponentRepository;

// cleanup implementation is in ComponentRepository.cpp
// TODO(coseo) remove owner handle, we don't use it
class baseComponentHandle
{
public:
	baseComponentHandle() {
		bHandle = nullptr;
		owner = nullptr;
		offset = 0;
	}
  baseComponentHandle(void* internalPtr)
  {
 		bHandle = nullptr;
	  owner = nullptr;
		offset = 0;
  }
	//!< publicly visible constructors to build component handles
	baseComponentHandle(blockHandle* handle_,
		icomponentRepository* owner_) : owner(owner_),
		bHandle(handle_),
		offset(0)
	{
		if (bHandle)
		{
			++bHandle->refCount;
		}
	}
	baseComponentHandle(const baseComponentHandle& other) : owner(other.owner),
		bHandle(other.bHandle),
		offset(other.offset)
	{
		if (bHandle) {
			++bHandle->refCount;
		}
	}


	// Doesn't really need to be virtual. have no extra vars
	virtual ~baseComponentHandle()
	{
		cleanup();
	};
	component* operator->()
	{
		return getBaseComponent();
	}

	// doesn't need to be virtual because all deriving classes have no extra vars
	bool operator==(const baseComponentHandle& other) const
	{
		if (owner == other.owner && bHandle == other.bHandle)
		{
			return true;
		}
		return false;
	}

	// doesn't need to be virtual because deriving classes have no extra vars
	baseComponentHandle& operator=(const baseComponentHandle& other)
	{
		cleanup();
		owner = other.owner;
		bHandle = other.bHandle;
		if (bHandle)
			++bHandle->refCount;
		offset = other.offset;
		return *this;
	}
	operator bool() const
	{
		return (bHandle != nullptr && bHandle->data != nullptr);
	}
protected:
	// repository that owns us
	icomponentRepository* owner;
	blockHandle* bHandle;
	size_t offset = 0;

	// this is necessary to allow the templated inheritors to call the base class's
	// protected function on their templated friends
	static component* getBaseComponentFromOtherStatic(const baseComponentHandle& other) {
		return other.getBaseComponent();
	}
	static icomponentRepository* getOwnerFromOtherStatic(const baseComponentHandle& other) {
		return other.owner;
	}
	static blockHandle* getBhandleFromOtherStatic(const baseComponentHandle& other) {
		return other.bHandle;
	}
  virtual component* getBaseComponent() const
  {
    if (bHandle)
    {
      return bHandle->data;
    }
    return nullptr;
  }
  virtual void remove()
  {
    // if this is called you have a problem
  }
	void cleanup();
	friend class icomponentRepository;
};

template<typename T>
class componentHandle : public baseComponentHandle
{
public:
	//!< default constructor is null
	componentHandle() {}
	//!< publicly visible constructors to build component handles
	componentHandle(blockHandle* handle_,
					icomponentRepository* owner_) : baseComponentHandle(handle_, owner_)
	{}

	static componentHandle<T> castHandle(const baseComponentHandle& handle)
	{
		// component pointer is a pointer to the type that they want

		// case: component pointer does not point to null
		componentHandle<T> result;
		component* ptr = result.getBaseComponentFromOther(handle);
		result.owner = result.getOwnerFromOther(handle);
		result.bHandle = result.getHandleFromOther(handle);
		if (ptr)
		{
			T* cast_result = dynamic_cast<T*>(ptr);
			if (cast_result != nullptr)
			{
				result.offset = reinterpret_cast<char*>(cast_result) -
					reinterpret_cast<char*>(result.bHandle->data);
				++result.bHandle->refCount;
				return result;
			}
		}
		// if it was at all malformed, throw out the data and return no offset
		result.offset = 0;
		result.bHandle = nullptr;
		return result;
	}

	componentHandle(const componentHandle<T>& other) : baseComponentHandle(other)
	{}

	// TODO(coseo) is this slow?
	T* operator->()
	{
		return reinterpret_cast<T*>(reinterpret_cast<char*>(bHandle->data) + offset);
	}

  void remove() override
  {
    // TODO: Implement this function
    // should handle the idea of "removing a component from a game object"
    // sort of like freeing 
  }
private:
	friend class icomponentRepository;
	virtual component* getBaseComponent() const override
	{
		if (bHandle)
		{
      return bHandle->data;
		}
		return nullptr;
	}
	// These functions are unfortunately necessary in oder to implement the casting
	blockHandle* getHandleFromOther(const baseComponentHandle& other)
	{
		return baseComponentHandle::getBhandleFromOtherStatic(other);
	}
	icomponentRepository* getOwnerFromOther(const baseComponentHandle& other)
	{
		return baseComponentHandle::getOwnerFromOtherStatic(other);
	}
	component* getBaseComponentFromOther(const baseComponentHandle& other)
	{
		return baseComponentHandle::getBaseComponentFromOtherStatic(other);
	}

	template <typename U>
	friend class componentHandle;
  template <typename U>
  friend componentHandle<U> castHandle(const baseComponentHandle& handle);
};


#elif MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR

class component;
using baseComponentHandle = component * ;

template<typename T>
using componentHandle = T * ;

#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD
class component;
using baseComponentHandle = component * ;

template<typename T>
using componentHandle = T * ;

#endif

class icomponentRepository
{
public:
	template<typename T>
	componentHandle<T> createComponent(const char * file,
									   const char * func,
							   		 int line)
	{
		componentHandle<T> result;
#if MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
    int id = componentRegistrar<T>::getID();
    if (!offsetIsEstablished(id))
    {
      int offset = reinterpret_cast<char*>(reinterpret_cast<T*>(0)) -
                   reinterpret_cast<char*>(static_cast<component*>((reinterpret_cast<T*>(0))));
      establishOffset(id, offset);
    }
    result = reinterpret_cast<componentHandle<T>>(createComponentInternal(componentRegistrar<T>::getID(), sizeof(T)
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , 
      file, 
      func, 
      line
#endif
    ));
    result = new(result) T;
#elif MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
		// TODO(coseo) integrate with CRTP
		blockHandle* raw_ptr = createComponentInternal(componentCRTP<T>::getStaticID(), sizeof(T), file, func, line);
		// Placement new to format memory
		raw_ptr->data = dynamic_cast<component*>((new (raw_ptr->data) T()));
		result.bHandle = raw_ptr;
    result.offset = reinterpret_cast<char*>(dynamic_cast<T*>(raw_ptr->data)) - 
                    reinterpret_cast<char*>(raw_ptr->data);
		raw_ptr->refCount++;
		result.owner = this;
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD
    // no overhead - we just allocate
    // TODO: record that an allocation happened 
    result = new T;
#endif
		return result;
	}
#if MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
  virtual void establishOffset(int id, int offset) = 0;
  virtual bool offsetIsEstablished(int id) = 0;
#endif
	virtual ~icomponentRepository() {}

	void freeComponent(baseComponentHandle handle,
		const char * file,
		const char * func,
		int line)
	{
#if MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
		freeComponentInternal(handle.getBaseComponent(), handle.bHandle, file, func, line);
#elif MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
    freeComponentInternal(handle
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
     , file, func, line
#endif
    );
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD
    delete handle;
    // TODO: record that a free just happened ?
#endif
	}

  struct memoryDebugData
  {
  public:
    std::string name;
    unsigned allocationCount;
    unsigned memoryUsed;
  };
  virtual void getMemoryDebugData(std::vector<memoryDebugData>& vectorToFill) = 0;

protected:

#if MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
	virtual void freeComponentInternal(component* component,
		blockHandle* handle
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , const char * file, const char * func, int line
#endif
  ) = 0;
	virtual blockHandle* createComponentInternal(unsigned componentID,
												 size_t size
  #if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , const char * file, const char * func, int line
#endif
  ) = 0;
	virtual void returnBlockHandle(blockHandle* handle) = 0;
#elif MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
  	virtual void freeComponentInternal(component* component
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , const char * file, const char * func, int line
#endif
    ) = 0;
	virtual void* createComponentInternal(unsigned componentID,
												 size_t size
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , const char * file, const char * func, int line
#endif
  ) = 0;
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD
  	virtual void freeComponentInternal(component* component
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , const char * file, const char * func, int line
#endif
    ) = 0;
	virtual void* createComponentInternal(
                         size_t size
#if MEMORY_DEBUG_INFORMATION_COLLECTED == 1
    , const char * file, const char * func, int line
#endif
  ) = 0;
#endif
#if MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
	template <typename U>
	friend class componentHandle;
	friend class baseComponentHandle;
#endif

};

// global cast handle 
template <typename T>
componentHandle<T> castHandle(const baseComponentHandle& handle)
{
#if MEMORY_MODE == MEMORY_MODE_POOL_BASED_ALLOCATOR
	// component pointer is a pointer to the type that they want
	// case: component pointer does not point to null
	componentHandle<T> result = dynamic_cast<T*>(handle);
	return result;
#elif MEMORY_MODE == MEMORY_MODE_CONTIGUOUS_ALLOCATOR
  	// component pointer is a pointer to the type that they want
	  // case: component pointer does not point to null
	  componentHandle<T> result;
	  component* ptr = result.getBaseComponentFromOther(handle);
	  result.owner = result.getOwnerFromOther(handle);
	  result.bHandle = result.getHandleFromOther(handle);
	  if (ptr)
	  {
		  T* cast_result = dynamic_cast<T*>(ptr);
		  if (cast_result != nullptr)
		  {
			  result.offset = reinterpret_cast<char*>(cast_result) -
				  reinterpret_cast<char*>(result.bHandle->data);
			  ++result.bHandle->refCount;
			  return result;
		  }
	  }
	  // if it was at all malformed, throw out the data and return no offset
	  result.offset = 0;
	  result.bHandle = nullptr;
	  return result;
#elif MEMORY_MODE == MEMORY_MODE_NO_OVERHEAD
  return static_cast<componentHandle<T>>(handle);
#endif 
}

template <typename T>
baseComponentHandle toBaseHandle(const componentHandle<T> original)
{
  return static_cast<baseComponentHandle>(original);
}



