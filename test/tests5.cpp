#include <gtest/gtest.h>
#include "../include/list.h"
#include "../include/my_pmr.h"
#include <memory_resource>
#include "../include/square.h"


TEST(IteratorTest, DefaultConstructor) {
    My_iterator<int> it;
    EXPECT_EQ(it.operator->(), nullptr);
}

TEST(IteratorTest, NodeConstructor) {
    Node<int> node{42, nullptr, nullptr};
    My_iterator<int> it(&node);
    EXPECT_EQ(*it, 42);                 // сравниваем значение
    EXPECT_EQ(it.operator->(), &node.value); 
}

TEST(IteratorTest, CopyConstructor) {
    Node<int> node{42, nullptr, nullptr};
    My_iterator<int> it1(&node);
    My_iterator<int> it2(it1);
    EXPECT_EQ(it1.operator->(), it2.operator->());
}

TEST(IteratorTest, AssignmentOperator) {
    Node<int> node1{42, nullptr, nullptr};
    Node<int> node2{24, nullptr, nullptr};
    My_iterator<int> it1(&node1);
    My_iterator<int> it2(&node2);
    
    it2 = it1;
    EXPECT_EQ(it1.operator->(), it2.operator->());
}

TEST(IteratorTest, EqualityOperators) {
    Node<int> node1{42, nullptr, nullptr};
    Node<int> node2{24, nullptr, nullptr};
    My_iterator<int> it1(&node1);
    My_iterator<int> it2(&node1);  
    My_iterator<int> it3(&node2);  
    
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);
    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
}

TEST(IteratorTest, DereferenceOperator) {
    Node<int> node{42, nullptr, nullptr};
    My_iterator<int> it(&node);
    EXPECT_EQ(*it, 42);
}

TEST(IteratorTest, IncrementOperator) {
    Node<int> node2{2, nullptr, nullptr};
    Node<int> node1{1, &node2, nullptr};
    node2.prev = &node1;
    
    My_iterator<int> it(&node1);
    ++it;
    EXPECT_EQ(it.operator->(), &node2.value);
    EXPECT_EQ(*it, 2);
}

TEST(IteratorTest, DecrementOperator) {
    Node<int> node2{2, nullptr, nullptr};
    Node<int> node1{1, &node2, nullptr};
    node2.prev = &node1;
    
    My_iterator<int> it(&node2);
    --it;
    EXPECT_EQ(it.operator->(), &node1.value);
    EXPECT_EQ(*it, 1);
}

TEST(IteratorTest, IncrementNullThrows) {
    My_iterator<int> it(nullptr);
    EXPECT_THROW(++it, std::runtime_error);
}

TEST(DLinkedListTest, DefaultConstructor) {
    D_linked_list<int> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.lenght(), 0);
}

TEST(DLinkedListTest, MemoryResourceConstructor) {
    char buffer[1024];
    std::pmr::monotonic_buffer_resource pool{buffer, sizeof(buffer)};
    D_linked_list<int> list(&pool);
    EXPECT_TRUE(list.is_empty());
}

TEST(DLinkedListTest, PushFront) {
    D_linked_list<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    
    EXPECT_FALSE(list.is_empty());
    EXPECT_EQ(list.lenght(), 3);
}

TEST(DLinkedListTest, PushBack) {
    D_linked_list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    EXPECT_EQ(list.lenght(), 3);
}

TEST(DLinkedListTest, PopFront) {
    D_linked_list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    EXPECT_TRUE(list.pop_front());
    EXPECT_EQ(list.lenght(), 2);
    EXPECT_TRUE(list.pop_front());
    EXPECT_TRUE(list.pop_front());
    EXPECT_TRUE(list.is_empty());
    EXPECT_FALSE(list.pop_front());  // Empty list
}

TEST(DLinkedListTest, PopBack) {
    D_linked_list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    EXPECT_TRUE(list.pop_back());
    EXPECT_EQ(list.lenght(), 2);
    EXPECT_TRUE(list.pop_back());
    EXPECT_TRUE(list.pop_back());
    EXPECT_TRUE(list.is_empty());
    EXPECT_FALSE(list.pop_back());  // Empty list
}

TEST(DLinkedListTest, BeginEndIterators) {
    D_linked_list<int> list;
    list.push_back(1);
    list.push_back(2);
    
    auto begin = list.begin();
    auto end = list.end();
    
    EXPECT_NE(begin, end);
    EXPECT_EQ(*begin, 1);
}


TEST(DLinkedListTest, ComplexType) {
    struct TestStruct {
        int a;
        double b;
        TestStruct(int a, double b) : a(a), b(b) {}
    };
    
    D_linked_list<TestStruct> list;
    list.push_back(TestStruct{1, 2.0});
    list.push_back(TestStruct{3, 4.0});
    
    EXPECT_EQ(list.lenght(), 2);
    
    auto it = list.begin();
    EXPECT_EQ(it->a, 1);
    EXPECT_EQ(it->b, 2.0);
}

TEST(ListPMR, StoreSquares)
{
    LinearMemSource mem(4096);
    D_linked_list<Square<double>> lst(&mem);

    Square<double> sq1({0,0}, 10);
    Square<double> sq2({1,2}, 5);

    lst.push_back(sq1);
    lst.push_back(sq2);

    EXPECT_EQ(lst.lenght(), 2);

    auto head = lst.get_head();
    ASSERT_NE(head, nullptr);

    EXPECT_NEAR(head->value.Squere(), 2 * 10 * 10, 1e-6);
    EXPECT_NEAR(head->next->value.Squere(), 2 * 5 * 5, 1e-6);
}

TEST(DLinkedListTest, PMRAllocation) {
    char buffer[1024];
    std::pmr::monotonic_buffer_resource pool{buffer, sizeof(buffer)};
    
    D_linked_list<int> list(&pool);
    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
    }
    
    EXPECT_EQ(list.lenght(), 10);
}

TEST(LinearMemSourceTest, CustomSizeConstructor) {
    LinearMemSource resource(1024);
    void* ptr = resource.allocate(256, 8);
    EXPECT_NE(ptr, nullptr);
    resource.deallocate(ptr, 256, 8);
}

TEST(LinearMemSourceTest, MultipleAllocations) {
    LinearMemSource resource(512);
    
    void* ptr1 = resource.allocate(64, 8);
    void* ptr2 = resource.allocate(128, 8);
    void* ptr3 = resource.allocate(256, 8);
    
    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr3, nullptr);
    
    EXPECT_NE(ptr1, ptr2);
    EXPECT_NE(ptr1, ptr3);
    EXPECT_NE(ptr2, ptr3);
    
    resource.deallocate(ptr1, 64, 8);
    resource.deallocate(ptr2, 128, 8);
    resource.deallocate(ptr3, 256, 8);
}

TEST(LinearMemSourceTest, OutOfMemory) {
    LinearMemSource resource(128);  // Small buffer
    
    void* ptr1 = resource.allocate(64, 8);
    EXPECT_NE(ptr1, nullptr);
    
    void* ptr2 = resource.allocate(64, 8);
    EXPECT_NE(ptr2, nullptr);
    
    EXPECT_THROW((void)resource.allocate(64, 8), std::bad_alloc);
    
    resource.deallocate(ptr1, 64, 8);
    resource.deallocate(ptr2, 64, 8);
}

TEST(LinearMemSourceTest, IntegrationWithPMR) {
    LinearMemSource resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);
    
    int* ptr = alloc.allocate(5);  // Array of 5 integers
    for (int i = 0; i < 5; ++i) {
        ptr[i] = i * 10;
    }
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(ptr[i], i * 10);
    }
    
    alloc.deallocate(ptr, 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}