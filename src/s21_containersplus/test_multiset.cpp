#include <gtest/gtest.h>

#include "multiset/s21_multiset.h"

using namespace s21;

TEST(MultisetTest, DefaultConstructor) {
  multiset<int> ms;
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0);
}

TEST(MultisetTest, CopyConstructor) {
  multiset<int> ms1;
  ms1.insert(1);
  ms1.insert(2);
  multiset<int> ms2(ms1);
  EXPECT_EQ(ms2.size(), 2);
  EXPECT_TRUE(ms2.contains(1));
  EXPECT_TRUE(ms2.contains(2));
}

TEST(MultisetTest, AssignmentOperator) {
  multiset<int> ms1;
  ms1.insert(1);
  ms1.insert(2);
  multiset<int> ms2;
  ms2 = ms1;
  EXPECT_EQ(ms2.size(), 2);
  EXPECT_TRUE(ms2.contains(1));
  EXPECT_TRUE(ms2.contains(2));
}

TEST(MultisetTest, Insert) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  EXPECT_EQ(ms.size(), 2);
  EXPECT_TRUE(ms.contains(1));
  EXPECT_TRUE(ms.contains(2));
}

TEST(MultisetTest, Size) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  ms.insert(3);
  EXPECT_EQ(ms.size(), 3);
}

TEST(MultisetTest, Erase) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  ms.erase(ms.find(1));
  EXPECT_FALSE(ms.contains(1));
  EXPECT_EQ(ms.size(), 1);
}

TEST(MultisetTest, Swap) {
  multiset<int> ms1;
  ms1.insert(1);
  ms1.insert(2);
  multiset<int> ms2;
  ms2.insert(3);
  ms2.insert(4);
  ms1.swap(ms2);
  EXPECT_TRUE(ms1.contains(3));
  EXPECT_TRUE(ms1.contains(4));
  EXPECT_TRUE(ms2.contains(1));
  EXPECT_TRUE(ms2.contains(2));
}

TEST(MultisetTest, Merge) {
  multiset<int> ms1;
  ms1.insert(1);
  ms1.insert(2);
  multiset<int> ms2;
  ms2.insert(2);
  ms2.insert(3);
  ms1.merge(ms2);
  EXPECT_EQ(ms1.size(), 4);
  EXPECT_TRUE(ms1.contains(1));
  EXPECT_TRUE(ms1.contains(2));
  EXPECT_TRUE(ms1.contains(2));
  EXPECT_TRUE(ms1.contains(3));
}

TEST(MultisetTest, Count) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(1);
  ms.insert(2);
  EXPECT_EQ(ms.count(1), 2);
  EXPECT_EQ(ms.count(2), 1);
}

TEST(MultisetTest, Find) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  auto it = ms.find(1);
  EXPECT_TRUE(it != ms.end());
  EXPECT_EQ(*it, 1);
}

TEST(MultisetTest, Contains) {
  multiset<int> ms;
  ms.insert(1);
  EXPECT_TRUE(ms.contains(1));
  EXPECT_FALSE(ms.contains(2));
}

TEST(MultisetTest, EqualRange) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(1);
  ms.insert(2);
  auto range = ms.equal_range(1);

  // Проверяем, что первый элемент в диапазоне равен 1
  ASSERT_EQ(*range.first, 1);

  // Проверяем, что следующий элемент в диапазоне также равен 1
  ASSERT_EQ(*(++range.first), 1);

  // Проверяем, что последующий элемент в диапазоне не равен 1
  ASSERT_NE(*(++range.first), 1);
}

TEST(MultisetTest, LowerBound) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  auto it = ms.lower_bound(2);
  EXPECT_TRUE(it != ms.end());
  EXPECT_EQ(*it, 2);
}

TEST(MultisetTest, UpperBound) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  auto it = ms.upper_bound(1);
  EXPECT_TRUE(it != ms.end());
  EXPECT_EQ(*it, 2);
}

TEST(MultisetTest, InsertMany) {
  multiset<int> ms;
  ms.insert_many(1, 2, 3);
  EXPECT_EQ(ms.size(), 3);
  EXPECT_TRUE(ms.contains(1));
  EXPECT_TRUE(ms.contains(2));
  EXPECT_TRUE(ms.contains(3));
}