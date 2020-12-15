// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_
#include <atomic>
#include <cstddef>
#include <iostream>
#include <stdexcept>
template <typename T>
struct ControlBlock {
  explicit ControlBlock(T* ptr)
      :point(ptr),
        count(1){}
  void AddPointer(){
    ++count;
  }
  size_t GetCount(){
    return count;
  }
  void SubPointer(){
    --count;
  }
 public:
 private:
  T* point;
  std::atomic<size_t> count;

};

template <typename T>
class SharedPtr {
 public:
  SharedPtr()
      :objectPtr(nullptr),
        blockPtr(nullptr){};
  explicit SharedPtr(T* ptr)
      :objectPtr(ptr),
        blockPtr(new ControlBlock<T>(ptr)){};
  SharedPtr(const SharedPtr& r){
    if (std::is_move_constructible<T>::value){
      objectPtr = r.objectPtr;
      blockPtr = r.blockPtr;
      blockPtr->AddPointer();
    }else{
      throw std::runtime_error("Type not move constructible");
    }
  };
  SharedPtr(SharedPtr&& r){
        if(std::is_move_assignable<T>::value){
          objectPtr = r.objectPtr;
          blockPtr = r.blockPtr;
          r.blockPtr = nullptr;
          r.objectPtr = nullptr;
        }else{
          throw std::runtime_error("Type not move assignable");
        }
  };
   ~SharedPtr(){
    if (blockPtr!= nullptr) {
       blockPtr->SubPointer();
       if(!blockPtr->GetCount())
         delete blockPtr;
     }
  };
  auto operator=(const SharedPtr& r) -> SharedPtr&{
    if(&r == this){
      std::cout << "Equal objects" << std::endl;
    }
    if(std::is_move_constructible<T>::value && &r != this){
      blockPtr->SubPointer();
      objectPtr = r.objectPtr;
      blockPtr = r.blockPtr;
      blockPtr->AddPointer();
    } else {
        throw std::runtime_error("Type not move constructible");
      }
      return *this;
  };
  auto operator=(SharedPtr&& r) -> SharedPtr&{
      if(&r == this){
        std::cout << "Equal objects" << std::endl;
      }
      if(std::is_move_assignable<T>::value){
        reset();
        objectPtr = r.objectPtr;
        blockPtr = r.blockPtr;
        r.blockPtr = nullptr;
        r.objectPtr = nullptr;
      } else {
        throw std::runtime_error("Type not move assignable");
      }
      return *this;
  };

  operator bool() const{
      return (objectPtr != nullptr);
  };
  auto operator*() const -> T&{
      return *objectPtr;
  };
  auto operator->() const -> T*{
      return objectPtr;
  };
  auto get() -> T*{
      return objectPtr;
  };
  void reset(){
      objectPtr = nullptr;
      blockPtr -> SubPointer();
      if (!blockPtr->GetCount())
        delete blockPtr;
      blockPtr = nullptr;
  };
  void reset(T* ptr){
    reset();
    objectPtr = ptr;
    blockPtr = new ControlBlock<T>(ptr);

  };
  void swap(SharedPtr& r){
      std::swap(objectPtr, r.objectPtr);
      std::swap(blockPtr, r.blockPtr);
  };
  auto use_count() const -> std::size_t{
      if(objectPtr != nullptr){
        return blockPtr->GetCount();
      }
      return 0;
  };

 private:
  T* objectPtr;
  ControlBlock<T>* blockPtr;
};
#endif // INCLUDE_SHAREDPTR_HPP_
