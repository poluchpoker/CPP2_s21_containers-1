#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_QUEUE_S21_QUEUE_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_QUEUE_S21_QUEUE_H

#include <algorithm>
#include <initializer_list>

#include "../list/s21_list.h"

namespace s21 {
template <typename T, typename Container = s21::list<T>>
class queue {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;

 public:
  /**
   * @brief Конструктор по умолчанию для класса queue.
   *
   * Создает пустую очередь без элементов.
   */
  queue() noexcept : container_{} {}

  /**
   * @brief Конструктор для инициализации очереди с помощью списка
   * инициализации.
   *
   * @param items Список инициализации, содержащий элементы, которые нужно
   * добавить в очередь.
   *
   * Создает очередь и инициализирует ее элементами из списка инициализации.
   */
  explicit queue(std::initializer_list<value_type> const &items)
      : container_{items} {}

  /**
   * @brief Конструктор копирования для класса queue.
   *
   * @param q Ссылка на объект queue, который нужно скопировать.
   *
   * Создает новую очередь, которая является копией заданной очереди.
   */
  queue(const queue &q) : container_{q.container_} {}

  /**
   * @brief Конструктор перемещения для класса queue.
   *
   * @param q Ссылка на объект queue, который нужно переместить.
   *
   * Создает новую очередь, перемещая содержимое заданной очереди.
   */
  queue(queue &&q) : container_{std::move(q.container_)} {}

  /**
   * @brief Деструктор по умолчанию для класса queue.
   *
   * Освобождает ресурсы, занимаемые объектом queue.
   */
  ~queue() noexcept = default;

  /**
   * @brief Оператор присваивания копирования для класса queue.
   *
   * @param q Ссылка на объект queue, который нужно скопировать.
   * @return Ссылку на текущий объект queue.
   *
   * Присваивает содержимое заданной очереди текущему объекту.
   */
  queue &operator=(const queue &q) {
    container_ = q.container_;
    return *this;
  }

  /**
   * @brief Оператор присваивания перемещения для класса queue.
   *
   * @param q Ссылка на объект queue, который нужно переместить.
   * @return Ссылку на текущий объект queue.
   *
   * Перемещает содержимое заданной очереди в текущий объект.
   */
  queue &operator=(queue &&q) noexcept {
    container_ = std::move(q.container_);
    return *this;
  }

 public:
  /**
   * @brief Возвращает ссылку на первый элемент очереди.
   *
   * @return Ссылка на первый элемент очереди.
   *
   * @note Элемент не удаляется из очереди.
   * @note Метод не выбрасывает исключения.
   */
  reference front() noexcept { return container_.front(); }

  /**
   * @brief Возвращает константную ссылку на первый элемент очереди.
   *
   * @return Константная ссылка на первый элемент очереди.
   *
   * @note Элемент не удаляется из очереди.
   * @note Метод не выбрасывает исключения.
   */
  const_reference front() const noexcept { return container_.front(); }

  /**
   * @brief Возвращает ссылку на последний элемент очереди.
   *
   * @return Ссылка на последний элемент очереди.
   *
   * @note Элемент не удаляется из очереди.
   * @note Метод не выбрасывает исключения.
   */
  reference back() noexcept { return container_.back(); }

  /**
   * @brief Возвращает константную ссылку на последний элемент очереди.
   *
   * @return Константная ссылка на последний элемент очереди.
   *
   * @note Элемент не удаляется из очереди.
   * @note Метод не выбрасывает исключения.
   */
  const_reference back() const noexcept { return container_.back(); }

 public:
  /**
   * @brief Проверяет, пуста ли очередь.
   *
   * @return true, если очередь пуста, и false в противном случае.
   *
   * @note Метод не выбрасывает исключения.
   */
  bool empty() const noexcept { return container_.empty(); }

  /**
   * @brief Возвращает количество элементов в очереди.
   *
   * @return Количество элементов в очереди.
   *
   * @note Метод не выбрасывает исключения.
   */
  size_type size() const noexcept { return container_.size(); }

  /**
   * @brief Добавляет элемент в конец очереди.
   *
   * @param value Ссылка на значение, которое нужно добавить в очередь.
   *
   * @note Метод не выбрасывает исключения.
   */
  void push(const_reference value) { container_.push_back(value); }

  /**
   * @brief Удаляет первый элемент из очереди.
   *
   * @note Метод не выбрасывает исключения.
   */
  void pop() noexcept { container_.pop_front(); }

  /**
   * @brief Меняет местами содержимое текущей очереди с содержимым другой
   * очереди.
   *
   * @param other Ссылка на другую очередь, с которой нужно поменяться
   * содержимым.
   *
   * @note Метод не выбрасывает исключения.
   */
  void swap(queue &other) { std::swap(container_, other.container_); }

  /**
   * @brief Вставляет несколько элементов в конец очереди.
   *
   * @tparam Args Типы аргументов, которые нужно вставить.
   * @param args Аргументы, которые нужно вставить в очередь.
   *
   * @note Метод использует перегрузку шаблонов для поддержки вставки нескольких
   * элементов разных типов.
   */
  template <typename... Args>
  void insert_many_back(Args &&...args) {
    container_.insert_many_back(std::forward<Args>(args)...);
  }

 private:
  Container container_;
};

}  // namespace s21

#endif