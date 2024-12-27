// RedBlackTreeTest.cpp
#include "AVLTree/AVLTree.h"
#include "gtest/gtest.h"

using namespace s21;

TEST(RedBlackTreeTest, MergeTrees) {
  RedBlackTree<int> tree1;
  RedBlackTree<int> tree2;

  // Вставляем элементы в первое дерево
  tree1.Insert(1);
  tree1.Insert(2);
  tree1.Insert(3);

  // Вставляем элементы во второе дерево
  tree2.Insert(4);
  tree2.Insert(5);
  tree2.Insert(6);

  // Сливаем два дерева
  tree1.Merge(tree2);

  // Проверяем, что корень первого дерева теперь содержит все элементы обоих
  // деревьев
  ASSERT_EQ(tree1.GetRoot()->key_, 2);
  ASSERT_EQ(tree1.GetRoot()->left_->key_, 1);
  ASSERT_EQ(tree1.GetRoot()->right_->key_, 4);
  ASSERT_EQ(tree1.GetRoot()->right_->left_->key_, 3);
  ASSERT_EQ(tree1.GetRoot()->right_->right_->key_, 5);
  ASSERT_EQ(tree1.GetRoot()->right_->right_->right_->key_, 6);
}