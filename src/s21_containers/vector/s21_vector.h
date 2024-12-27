#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_VECTOR_S21_VECTOR_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_VECTOR_S21_VECTOR_H

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace s21 {
// Определяем, чтобы работать с разными типами данных
template <typename T>
class vector {
 public:
  // Создаём псевдоним шаблона(синоним)
  using value_type = T;
  // Например reference<int> будет заменен на int&
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  // размер типа в байтах
  using size_type = std::size_t;
  // целое число, разница между двумя указателями
  using difference_type = std::ptrdiff_t;

 public:
  // Конструктор без параметров
  vector() {}
  // explicit - не может использоваться для неявных преобразований
  //  Example: explicit MyClass(int value){}
  // MyClass obj(10); - good
  // but MyClass obj2 = 10; - bad, don't working
  /**
   *@brief параметризированный коснтруктор
   *
   *@param size - размер вектора
   */
  explicit vector(size_type size) {
    size_ = size;
    capacity_ = size;
    buffer_ = nullptr;
    if (size > 0) {
      buffer_ = new value_type[capacity_];
    }
  }
  /**
   * @brief Инициализируем объекты вектора с помощью списка инициализации
   * @param init - параметр конструктора, который принимает ссылку.
   *Элементы с помощью которые инициализируется вектор
   *
   */
  vector(std::initializer_list<value_type> const &init)
      : size_{init.size()},
        capacity_(init.size()),
        buffer_{new value_type[capacity_]} {
    std::copy(init.begin(), init.end(), buffer_);
  }
  /**
   * @brief Конструктор копирования - копируем все из rhs
   * @param rhs - Объект их которого копируем
   *
   */
  vector(const vector &rhs) {
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;
    buffer_ = nullptr;
    if (size_ > 0) {
      buffer_ = new value_type[capacity_];
    }
    std::copy(rhs.begin(), rhs.end(), buffer_);
  }

  /**
   * @brief Конструктор перемещения - перемещаем все из rhs
   * @param rhs - Объект их которого перемещаем в текущий объект
   *
   */
  vector(vector &&rhs) noexcept {
    size_ = std::exchange(rhs.size_, 0);
    capacity_ = std::exchange(rhs.capacity_, 0);
    buffer_ = std::exchange(rhs.buffer_, nullptr);
  }
  /**
   * @brief Деструктор - чистим всю память
   */
  ~vector() { delete[] buffer_; }
  /**
   * @brief Оператор присваивания перемещением
   *
   * @param rhs Объект из готорого будут взяты ресурсы
   * @return Ссылка на текущий объект(объект в который перемещали)
   */
  constexpr vector &operator=(vector &&rhs) noexcept {
    if (this != &rhs) {
      this->swap(rhs);
      if (rhs.buffer_ != nullptr) delete[] rhs.buffer_;
      rhs.size_ = 0;
      rhs.capacity_ = 0;
      rhs.buffer_ = nullptr;
    }
    return *this;
  }

  /**
   * @brief Опреатор присваивания копированием, копируем все элементы из
   * переданного объекта
   *
   * @param rhs Объект из которого копируем
   * @return Результат копирования
   */
  constexpr vector &operator=(const vector &rhs) {
    // Проверка самоприсваивания
    if (this != &rhs) {
      delete[] buffer_;

      if (rhs.size_ > 0) {
        buffer_ = new value_type[rhs.capacity_];
        std::copy(rhs.begin(), rhs.end(), buffer_);
      }
      size_ = rhs.size_;
      capacity_ = rhs.capacity_;
    }

    return *this;
  }

  /**
   *constexpr - метод можно использовать во время компиляции
   *reference - указывает, что возвращается ссылка внутри вектора
   * @brief Используется для доступа к элементу по заданному индексу
   *
   * @param pos Индекс элемента к которому нужно получить доступ
   * @return Ссылка на элемент по заданному индексу внутри вектора
   */
  constexpr reference at(size_type pos) {
    if (pos >= size_)
      throw std::out_of_range("s21::vector::at The index is out of range");

    return buffer_[pos];
  }

  /**
   * @brief Получение доступа к элементу из контейнера
   *
   * @param pos Индекс элемента к которому требуется доступ
   * @return Элемент в векторе по заданному индексу
   */
  constexpr const_reference at(size_type pos) const {
    if (pos >= size_)
      throw std::out_of_range("s21::vector::at The index is out of range");

    return buffer_[pos];
  }

  /**
   * @brief Получение доступа к элементу из контейнера
   *
   * @param pos Индекс элемента к которому требуется доступ
   * @return Элемент в векторе по заданному индексу
   */
  constexpr reference operator[](size_type pos) { return at(pos); }

  /**
   * @brief Получение доступа к элементу из контейнера
   *
   * @param pos Индекс элемента к которому требуется доступ
   * @return Элемент в векторе по заданному индексу
   */
  constexpr const_reference operator[](size_type pos) const { return at(pos); }

  /**
   * @brief Получение доступа к первому элементу в контейнере
   *
   * @return Первый элемент в контейнере
   */
  constexpr reference front() {
    if (size_ == 0)
      throw std::out_of_range(
          "s21::vector::front Using methods on a "
          "zero sized container results "
          "in the UB");
    return *begin();
  }

  /**
   * @brief Получение доступа к первому элементу в контейнере
   *
   * @return Первый элемент в контейнере
   */
  constexpr const_reference front() const {
    if (size_ == 0)
      throw std::out_of_range(
          "s21::vector::front Using methods on a "
          "zero sized container results "
          "in the UB");
    return *begin();
  }

  /**
   * @brief Получение доступа к последнему элементу в контейнере
   *
   * @return Последний элемент в контейнере
   */
  constexpr reference back() {
    if (size_ == 0)
      throw std::out_of_range(
          "s21::vector::back Using methods on a zero "
          "sized container results "
          "in the UB");
    return *std::prev(end());
  }

  /**
   * @brief Получение доступа к последнему элементу в контейнере
   *
   * @return Последний элемент в контейнере
   */
  constexpr const_reference back() const {
    if (size_ == 0)
      throw std::out_of_range(
          "s21::vector::back Using methods on a zero "
          "sized container results "
          "in the UB");
    return *std::prev(end());
  }

  /**
   * @brief Доступ к базовому указателю на память, которую мы выделили
   *
   * @return Указатель на первый элемент вектора
   */
  constexpr iterator data() noexcept { return buffer_; }

  /**
   * @brief Доступ к базовому указателю на память, которую мы выделили
   *
   * @return Указатель на первый элемент вектора
   */
  constexpr const_iterator data() const noexcept { return buffer_; }

  /**
   * @brief Доступ к итератору, указывающий на первый элемент
   *
   * @return Указатель на первый элемент вектора
   */
  constexpr iterator begin() noexcept { return buffer_; }

  /**
   * @brief Доступ к итератору, указывающий на первый элемент
   *
   * @return Указатель на первый элемент вектора
   */
  constexpr const_iterator begin() const noexcept { return buffer_; }

  /**
   * @brief Доступ к итератору, указывающий на последний элемент
   *
   * @return Указатель на последний элемент вектора
   */
  constexpr iterator end() noexcept { return buffer_ + size_; }

  /**
   * @brief Доступ к итератору, указывающий на последний элемент
   *
   * @return Указатель на последний элемент вектора
   */
  constexpr const_iterator end() const noexcept { return buffer_ + size_; }

  // Capacity
 public:
  // nodiscard - атрибут, который позволяет компилятору выдавать предупреждение
  // если результат вызова метода не используется
  /**
   * @brief Проверка на пустоту вектора
   *
   * @return Правда если пустой, иначе ложь
   */
  [[nodiscard]] bool empty() const noexcept { return begin() == end(); }

  /**
   * @brief Текущий размер контейнера
   *
   * @return Размер контейнера
   */
  [[nodiscard]] constexpr size_type size() const noexcept {
    return std::distance(begin(), end());
  }

  /**
   * @brief Максимальное количество элементов, которое может содержать контейнер
   *
   * @return Максимальная вместимость
   */
  [[nodiscard]] constexpr size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type) / 2;
  }

  /**
   * @brief Увеличивает емкость вектора до значение <= new_cap
   * Если new_cap больше текущей емкости, то выделяется новое хранилище
   * иначе функция ничего не делает
   *
   * @param new_cap Новая емкость вектора, выраженная в количестве элементов
   */
  constexpr void reserve(size_type new_cap) {
    if (new_cap <= capacity_) return;

    if (new_cap > max_size())
      throw std::length_error(
          "s21::vector::reserve Reserve capacity can't be larger than "
          "Vector<T>::max_size()");

    ReallocVector(new_cap);
  }

  /**
   * @brief Возвращает количество элементов, для которых выделано
   * пространство в контейнере
   *
   * @return Текущая емкость
   */
  constexpr size_type capacity() const noexcept { return capacity_; }

  /**
   * @brief Запрашивает удаление неиспользуемой емкости
   */
  constexpr void shrink_to_fit() {
    if (capacity_ == size_) return;

    ReallocVector(size_);
  }

  /**
   * @brief Удаляет все элементы из контейнера. После вызова size равен нулю
   */
  constexpr void clear() noexcept { size_ = 0; }

  /**
   * @brief Вставляет элементы в указанное место в контейнере
   *
   * @param pos итератор, перед которым будет вставлено содержимое
   * @param value значение элемента, который нужно вставить
   * @return Итератор, указывающий на вставленное значение
   */
  constexpr iterator insert(const_iterator pos, value_type &&value) {
    size_type index = pos - begin();
    if (index > size_)
      throw std::out_of_range(
          "s21::vector::insert Unable to insert into a position out of "
          "range of begin() to end()");

    if (size_ == capacity_) ReallocVector(size_ ? size_ * 2 : 1);

    std::copy(begin() + index, end(), begin() + index + 1);
    *(buffer_ + index) = std::move(value);

    ++size_;
    return begin() + index;
  }

  /**
   * @brief Вставляет элементы в указанное место в контейнере
   *
   * @param pos итератор, перед которым будет вставлено содержимое
   * @param value значение элемента, который нужно вставить
   * @return Итератор, указывающий на вставленное значение
   */
  constexpr iterator insert(const_iterator pos, const_reference value) {
    size_type index = pos - begin();
    if (index > size_)
      throw std::out_of_range(
          "s21::vector::insert Unable to insert into a position out of "
          "range of begin() to end()");

    if (size_ == capacity_) ReallocVector(size_ ? size_ * 2 : 1);

    std::copy(begin() + index, end(), begin() + index + 1);
    *(buffer_ + index) = value;

    ++size_;
    return begin() + index;
  }

  /**
   * @brief Удаляет указанные элементы из контейнера
   *
   * @param pos итератор для удаления элемента
   * @return Итератор, следующий за последним удаленным элементом
   */
  constexpr iterator erase(const_iterator pos) {
    size_type index = pos - begin();
    if (index >= size_)
      throw std::out_of_range(
          "s21::vector::erase Unable to erase a position out of range of "
          "begin() to end()");

    std::copy(begin(), const_cast<iterator>(pos), buffer_);
    std::copy(const_cast<iterator>(pos) + 1, end(), buffer_ + index);

    --size_;
    return begin() + index;
  }

  /**
   * @brief Добавляет элемент в конец контейнера
   *
   * @param value Значение добавляемого элемента
   */
  constexpr void push_back(const_reference value) {
    if (size_ == capacity_) reserve(size_ ? size_ * 2 : 1);

    buffer_[size_] = value;
    ++size_;
  }

  /**
   * @brief Добавляет элемент в конец контейнера
   *
   * @param value Значение добавляемого элемента
   */
  constexpr void push_back(value_type &&value) {
    if (size_ == capacity_) reserve(size_ ? size_ * 2 : 1);

    buffer_[size_] = std::move(value);
    ++size_;
  }

  /**
   * @brief Удаляет последний элемент в контейнере
   * @details Итераторы и ссылки на последний элемент становятся
   * недействительными
   */
  constexpr void pop_back() {
    if (size_ != 0)
      // throw std::length_error(
      //     "s21::vector::pop_back Calling pop_back on an empty container "
      //     "results in UB");
      --size_;
  }

  /**
   * @brief Обменивает содержимое контейнера с содержимым других элементов.
   * Все итераторы и ссылки остаются действительными
   *
   * @param other контейнер для обмена содержимым
   */
  constexpr void swap(vector &other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  /**
   * @brief Вставляет новый элемент перед pos
   *
   * @param pos итератор перед которым будет создан новый элемент
   * @param args аргументы для пересылки конструктору элемента
   * @return Итератор, указывающий на внедренный элемент
   */
  template <typename... Args>
  constexpr iterator insert(const_iterator pos, Args &&...args) {
    iterator ret = nullptr;
    auto id = pos - begin();
    reserve(capacity_ + sizeof...(args));

    for (auto &&item : {std::forward<Args>(args)...}) {
      ret = insert(begin() + id++, item);
    }
    return ret;
  }

  /**
   * @brief Добавляет новый элемент в конец контейнера
   *
   * @param args аргументы для пересылки конструктору элемента
   * @return Итератор, указывающий на внедренный элемент
   */
  template <typename... Args>
  constexpr iterator push_back(Args &&...args) {
    for (auto &&item : {std::forward<Args>(args)...}) {
      push_back(item);
    }
    return end() - 1;
  }

 private:
  size_type size_ = 0;
  size_type capacity_ = 0;
  iterator buffer_ = nullptr;
  void ReallocVector(size_type new_capacity) {
    iterator tmp = new value_type[new_capacity];
    for (size_type i = 0; i < size_; ++i) {
      tmp[i] = std::move(buffer_[i]);
    }
    delete[] buffer_;
    buffer_ = tmp;
    capacity_ = new_capacity;
  }
};
}  // namespace s21

#endif