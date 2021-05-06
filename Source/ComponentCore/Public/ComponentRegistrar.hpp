#pragma once

#include <IComponentMemoryClasses.hpp>
#include <map>
class component;
class globalComponentRegistrar
{
  public:
    static int getComponentTypes() { return componentTypeCount;  }
		virtual int getTypeID() = 0;

    static std::map<int, size_t>& getComponentSizeMap()
    {
      static std::map<int, size_t> componentSizeMap;
      return componentSizeMap;
    }
    static std::map<int, std::string>& getComponentNameMap()
    {
      static std::map<int, std::string> componentNameMap;
      return componentNameMap;
    }

    static std::map<int, size_t>& getComponentMemoryBlockSizeMap()
    {
      static std::map<int, size_t> getComponentMemoryBlockSizeMap;
      return getComponentMemoryBlockSizeMap;
    }

    static std::map<int, int>& componentMemoryBlockOffsets()
    {
      static std::map<int, int> componentMemoryBlockOffsets;
      return componentMemoryBlockOffsets;
    }

    // NOTE: THIS CODE IS DEPRECATED AND NOT IN USE.
    // PLEASE DO NOT CALL THIS FUNCTION.
    static std::map<int, int> componentBaseToMostDerivedOffsetMap()
    {
      static std::map<int, int> componentBaseToMostDerivedOffsetMap;
      return componentBaseToMostDerivedOffsetMap;
    }
    template<typename T>
		static int registerComponent(std::string name) {
      // TODO(coseo) also include information for memory chunk size info
      int newID = componentTypeCount++;
      getComponentSizeMap()[newID] = sizeof(T);
      getComponentNameMap()[newID] = name;
      getComponentMemoryBlockSizeMap()[newID] = sizeof(memoryBlock<T>);
      componentMemoryBlockOffsets()[newID] =
        reinterpret_cast<char*>(static_cast<genericMemoryBlock*>(reinterpret_cast<memoryBlock<T>*>(0))) -
        reinterpret_cast<char*>(&(reinterpret_cast<memoryBlock<T>*>(0)->chunk));
      return newID;
    }
  private:
    // NOTE: This value is owned by (initialized/declared in)
    // Component.cpp
    // TODO: Move initialization into ComponentRegistrar.cpp
    static int componentTypeCount;
};

template <typename T>
class componentRegistrar
{
public:
  static int getID()
  {
    return componentRegistrarID;
  }
  private:
    static int componentRegistrarID;
};

template<typename T>
int componentRegistrar<T>::componentRegistrarID =   
    globalComponentRegistrar::registerComponent<T>(typeid(T).name());


