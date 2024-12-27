#ifndef S21_CONTAINERS_SRC_S21_CONTAINERSPLUS_MULTISET_S21_MULTISET_H
#define S21_CONTAINERS_SRC_S21_CONTAINERSPLUS_MULTISET_S21_MULTISET_H

#include "../../s21_containers/AVLTree/AVLTree.h"

namespace s21 {

template <class Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = key_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using tree_type = RedBlackTree<value_type>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

  /**
   * @brief Конструктор по умолчанию.
   * Создает пустой multiset без элементов.
   */
  multiset() : tree_(new tree_type{}){};

  /**
   * @brief Конструктор с инициализатором списка.
   * Создает multiset и заполняет его элементами из инициализатора списка.
   *
   * @param items Инициализатор списка, содержащий элементы для добавления в
   * multiset.
   */
  multiset(std::initializer_list<value_type> const &items) : multiset() {
    for (auto item : items) insert(item);
  }

  /**
   * @brief Конструктор копирования.
   * Создает новый multiset как копию существующего.
   *
   * @param other Ссылка на существующий multiset, который будет скопирован.
   */
  multiset(const multiset &other) : tree_(new tree_type(*other.tree_)) {}

  /**
   * @brief Конструктор перемещения.
   * Создает новый multiset, перемещая элементы из другого multiset.
   *
   * @param other Ссылка на существующий multiset, элементы которого будут
   * перемещены.
   */
  multiset(multiset &&other) noexcept
      : tree_(new tree_type(std::move(*other.tree_))) {}

  /**
   * @brief Оператор присваивания копированием.
   * Копирует элементы из другого multiset в текущий.
   *
   * @param other Ссылка на существующий multiset, который будет скопирован.
   * @return Ссылка на текущий multiset после присваивания.
   */
  multiset &operator=(const multiset &other) {
    *tree_ = *other.tree_;
    return *this;
  }

  /**
   * @brief Оператор присваивания перемещением.
   * Перемещает элементы из другого multiset в текущий.
   *
   * @param other Ссылка на существующий multiset, элементы которого будут
   * перемещены.
   * @return Ссылка на текущий multiset после присваивания.
   */
  multiset &operator=(multiset &&other) noexcept {
    *tree_ = std::move(*other.tree_);
    return *this;
  }

  /**
   * @brief Деструктор.
   * Освобождает ресурсы, занимаемые multiset.
   */
  ~multiset() {
    delete tree_;
    tree_ = nullptr;
  }

 public:
  /**
   * @brief Возвращает итератор, указывающий на первый элемент multiset.
   *
   * @return Итератор, указывающий на первый элемент multiset.
   */
  iterator begin() noexcept { return tree_->Begin(); }

  /**
   * @brief Возвращает константный итератор, указывающий на первый элемент
   * multiset.
   *
   * @return Константный итератор, указывающий на первый элемент multiset.
   */
  const_iterator begin() const noexcept { return tree_->Begin(); }

  /**
   * @brief Возвращает итератор, указывающий на элемент, следующий за последним
   * элементом multiset.
   *
   * @return Итератор, указывающий на элемент, следующий за последним элементом
   * multiset.
   */
  iterator end() noexcept { return tree_->End(); }

  /**
   * @brief Возвращает константный итератор, указывающий на элемент, следующий
   * за последним элементом multiset.
   *
   * @return Константный итератор, указывающий на элемент, следующий за
   * последним элементом multiset.
   */
  const_iterator end() const noexcept { return tree_->End(); }

 public:
  /**
   * @brief Проверяет, пусто ли дерево.
   *
   * @return true, если дерево пустое, и false в противном случае.
   * @throws none
   */
  bool empty() const noexcept { return tree_->Empty(); }

  /**
   * @brief Возвращает количество элементов в дереве.
   *
   * @return Количество элементов в дереве.
   * @throws none
   */
  size_type size() const noexcept { return tree_->Size(); }

  /**
   * @brief Возвращает максимальное количество элементов, которое может
   * содержать дерево.
   *
   * @return Максимальное количество элементов, которое может содержать дерево.
   * @throws none
   */
  size_type max_size() const noexcept { return tree_->MaxSize(); }

 public:
  /**
   * @brief Очищает дерево, удаляя все элементы.
   *
   * @throws none
   */
  void clear() noexcept { return tree_->Clear(); }

  /**
   * @brief Вставляет элемент в дерево.
   *
   * Вставляет значение в дерево и возвращает итератор, указывающий на
   * вставленный элемент.
   *
   * @param value Значение, которое нужно вставить.
   * @return Итератор, указывающий на вставленный элемент.
   * @throws none
   */
  iterator insert(const value_type &value) { return tree_->Insert(value); }

  /**
   * @brief Удаляет элемент из дерева.
   *
   * Удаляет элемент, на который указывает итератор, из дерева.
   *
   * @param pos Итератор, указывающий на элемент, который нужно удалить.
   * @throws none
   */
  void erase(iterator pos) noexcept { tree_->Erase(pos); }

  /**
   * @brief Меняет местами содержимое двух деревьев.
   *
   * Меняет местами содержимое текущего дерева с содержимым другого дерева.
   *
   * @param other Дерево, с которым нужно поменяться местами.
   * @throws none
   */
  void swap(multiset &other) noexcept { tree_->Swap(*other.tree_); }

  /**
   * @brief Сливает содержимое двух деревьев.
   *
   * Сливает содержимое текущего дерева с содержимым другого дерева.
   * После слияния содержимое другого дерева очищается.
   *
   * @param other Дерево, с которым нужно слить содержимое.
   * @throws none
   */
  void merge(multiset &other) noexcept { tree_->Merge(*other.tree_); }

 public:
  /**
   * @brief Возвращает количество элементов с заданным ключом.
   *
   * Выполняет поиск элементов с заданным ключом и возвращает количество
   * найденных элементов.
   *
   * @param key Ключ, по которому нужно выполнить поиск.
   * @return Количество элементов с заданным ключом.
   * @throws none
   */
  size_type count(const key_type &key) const {
    auto lower_iterator = lower_bound(key);
    if (*lower_iterator != key) return 0;

    auto end_iterator = end();
    size_type result_count = 0;
    while (lower_iterator != end_iterator && *lower_iterator == key) {
      ++result_count;
      ++lower_iterator;
    }

    return result_count;
  }

  /**
   * @brief Находит элемент с заданным ключом.
   *
   * Выполняет поиск элемента с заданным ключом и возвращает итератор,
   * указывающий на найденный элемент. Если элемент не найден, возвращает
   * итератор, указывающий на конец дерева.
   *
   * @param key Ключ, по которому нужно выполнить поиск.
   * @return Итератор, указывающий на найденный элемент или на конец дерева.
   * @throws none
   */
  iterator find(const key_type &key) noexcept { return tree_->Find(key); }

  /**
   * @brief Находит элемент с заданным ключом.
   *
   * Выполняет поиск элемента с заданным ключом и возвращает константный
   * итератор, указывающий на найденный элемент. Если элемент не найден,
   * возвращает константный итератор, указывающий на конец дерева.
   *
   * @param key Ключ, по которому нужно выполнить поиск.
   * @return Константный итератор, указывающий на найденный элемент или на конец
   * дерева.
   * @throws none
   */
  const_iterator find(const key_type &key) const noexcept {
    return tree_->Find(key);
  }

  /**
   * @brief Проверяет, содержит ли дерево элемент с заданным ключом.
   *
   * Выполняет поиск элемента с заданным ключом и возвращает true, если элемент
   * найден, и false в противном случае.
   *
   * @param key Ключ, по которому нужно выполнить поиск.
   * @return true, если элемент с заданным ключом найден, и false в противном
   * случае.
   * @throws none
   */
  bool contains(const key_type &key) const noexcept {
    return tree_->Find(key) != tree_->End();
  }

  /**
   * @brief Возвращает пару итераторов, указывающих на начало и конец диапазона
   * элементов с заданным ключом.
   *
   * Выполняет поиск элементов с заданным ключом и возвращает пару итераторов:
   * первый указывает на первый элемент с заданным ключом, второй - на элемент,
   * следующий за последним элементом с заданным ключом. Если элементы с
   * заданным ключом отсутствуют, возвращает пару итераторов, указывающих на
   * конец дерева.
   *
   * @param key Ключ, по которому нужно выполнить поиск.
   * @return Пара итераторов, указывающих на начало и конец диапазона элементов
   * с заданным ключом.
   * @throws none
   */
  std::pair<iterator, iterator> equal_range(const key_type &key) noexcept {
    iterator first = lower_bound(key);
    iterator second = upper_bound(key);

    return std::pair<iterator, iterator>{first, second};
  }

  /**
   * @brief Возвращает пару константных итераторов, указывающих на начало и
   * конец диапазона элементов с заданным ключом.
   *
   * Выполняет поиск элементов с заданным ключом и возвращает пару константных
   * итераторов: первый указывает на первый элемент с заданным ключом, второй -
   * на элемент, следующий за последним элементом с заданным ключом. Если
   * элементы с заданным ключом отсутствуют, возвращает пару константных
   * итераторов, указывающих на конец дерева.
   *
   * @param key Ключ, по которому нужно выполнить поиск.
   * @return Пара константных итераторов, указывающих на начало и конец
   * диапазона элементов с заданным ключом.
   * @throws none
   */
  std::pair<const_iterator, const_iterator> equal_range(
      const key_type &key) const noexcept {
    const_iterator first = lower_bound(key);
    const_iterator second = upper_bound(key);

    return std::pair<const_iterator, const_iterator>{first, second};
  }

  /**
   * @brief Находит первый элемент, который не меньше указанного ключа.
   *
   * Этот метод возвращает итератор, указывающий на первый элемент, который не
   * меньше указанного ключа. Если такого элемента не найдено, возвращается
   * итератор конца.
   *
   * @param key Ключ для сравнения с элементами.
   * @return Итератор, указывающий на первый элемент, который не меньше ключа.
   * @throws None
   * @note Этот метод помечен как noexcept, что означает, что он не выбрасывает
   * исключения.
   */
  iterator lower_bound(const key_type &key) noexcept {
    return tree_->LowerBound(key);
  }

  /**
   * @brief Находит первый элемент, который не меньше указанного ключа.
   *
   * Этот метод возвращает константный итератор, указывающий на первый элемент,
   * который не меньше указанного ключа. Если такого элемента не найдено,
   * возвращается итератор конца.
   *
   * @param key Ключ для сравнения с элементами.
   * @return Константный итератор, указывающий на первый элемент, который не
   * меньше ключа.
   * @throws None
   * @note Этот метод помечен как noexcept, что означает, что он не выбрасывает
   * исключения.
   */
  const_iterator lower_bound(const key_type &key) const {
    return tree_->LowerBound(key);
  }

  /**
   * @brief Находит первый элемент, который больше указанного ключа.
   *
   * Этот метод возвращает итератор, указывающий на первый элемент, который
   * больше указанного ключа. Если такого элемента не найдено, возвращается
   * итератор конца.
   *
   * @param key Ключ для сравнения с элементами.
   * @return Итератор, указывающий на первый элемент, который больше ключа.
   * @throws None
   * @note Этот метод помечен как noexcept, что означает, что он не выбрасывает
   * исключения.
   */
  iterator upper_bound(const key_type &key) noexcept {
    return tree_->UpperBound(key);
  }

  /**
   * @brief Находит первый элемент, который больше указанного ключа.
   *
   * Этот метод возвращает константный итератор, указывающий на первый элемент,
   * который больше указанного ключа. Если такого элемента не найдено,
   * возвращается итератор конца.
   *
   * @param key Ключ для сравнения с элементами.
   * @return Константный итератор, указывающий на первый элемент, который больше
   * ключа.
   * @throws None
   * @note Этот метод помечен как noexcept, что означает, что он не выбрасывает
   * исключения.
   */
  const_iterator upper_bound(const key_type &key) const {
    return tree_->UpperBound(key);
  }

 public:
  /**
   * @brief Вставляет множество элементов в контейнер.
   *
   * Этот метод позволяет вставить в контейнер множество элементов, используя
   * переадресацию аргументов. Возвращает вектор пар, где каждая пара содержит
   * итератор на вставленный элемент и булево значение, указывающее, был ли
   * элемент вставлен (true) или уже существовал (false).
   *
   * @tparam Args Типы аргументов для вставки.
   * @param args Аргументы для вставки.
   * @return Вектор пар, содержащих итераторы на вставленные элементы и булевы
   * значения, указывающие на успешность вставки.
   * @throws None
   * @note Этот метод использует переадресацию аргументов для оптимизации
   * передачи параметров.
   */
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    return tree_->insert_many(std::forward<Args>(args)...);
  }

 private:
  tree_type *tree_;
};

}  // namespace s21

#endif