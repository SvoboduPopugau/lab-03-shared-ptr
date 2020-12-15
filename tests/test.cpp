// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>
#include <SharedPtr.hpp>

TEST(Construcors, EmptyConstructor) {
  SharedPtr<int> Example;
  EXPECT_EQ(Example.get(), nullptr);
  EXPECT_EQ(Example.use_count(), 0);
  EXPECT_FALSE(Example);
}
TEST(Constructors, ConstructorForValue){
  int val = 20;
  SharedPtr<int> Example1(&val);
  SharedPtr<int> Example2(Example1);

  EXPECT_EQ(Example1.get(), &val);
  EXPECT_EQ(Example2.get(), &val);
  EXPECT_EQ(Example1.use_count(), Example2.use_count());
  EXPECT_EQ(Example2.use_count(), 2);
  EXPECT_EQ(Example1.use_count(), 2);
  EXPECT_EQ(*Example2, 20);
  EXPECT_EQ(*Example1, 20);
  EXPECT_EQ(Example1.operator->(),&val);
  EXPECT_EQ(Example2.operator->(),&val);
  EXPECT_TRUE(Example1);
  EXPECT_TRUE(Example2);
}
TEST(Constructors, ConstructorForStruct){
  struct TestStruct{
    int val = 10;
  };
  TestStruct Tested;
  SharedPtr<TestStruct> Example1(&Tested);
  SharedPtr<TestStruct> Example2(Example1);

  EXPECT_EQ(Example1.get(),&Tested);
  EXPECT_EQ(Example2.get(),&Tested);
  EXPECT_EQ(Example1.use_count(),Example2.use_count());
  EXPECT_EQ(Example2.use_count(), 2);
  EXPECT_EQ(Example1.use_count(),2);
  EXPECT_EQ(Example2->val, 10);
  EXPECT_EQ(Example1->val, 10);
  EXPECT_TRUE(Example1);
  EXPECT_TRUE(Example2);
}
TEST(Constructors, ConstructorForClass){
  class TestClass{
    public:
     TestClass(){ val = 10;};
   int Getter(){
        return val;
   };
   private:
    int val;
  };

  TestClass Tested;
  SharedPtr<TestClass> Example1(&Tested);
  SharedPtr<TestClass> Example2(Example1);

  EXPECT_EQ(Example1.get(),&Tested);
  EXPECT_EQ(Example2.get(),&Tested);
  EXPECT_EQ(Example1.use_count(),Example2.use_count());
  EXPECT_EQ(Example1.use_count(),2);
  EXPECT_EQ(Example1->Getter(), 10);
  EXPECT_EQ(Example2->Getter(), 10);
  EXPECT_TRUE(Example1);
  EXPECT_TRUE(Example2);
}
TEST(Constructors, ConstructorForVectors){
  std::vector<int> Tested = {1, 55, 32};
  SharedPtr<std::vector<int>> Example1(&Tested);
  SharedPtr<std::vector<int>> Example2(Example1);

  std::vector<int> Tested2 = Tested;
  EXPECT_EQ(Example1.get(),&Tested);
  EXPECT_EQ(Example2.get(),&Tested);
  EXPECT_EQ(Example1.use_count(),Example2.use_count());
  EXPECT_EQ(Example1.use_count(),2);
  EXPECT_EQ(*Example1,Tested2);
  EXPECT_EQ(*Example1,*Example2);
  EXPECT_EQ(Example1->size(),Tested.size());
  EXPECT_EQ(Example2->size(),Tested.size());
  EXPECT_TRUE(Example2);
  EXPECT_FALSE(!Example1);
}
TEST(Methods,UseCountMethod){
  int val = 10;
  SharedPtr<int> Example1(&val);
  EXPECT_EQ(Example1.use_count(),1);
  SharedPtr<int> Example2(Example1);
  EXPECT_EQ(Example1.use_count(),2);
  EXPECT_EQ(Example2.use_count(),2);
  SharedPtr<int> Example3(Example2);
  EXPECT_EQ(Example1.use_count(),3);
  EXPECT_EQ(Example2.use_count(),3);
  EXPECT_EQ(Example3.use_count(),3);
  if(Example3){
    SharedPtr<int> Example4(Example3);
    EXPECT_EQ(Example1.use_count(),4);
    EXPECT_EQ(Example4.use_count(),4);
  }
  EXPECT_EQ(Example3.use_count(),3);
}
TEST(Methods,GetMethod){
  int val = 10;
  SharedPtr<int> Example1(&val);
  EXPECT_EQ(Example1.get(),&val);
}
TEST(Methods,SwapMethod){
  int val1 = 10;
  int val2 = 20;

  SharedPtr<int> Example1(&val1);
  SharedPtr<int> Example2(&val2);
  SharedPtr<int> Example3(Example2);
  EXPECT_EQ(Example1.get(),&val1);
  EXPECT_EQ(Example1.use_count(), 1);
  EXPECT_EQ(*Example1, 10);
  EXPECT_EQ(Example2.get(),&val2);
  EXPECT_EQ(Example2.use_count(), 2);
  EXPECT_EQ(Example3.get(),&val2);
  EXPECT_EQ(Example3.use_count(), 2);
  EXPECT_EQ(*Example3, 20);
  Example1.swap(Example2);
  EXPECT_EQ(Example1.get(),&val2);
  EXPECT_EQ(Example1.use_count(), 2);
  EXPECT_EQ(*Example1, 20);
  EXPECT_EQ(Example2.get(),&val1);
  EXPECT_EQ(Example2.use_count(), 1);
  EXPECT_EQ(*Example2, 10);
  EXPECT_TRUE(Example1);
  EXPECT_TRUE(Example2);
  EXPECT_TRUE(Example3);
}
TEST(Methods, VoidResetMethod){
  int val = 10;
  SharedPtr<int> ptr1(&val);
  EXPECT_EQ(ptr1.get(), &val);
  EXPECT_EQ(ptr1.use_count(), 1);
  ptr1.reset();
  EXPECT_EQ(ptr1.get(), nullptr);
  EXPECT_EQ(ptr1.use_count(), 0);
}
TEST(Methods, PtrResetMethod){
  int val = 10;
  SharedPtr<int> ptr1(&val);
  EXPECT_EQ(ptr1.get(), &val);
  EXPECT_EQ(ptr1.use_count(), 1);
  int val1 = 56;
  ptr1.reset(&val1);
  EXPECT_EQ(ptr1.get(), &val1);
  EXPECT_EQ(ptr1.use_count(), 1);
}
TEST(Operators, BoolOperator){
  int val = 10;

  SharedPtr<int> Example1(&val);
  SharedPtr<int> Example2;
  EXPECT_TRUE(Example1);
  EXPECT_FALSE(Example2);
}
struct NotAssignConstructible {
  NotAssignConstructible& operator= (NotAssignConstructible&&) = delete;
};
TEST(Constructors, NotAssignConstructible) {
  NotAssignConstructible* Object = new NotAssignConstructible;
  SharedPtr<NotAssignConstructible> ptr1(Object);
  EXPECT_THROW(SharedPtr<NotAssignConstructible> ptr2(ptr1),
               std::runtime_error);
  EXPECT_THROW(SharedPtr<NotAssignConstructible> ptr3(std::move(ptr1)),
               std::runtime_error);
  delete Object;
}
TEST(Operators, EquelOperator){
  NotAssignConstructible* Object = new NotAssignConstructible;
  SharedPtr<NotAssignConstructible> ptr1(Object);
  SharedPtr<NotAssignConstructible> ptr4;
  SharedPtr<NotAssignConstructible> ptr5;
  EXPECT_THROW(ptr4 = ptr1,
               std::runtime_error);
  EXPECT_THROW(ptr5 = std::move(ptr1),
               std::runtime_error);
  delete Object;
}
TEST(MoveTest, IsConstructAssignable){
  EXPECT_TRUE(std::is_move_assignable<int>::value);
  EXPECT_TRUE(std::is_move_constructible<int>::value);
}
