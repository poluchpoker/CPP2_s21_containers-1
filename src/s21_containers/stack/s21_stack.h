#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_STACK_S21_STACK_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_STACK_S21_STACK_H

#include <algorithm>
#include <initializer_list>

#include "../list/s21_list.h"

namespace s21 {
template <typename T, typename Container = s21::list<T>>
class stack {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;

 public:
  /**
   * @brief Конструктор по умолчанию для класса stack.
   *
   * Создает пустую стековую структуру без элементов.
   */
  stack() noexcept : container_{} {}

  /**
   * @brief Конструктор для инициализации стека с помощью списка инициализации.
   *
   * @param items Список инициализации, содержащий элементы, которые нужно
   * добавить в стек.
   *
   * Создает стек и инициализирует его элементами из списка инициализации.
   */
  explicit stack(std::initializer_list<value_type> const &items)
      : container_{items} {}

  /**
   * @brief Конструктор копирования для класса stack.
   *
   * @param s Ссылка на объект stack, который нужно скопировать.
   *
   * Создает новую стековую структуру, которая является копией заданной стека.
   */
  stack(const stack &s) : container_{s.container_} {}

  /**
   * @brief Конструктор перемещения для класса stack.
   *
   * @param s Ссылка на объект stack, который нужно переместить.
   *
   * Создает новую стековую структуру, перемещая содержимое заданной стека.
   */
  stack(stack &&s) noexcept : container_{std::move(s.container_)} {}

  /**
   * @brief Деструктор по умолчанию для класса stack.
   *
   * Освобождает ресурсы, занимаемые объектом stack.
   */
  ~stack() = default;

  /**
   * @brief Оператор присваивания копирования для класса stack.
   *
   * @param other Ссылка на объект stack, который нужно скопировать.
   * @return Ссылку на текущий объект stack.
   *
   * Присваивает содержимое заданной стека текущему объекту.
   */
  stack &operator=(const stack &other) {
    container_ = other.container_;
    return *this;
  }

  /**
   * @brief Оператор присваивания перемещения для класса stack.
   *
   * @param other Ссылка на объект stack, который нужно переместить.
   * @return Ссылку на текущий объект stack.
   *
   * Перемещает содержимое заданной стека в текущий объект.
   */
  stack &operator=(stack &&other) {
    container_ = std::move(other.container_);
    return *this;
  }

 public:
  /**
   * @brief Возвращает ссылку на верхний элемент стека.
   *
   * @return Ссылка на верхний элемент стека.
   *
   * @note Элемент не удаляется из стека.
   * @note Метод не выбрасывает исключения.
   */
  reference top() noexcept { return container_.back(); }

  /**
   * @brief Возвращает константную ссылку на верхний элемент стека.
   *
   * @return Константная ссылка на верхний элемент стека.
   *
   * @note Элемент не удаляется из стека.
   * @note Метод не выбрасывает исключения.
   */
  const_reference top() const noexcept { return container_.back(); }

 public:
  /**
   * @brief Проверяет, пуст ли стек.
   *
   * @return true, если стек пуст, и false в противном случае.
   *
   * @note Метод не выбрасывает исключения.
   */
  bool empty() const noexcept { return container_.empty(); }

  /**
   * @brief Возвращает количество элементов в стеке.
   *
   * @return Количество элементов в стеке.
   *
   * @note Метод не выбрасывает исключения.
   */
  size_type size() const noexcept { return container_.size(); }

 public:
  /**
   * @brief Добавляет элемент в верх стека.
   *
   * @param value Ссылка на значение, которое нужно добавить в стек.
   *
   * @note Метод не выбрасывает исключения.
   */
  void push(const_reference value) { container_.push_back(value); }

  /**
   * @brief Удаляет верхний элемент из стека.
   *
   * @note Метод не выбрасывает исключения.
   */
  void pop() noexcept { container_.pop_back(); }

  /**
   * @brief Меняет местами содержимое текущего стека с содержимым другого стека.
   *
   * @param other Ссылка на другой стек, с которым нужно поменяться содержимым.
   *
   * @note Метод не выбрасывает исключения.
   */
  void swap(stack &other) noexcept { std::swap(container_, other.container_); }

  /**
   * @brief Вставляет несколько элементов в начало стека.
   *
   * @tparam Args Типы аргументов, которые нужно вставить.
   * @param args Аргументы, которые нужно вставить в стек.
   *
   * @note Метод использует перегрузку шаблонов для поддержки вставки нескольких
   * элементов разных типов.
   */
  template <typename... Args>
  void insert_many_front(Args &&...args) {
    container_.insert_many_back(std::forward<Args>(args)...);
  }

 private:
  Container container_;
};

}  // namespace s21

#endif