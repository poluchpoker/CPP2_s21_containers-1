#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_SET_S21_SET_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_SET_S21_SET_H

#include <vector>

#include "../AVLTree/AVLTree.h"

namespace s21 {
/**
 * @brief Реализация структуры данных "множество" с использованием
 * красно-черного дерева.
 *
 * Множество - это коллекция уникальных элементов. В этом классе реализованы
 * основные операции над множеством: вставка, удаление, поиск, проверка наличия
 * элемента и другие.
 */
template <class Key>
class set {
 public:
  using key_type = Key;
  using value_type = key_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using tree_type = RedBlackTree<value_type>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

 public:
  /**
   * @brief Конструктор по умолчанию.
   */
  set() : tree_(new tree_type{}) {}

  /**
   * @brief Конструктор из инициализатора.
   * @param items Инициализатор, содержащий элементы для добавления в набор.
   */
  set(std::initializer_list<value_type> const &items) : set() {
    for (auto item : items) insert(item);
  }

  /**
   * @brief Конструктор копирования.
   * @param other Ссылка на другой объект типа set для копирования.
   */
  set(const set &other) : tree_(new tree_type(*other.tree_)) {}

  /**
   * @brief Конструктор перемещения.
   * @param other Указатель на другой объект типа set для перемещения.
   */
  set(set &&other) noexcept : tree_(new tree_type(std::move(*other.tree_))) {}

  /**
   * @brief Оператор присваивания по значению.
   * @param other Ссылка на другой объект типа set для копирования.
   * @return Ссылка на текущий объект после присваивания.
   */
  set &operator=(const set &other) {
    *tree_ = *other.tree_;
    return *this;
  }

  /**
   * @brief Оператор присваивания по значению.
   * @param other Указатель на другой объект типа set для перемещения.
   * @return Ссылка на текущий объект после присваивания.
   */
  set &operator=(set &&other) noexcept {
    *tree_ = std::move(*other.tree_);
    return *this;
  }

  /**
   * @brief Деструктор.
   */
  ~set() {
    delete tree_;

    tree_ = nullptr;
  }

 public:
  /**
   * @brief Возвращает итератор, указывающий на начало набора.
   *
   * Этот метод возвращает итератор, который указывает на первый элемент в
   * наборе. Если набор пуст, возвращаемый итератор будет равен `end()`.
   *
   * @return Итератор, указывающий на начало набора.
   */
  iterator begin() noexcept { return tree_->Begin(); }

  /**
   * @brief Возвращает константный итератор, указывающий на начало набора.
   *
   * Этот метод возвращает константный итератор, который указывает на первый
   * элемент в наборе. Если набор пуст, возвращаемый итератор будет равен
   * `end()`.
   *
   * @return Константный итератор, указывающий на начало набора.
   */
  const_iterator begin() const noexcept { return tree_->Begin(); }

  /**
   * @brief Возвращает итератор, указывающий на конец набора.
   *
   * Этот метод возвращает итератор, который указывает на позицию следующей за
   * последним элементом в наборе. Если набор пуст, возвращаемые итераторы будут
   * равны `begin()`.
   *
   * @return Итератор, указывающий на конец набора.
   */
  iterator end() noexcept { return tree_->End(); }

  /**
   * @brief Возвращает константный итератор, указывающий на конец набора.
   *
   * Этот метод возвращает константный итератор, который указывает на позицию
   * следующей за последним элементом в наборе. Если набор пуст, возвращаемые
   * итераторы будут равны `begin()`.
   *
   * @return Константный итератор, указывающий на конец набора.
   */
  const_iterator end() const noexcept { return tree_->End(); }

 public:
  /**
   * @brief Проверяет, является ли набор пустым.
   *
   * Этот метод возвращает `true`, если набор не содержит элементов, и `false` в
   * противном случае.
   *
   * @return `true`, если набор пуст, иначе `false`.
   */
  bool empty() const noexcept { return tree_->Empty(); }

  /**
   * @brief Возвращает количество элементов в наборе.
   *
   * Этот метод возвращает количество элементов, хранящихся в наборе.
   *
   * @return Количество элементов в наборе.
   */
  size_type size() const noexcept { return tree_->Size(); }

  /**
   * @brief Возвращает максимально возможное количество элементов, которое может
   * хранить набор.
   *
   * Этот метод возвращает максимально возможное количество элементов, которое
   * может хранить набор.
   *
   * @return Максимальное количество элементов, которое может хранить набор.
   */
  size_type max_size() const noexcept { return tree_->MaxSize(); }

 public:
  /**
   * @brief Очищает набор от всех элементов.
   *
   * Этот метод удаляет все элементы из набора, оставляя его пустым.
   */
  void clear() noexcept { tree_->Clear(); }

  /**
   * @brief Вставляет элемент в набор.
   *
   * Этот метод вставляет указанный элемент в набор, если он еще не
   * присутствует. Возвращает пару, где первый элемент - итератор к вставленному
   * элементу, а второй - булево значение, указывающее, был ли элемент успешно
   * вставлен.
   *
   * @param value Элемент для вставки в набор.
   * @return Пара, содержащая итератор к вставленному элементу и булево
   * значение.
   */
  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_->InsertUnique(value);
  }

  /**
   * @brief Удаляет элемент из набора по указанному итератору.
   *
   * Этот метод удаляет элемент, на который указывает переданный итератор, из
   * набора.
   *
   * @param pos Итератор, указывающий на элемент для удаления.
   */
  void erase(iterator pos) noexcept { tree_->Erase(pos); }

  /**
   * @brief Обменивает содержимое двух наборов.
   *
   * Этот метод обменивает содержимое текущего набора и другого набора.
   *
   * @param other Ссылка на другой набор для обмена.
   */
  void swap(set &other) noexcept { tree_->Swap(*other.tree_); }

  /**
   * @brief Сливает содержимое двух наборов.
   *
   * Этот метод сливает содержимое текущего набора и другого набора, сохраняя
   * уникальность элементов.
   *
   * @param other Ссылка на другой набор для слияния.
   */
  void merge(set &other) noexcept { tree_->MergeUnique(*other.tree_); }

 public:
  /**
   * @brief Нахождение элемента по ключу.
   *
   * Этот метод возвращает итератор, указывающий на элемент с заданным ключом,
   * если такой элемент существует в наборе. Если элемент не найден,
   * возвращается итератор, равный `end()`.
   *
   * @param key Ключ элемента для поиска.
   * @return Итератор, указывающий на найденный элемент или `end()`, если
   * элемент не найден.
   */
  iterator find(const key_type &key) noexcept { return tree_->Find(key); }

  /**
   * @brief Нахождение элемента по ключу (константная версия).
   *
   * Этот метод возвращает константный итератор, указывающий на элемент с
   * заданным ключом, если такой элемент существует в наборе. Если элемент не
   * найден, возвращается итератор, равный `end()`.
   *
   * @param key Ключ элемента для поиска.
   * @return Константный итератор, указывающий на найденный элемент или `end()`,
   * если элемент не найден.
   */
  const_iterator find(const key_type &key) const noexcept {
    return tree_->Find(key);
  }

  /**
   * @brief Проверяет, содержит ли набор элемент с заданным ключом.
   *
   * Этот метод возвращает `true`, если элемент с заданным ключом присутствует в
   * наборе, и `false` в противном случае.
   *
   * @param key Ключ элемента для проверки.
   * @return `true`, если элемент с заданным ключом присутствует в наборе, и
   * `false` в противном случае.
   */
  bool contains(const key_type &key) {
    return tree_->Find(key) != tree_->End();
  }

 public:
  /**
   * @brief Вставляет множество элементов в набор.
   *
   * Этот метод вставляет указанные элементы в набор, если они еще не
   * присутствуют. Возвращает вектор пар, где первый элемент каждой пары -
   * итератор к вставленному элементу, а второй - булево значение, указывающее,
   * был ли элемент успешно вставлен.
   *
   * @tparam Args Типы аргументов, которые могут быть переданы в метод.
   * @param args Переменные аргументы, представляющие элементы для вставки в
   * набор.
   * @return Вектор пар, содержащих итераторы к вставленным элементам и булевы
   * значения, указывающие на успешность вставки.
   */
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &...args) {
    return tree_->insert_many_unique(std::forward<Args>(args)...);
  }

 private:
  tree_type *tree_;
};

}  // namespace s21

#endif