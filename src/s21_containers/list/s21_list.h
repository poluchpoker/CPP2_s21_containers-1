#ifndef S21_CONTAINERS_SRC_S21_CONTAINERS_LIST_S21_LIST_H
#define S21_CONTAINERS_SRC_S21_CONTAINERS_LIST_S21_LIST_H

#include <initializer_list>
#include <iterator>
#include <limits>

namespace s21 {
template <typename Type>
class list {
 private:
  struct ListNode;
  struct ListIterator;
  struct ListIteratorConst;

 public:
  // Тип элемента, где Type - параметр шаблона
  using value_type = Type;
  // Тип ссылки на элемент
  using reference = Type &;
  // Тип константной ссылки на элемент
  using const_reference = const Type &;
  // Внутренний класс для итератора
  using iterator = ListIterator;
  // Внутренний класс для константного итератора
  using const_iterator = ListIteratorConst;
  // Тип для размера класса
  using size_type = std::size_t;

  // Внутренний класс узла списка
  using node_type = ListNode;

  /**
   * @brief Конструктор по умолчанию для списка.
   * Создает пустой список без элементов.
   * @note Инициализирует голову списка как новый узел с пустым значением.
   */
  list() : head_(new node_type{}), size_(0U) {}

  /**
   * @brief Конструктор списка с заданным количеством элементов.
   * Создает список с заданным количеством элементов, инициализированных
   * значением по умолчанию.
   * @param n Количество элементов в списке.
   * @note Использует цикл для добавления элементов в список.
   */
  explicit list(size_type n) : list() {
    while (n > 0) {
      push_back(value_type{});
      --n;
    }
  }

  /**
   * @brief Конструктор списка с инициализацией списком.
   * Создает список и инициализирует его элементами из инициализатора списка.
   * @param items Инициализатор списка, содержащий значения элементов.
   */
  list(std::initializer_list<value_type> const &items) : list() {
    for (auto item : items) push_back(item);
  }

  /**
   * @brief Конструктор копирования списка.
   * Создает новый список, копируя элементы из другого списка.
   * @param other Ссылка на существующий список для копирования.
   */
  list(const list &other) : list() {
    for (auto list_element : other) push_back(list_element);
  }

  /**
   * @brief Конструктор перемещения списка.
   * Создает новый список, перемещая элементы из другого списка.
   * @param other Ссылка на существующий список для перемещения.
   */
  list(list &&other) noexcept : list() { splice(begin(), other); }

  /**
   * @brief Оператор присваивания копированием.
   * Заменяет содержимое текущего списка копией содержимого другого списка.
   * @param other Ссылка на существующий список для копирования.
   * @return Ссылку на текущий список после присваивания.
   * @note Очищает текущий список перед копированием элементов из другого
   * списка.
   */
  list &operator=(const list &other) {
    if (this != &other) {
      iterator this_begin = begin();
      iterator this_end = end();
      const_iterator other_begin = other.begin();
      const_iterator other_end = other.end();

      while (this_begin != this_end && other_begin != other_end) {
        *this_begin = *other_begin;
        ++this_begin;
        ++other_begin;
      }

      while (this_begin != this_end) {
        iterator tmp = this_begin;
        ++this_begin;
        erase(tmp);
      }

      while (other_begin != other_end) {
        push_back(*other_begin);
        ++other_begin;
      }
    }

    return *this;
  }

  /**
   * @brief Оператор присваивания перемещением.
   * Заменяет содержимое текущего списка перемещением содержимого другого
   * списка.
   * @param other Ссылка на существующий список для перемещения.
   * @return Ссылку на текущий список после присваивания.
   * @note Очищает текущий список перед перемещением элементов из другого
   * списка.
   */
  list &operator=(list &&other) noexcept {
    if (this != &other) {
      clear();
      splice(begin(), other);
    }

    return *this;
  }

  /**
   * @brief Деструктор списка.
   * Освобождает память, занимаемую списком, и удаляет все элементы.
   * @note Очищает список перед удалением головы списка.
   */
  ~list() {
    clear();
    delete head_;
    head_ = nullptr;
  }

  /**
   * @brief Возвращает ссылку на первый элемент списка.
   * @return Ссылка на первый элемент списка.
   * @note Для изменения значения элемента используйте версию без `const`.
   */
  reference front() noexcept { return *begin(); }

  /**
   * @brief Возвращает константную ссылку на первый элемент списка.
   * @return Константная ссылка на первый элемент списка.
   * @note Используется для чтения значения элемента без его изменения.
   */
  const_reference front() const noexcept { return *begin(); }

  /**
   * @brief Возвращает ссылку на последний элемент списка.
   * @return Ссылка на последний элемент списка.
   * @note Для изменения значения элемента используйте версию без `const`.
   */
  reference back() noexcept { return *std::prev(end()); }

  /**
   * @brief Возвращает константную ссылку на последний элемент списка.
   * @return Константная ссылка на последний элемент списка.
   * @note Используется для чтения значения элемента без его изменения.
   */
  const_reference back() const noexcept { return *std::prev(end()); }

  /**
   * @brief Возвращает итератор, указывающий на первый элемент списка.
   * @return Итератор, указывающий на первый элемент списка.
   * @note Используется для обхода списка с начала.
   */
  iterator begin() noexcept { return iterator{head_->next_}; }

  /**
   * @brief Возвращает константный итератор, указывающий на первый элемент
   * списка.
   * @return Константный итератор, указывающий на первый элемент списка.
   * @note Используется для обхода списка с начала без изменения его элементов.
   */
  const_iterator begin() const noexcept { return const_iterator{head_->next_}; }

  /**
   * @brief Возвращает итератор, указывающий на элемент после последнего
   * элемента списка.
   * @return Итератор, указывающий на элемент после последнего элемента списка.
   * @note Используется для обхода списка до его конца.
   */
  iterator end() noexcept { return iterator{head_}; }

  /**
   * @brief Возвращает константный итератор, указывающий на элемент после
   * последнего элемента списка.
   * @return Константный итератор, указывающий на элемент после последнего
   * элемента списка.
   * @note Используется для обхода списка до его конца без изменения его
   * элементов.
   */
  const_iterator end() const noexcept { return const_iterator{head_}; }

  /**
   * @brief Проверяет, является ли список пустым.
   * @return `true`, если список пуст, и `false` в противном случае.
   * @note Эффективный способ проверить, содержит ли список элементы.
   */
  bool empty() const noexcept { return size_ == 0; }

  /**
   * @brief Возвращает количество элементов в списке.
   * @return Количество элементов в списке.
   * @note Эффективный способ получить размер списка.
   */
  size_type size() const noexcept { return size_; }

  /**
   * @brief Возвращает максимально возможное количество элементов, которые может
   * содержать список.
   *
   * @details Вообще, можно учитывать факт, что GCC ограничивает ровно в
   * половину адресного пространства, также можно учитывать размерность нашего
   * контейнера(node_type), и, также можно учитывать узел, те в итоге у нас
   * получается, что можно прийти к следующему:
   * (std::numeric_limits<size_type>::max()/2) / sizeof(node_type) - 1.
   * С этим можно согласиться, но, если учитывать тот факт, что не все
   * компьютеры имеют огромную доступную память, то мы дойдем до максимума, и,
   * соответственно, данная формула будет уже неверна.
   *
   * @return Максимальное количество элементов, которые может содержать список.
   * @note Это значение теоретически, и на практике оно может быть ограничено
   * доступной памятью системы.
   * @note Метод не изменяет состояние списка и не вызывает исключений.
   */
  size_type max_size() const noexcept {
    // Возвращаем максимальное значение size_type, которое теоретически может
    // быть достигнуто. На практике это значение может быть ограничено доступной
    // памятью системы.
    return std::numeric_limits<size_type>::max();
  }

  /**
   * @brief Очищает список, удаляя все его элементы.
   * @note Очистка списка происходит путем удаления каждого элемента, начиная с
   * начала списка.
   */
  void clear() noexcept {
    while (size_ > 0) erase(begin());
  }

  /**
   * @brief Вставляет элемент в список перед указанным итератором.
   * @param pos Итератор, перед которым будет вставлен новый элемент.
   * @param value Значение элемента, который нужно вставить.
   * @return Итератор, указывающий на вставленный элемент.
   * @note Вставка происходит путем создания нового узла и присоединения его к
   * списку перед указанным итератором.
   */
  iterator insert(iterator pos, const_reference value) {
    node_type *new_node = new node_type(value);
    pos.node_->AttachPrev(new_node);
    ++size_;

    return iterator(new_node);
  }

  /**
   * @brief Удаляет элемент из списка по указанному итератору.
   * @param pos Итератор, указывающий на элемент, который нужно удалить.
   * @note Удаление происходит путем отсоединения узла от списка и освобождения
   * памяти.
   */
  void erase(iterator pos) noexcept {
    if (pos != end()) {
      pos.node_->UnAttach();
      delete pos.node_;
      --size_;
    }
  }

  /**
   * @brief Добавляет элемент в конец списка.
   * @param value Значение элемента, который нужно добавить.
   * @note Добавление происходит путем вставки нового элемента перед итератором,
   * указывающим на конец списка.
   */
  void push_back(const_reference value) { insert(end(), value); }

  /**
   * @brief Удаляет последний элемент из списка.
   * @note Удаление происходит путем вызова метода `erase` с итератором,
   * указывающим на предпоследний элемент.
   */
  void pop_back() noexcept { erase(--end()); }

  /**
   * @brief Добавляет элемент в начало списка.
   * @param value Значение элемента, который нужно добавить.
   * @note Добавление происходит путем вставки нового элемента перед итератором,
   * указывающим на начало списка.
   */
  void push_front(const_reference value) noexcept { insert(begin(), value); }

  /**
   * @brief Удаляет первый элемент из списка.
   * @note Удаление происходит путем вызова метода `erase` с итератором,
   * указывающим на начало списка.
   */
  void pop_front() noexcept { erase(begin()); }

  /**
   * @brief Меняет местами содержимое текущего списка с содержимым другого
   * списка.
   * @param other Ссылка на другой список, с которым нужно поменяться
   * содержимым.
   * @note Меняет местами головы списка и размеры обоих списков.
   */
  void swap(list &other) noexcept {
    if (this != &other) {
      std::swap(head_, other.head_);
      std::swap(size_, other.size_);
    }
  }

  /**
   * Слияние двух списков.
   *
   * Эта функция объединяет элементы из списка `other` в текущий список. Списки
   * должны быть отсортированы перед вызовом этой функции. После слияния список
   * `other` очищается.
   *
   * @param other Ссылка на другой список, элементы которого нужно добавить в
   * текущий список.
   */
  void merge(list &other) {
    if (this != &other) {
      iterator this_begin = begin();
      iterator this_end = end();
      iterator other_begin = other.begin();
      iterator other_end = other.end();

      while (this_begin != this_end && other_begin != other_end) {
        if (*other_begin < *this_begin) {
          node_type *tmp = other_begin.node_;
          ++other_begin;
          tmp->UnAttach();
          --other.size_;
          this_begin.node_->AttachPrev(tmp);
          ++size_;
        } else {
          ++this_begin;
        }
      }

      splice(end(), other);
    }
  }

  /**
   * Вставка элементов из другого списка в текущий список.
   *
   * Эта функция вставляет все элементы из списка `other` в текущий список,
   * начиная с позиции `pos`. После вставки список `other` остается неизменным.
   *
   * @param pos Итератор, указывающий позицию в текущем списке, куда будут
   * вставлены элементы из `other`.
   * @param other Ссылка на другой список, элементы которого нужно вставить в
   * текущий список.
   */
  void splice(const_iterator pos, list &other) noexcept {
    if (!other.empty()) {
      iterator it_current{const_cast<node_type *>(pos.node_)};
      iterator it_other = other.end();

      it_other.node_->next_->prev_ = it_current.node_->prev_;
      it_other.node_->prev_->next_ = it_current.node_;

      it_current.node_->prev_->next_ = it_other.node_->next_;
      it_current.node_->prev_ = it_other.node_->prev_;

      size_ += other.size();
      other.size_ = 0;

      other.head_->next_ = other.head_;
      other.head_->prev_ = other.head_;
    }
  }

  /**
   * @brief Переворачивает порядок элементов в списке.
   *
   * Эта функция изменяет порядок элементов в списке таким образом, что
   * последний элемент становится первым, а первый - последним. Функция не
   * возвращает никакого значения, она просто изменяет порядок элементов в
   * списке.
   *
   * Пример использования:
   * @code
   * list_name.reverse();
   * @endcode
   *
   * @note Функция не выбрасывает исключений (`noexcept`).
   */
  void reverse() noexcept {
    iterator begin_iterator = begin();
    iterator end_iterator = end();

    while (begin_iterator != end_iterator) {
      begin_iterator.node_->SwapNextPrev();
      --begin_iterator;
    }
    head_->SwapNextPrev();
  }

  /**
   * @brief Удаляет дублирующиеся элементы из списка.
   *
   * Эта функция проходит по списку и удаляет все дублирующиеся элементы,
   * оставляя только уникальные значения. Если список пуст, функция не выполняет
   * никаких действий.
   *
   * Пример использования:
   * @code
   * list_name.unique();
   * @endcode
   *
   * @note Функция не возвращает никакого значения и не выбрасывает исключений.
   */
  void unique() {
    if (!this->empty()) {
      for (iterator it_last = begin(); it_last != end();) {
        iterator it_next = it_last;
        ++it_next;
        if (it_next.node_ == nullptr)
          return;
        else if (*it_last == *it_next)
          erase(it_next);
        else
          ++it_last;
      }
    }
  }

  /**
   * @brief Сортирует элементы в контейнере с использованием быстрой сортировки.
   *
   * @details Эта функция вызывает функцию `quick_sort` для сортировки всех
   * элементов в контейнере. Она использует итераторы `begin()` и `--end()` для
   * определения начала и конца диапазона, который нужно отсортировать, а также
   * `size_` для определения размера списка.
   *
   * @note Функция `quick_sort` реализована ниже в этом коде и использует
   * алгоритм быстрой сортировки для эффективной сортировки.
   */
  void sort() { quick_sort(begin(), --end(), size_); }

  /**
   * Вставляет новые элементы в контейнер непосредственно перед указанной
   * позицией.
   *
   * @param pos Итератор, указывающий позицию, перед которой будут вставлены
   * новые элементы.
   * @param args Переменное количество аргументов, представляющих элементы для
   * вставки.
   * @return Итератор, указывающий на первый вставленный элемент.
   *
   * Пример использования:
   * @code
   * s21::list<int> myList;
   * myList.insert_many(myList.begin(), 1, 2, 3);
   * @endcode
   */
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    iterator it_current{const_cast<node_type *>(pos.node_)};
    node_type *new_node;

    for (auto item : {std::forward<Args>(args)...}) {
      new_node = new node_type(std::move(item));
      it_current.node_->AttachPrev(new_node);
      ++size_;
    }

    return iterator(new_node);
  }

  /**
   * Добавляет новые элементы в конец контейнера.
   *
   * @param args Переменное количество аргументов, представляющих элементы для
   * добавления.
   *
   * Пример использования:
   * @code
   * s21::list<int> myList;
   * myList.insert_many_back(1, 2, 3);
   * @endcode
   */
  template <class... Args>
  void insert_many_back(Args &&...args) {
    (push_back(std::forward<Args>(args)), ...);
  }

  /**
   * Добавляет новые элементы в начало контейнера.
   *
   * @param args Переменное количество аргументов, представляющих элементы для
   * добавления.
   *
   * Пример использования:
   * @code
   * s21::list<int> myList;
   * myList.insert_many_front(1, 2, 3);
   * @endcode
   */
  template <class... Args>
  void insert_many_front(Args &&...args) {
    list tempList;

    (tempList.push_front(std::forward<Args>(args)), ...);

    splice(begin(), tempList);
  }

 private:
  /**
   * @brief Выполняет быструю сортировку на заданном диапазоне.
   *
   * @param left Итератор, указывающий на начало диапазона.
   * @param right Итератор, указывающий на конец диапазона.
   * @param list_size Размер списка, который нужно отсортировать.
   *
   * @details Функция использует алгоритм быстрой сортировки для сортировки
   * элементов в заданном диапазоне. Алгоритм быстрой сортировки работает путем
   * выбора "опорного" элемента из списка и разделения списка на две части:
   * элементы меньше опорного и элементы больше опорного. Затем функция
   * рекурсивно сортирует эти две части.
   */
  void quick_sort(iterator left, iterator right, size_type list_size) {
    // Проверяем, что диапазон не пустой и содержит более одного элемента
    if (left != right && list_size > 1) {
      // Инициализация итераторов для обмена значений и выбора опорного элемента
      iterator swap_it = left;
      iterator pivot_it = left;
      iterator tmp_left = left;
      iterator tmp_right = right;

      // Перемещаем опорный итератор на середину диапазона
      --swap_it;
      --pivot_it;
      size_type iShift = 0;

      // Выбираем опорный элемент
      while (iShift < list_size / 2) {
        ++pivot_it;
        ++iShift;
      }

      // Сохраняем значение опорного элемента
      value_type pivot = *pivot_it;

      // Сброс счетчика для обмена значений
      iShift = 0;

      // Обмен значений опорного элемента с последним элементом диапазона
      pivot_it.node_->SwapValues(right.node_);

      // Процесс разделения списка на две части
      while (tmp_left != tmp_right) {
        if (*tmp_left < pivot) {
          // Если элемент меньше опорного, обмениваем его с элементом на позиции
          // swap_it
          ++swap_it;
          ++iShift;
          tmp_left.node_->SwapValues(swap_it.node_);
          ++tmp_left;
        } else if (*tmp_left == pivot) {
          // Если элемент равен опорному, обмениваем его с элементом на позиции
          // tmp_right
          --tmp_right;
          tmp_left.node_->SwapValues(tmp_right.node_);
        } else {
          // Если элемент больше опорного, просто переходим к следующему
          // элементу
          ++tmp_left;
        }
      }

      // Подготовка к рекурсивным вызовам для левой и правой частей списка
      iterator next_step_left = swap_it;
      size_type next_step_left_size = iShift;
      size_type next_step_right_size = list_size - iShift - 1;

      // Обмен значений опорного элемента с элементом на позиции swap_it
      ++swap_it;
      while (tmp_right != right) {
        swap_it.node_->SwapValues(tmp_right.node_);
        ++swap_it;
        ++tmp_right;
        --next_step_right_size;
      }

      // Обмен значений опорного элемента с элементом на позиции tmp_right
      swap_it.node_->SwapValues(right.node_);
      ++swap_it;

      // Итератор для начала правой части списка
      iterator next_step_right = swap_it;

      // Рекурсивные вызовы для левой и правой частей списка
      quick_sort(left, next_step_left, next_step_left_size);
      quick_sort(next_step_right, right, next_step_right_size);
    }
  }

  struct ListNode {
    /**
     * @brief Конструктор по умолчанию для создания пустого узла.
     */
    ListNode() noexcept : next_(this), prev_(this), value_(value_type{}) {}
    /**
     * @brief Конструктор с параметром для создания узла с заданным значением.
     * @param value Значение для инициализации узла.
     */
    explicit ListNode(const_reference value) noexcept
        : next_(nullptr), prev_(nullptr), value_(value) {}
    /**
     * @brief Конструктор с параметром для создания узла с заданным значением,
     * используя move-семантику.
     * @param value Значение для инициализации узла.
     */
    explicit ListNode(value_type &&value) noexcept
        : next_(nullptr), prev_(nullptr), value_(std::move(value)) {}

    /**
     * @brief Встраивает узел new_node перед текущим узлом.
     * @param new_node Узел для встраивания.
     */
    void AttachPrev(node_type *new_node) noexcept {
      new_node->next_ = this;
      new_node->prev_ = prev_;
      prev_->next_ = new_node;
      prev_ = new_node;
    }
    /**
     * @brief Изымает узел из списка.
     */
    void UnAttach() noexcept {
      prev_->next_ = next_;
      next_->prev_ = prev_;
      next_ = this;
      prev_ = this;
    }
    /**
     * @brief Меняет значения текущего узла и узла other_node.
     * @param other_node Узел для обмена значениями.
     */
    void SwapValues(node_type *other_node) noexcept {
      std::swap(value_, other_node->value_);
    }
    /**
     * @brief Обменивает next_ и prev_ текущего узла.
     */
    void SwapNextPrev() noexcept { std::swap(next_, prev_); }
    node_type *next_;  // Указатель на следующий узел списка
    node_type *prev_;  // Указатель на предыдущий узел списка
    value_type value_;  // Значение узла списка
  };

  /**
   * @struct ListIterator
   * @brief Итератор для двунаправленного списка, позволяющий изменять значения
   * элементов списка.
   *
   * `ListIterator` предоставляет возможность обхода элементов списка в обоих
   * направлениях и изменять значения этих элементов. Итератор реализует
   * категорию итератора `std::bidirectional_iterator_tag`, что означает, что он
   * поддерживает операции инкремента и декремента.
   *
   * @note Итератор не проверяет валидность своего состояния, поэтому
   * пользователь должен быть осторожным при его использовании.
   *
   * @see ListIteratorConst
   */
  struct ListIterator {
    // Определение категории итератора как двунаправленный итератор.
    using iterator_category = std::bidirectional_iterator_tag;
    // Определение типа значения, на который указывает итератор, как тип
    // значения списка.
    using value_type = list::value_type;
    // Определение типа, используемого для представления разницы между двумя
    // итераторами.
    using difference_type = std::ptrdiff_t;
    // Определение типа указателя на значение, на которое указывает итератор.
    using pointer = value_type *;
    // Определение типа ссылки на значение, на которое указывает итератор.
    using reference = value_type &;

    // Конструктор по умолчанию удален для предотвращения создания
    // неинициализированного итератора.
    ListIterator() = delete;

    /**
     * @brief Конструктор итератора с указанием узла.
     * @param node Указатель на узел списка.
     * @note Итератор становится инициализированным и указывает на указанный
     * узел.
     */
    explicit ListIterator(node_type *node) noexcept : node_(node) {}

    /**
     * @brief Оператор разыменования для доступа к значению узла.
     * @return Ссылка на значение узла.
     */
    reference operator*() const noexcept { return node_->value_; }

    /**
     * @brief Префиксный инкремент итератора.
     * @return Ссылка на итератор после инкремента.
     */
    iterator &operator++() noexcept {
      node_ = node_->next_;
      return *this;
    }

    /**
     * @brief Постфиксный инкремент итератора.
     * @return Копия итератора до инкремента.
     */
    iterator &operator++(int) noexcept {
      iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    /**
     * @brief Префиксный декремент итератора.
     * @return Ссылка на итератор после декремента.
     */
    iterator &operator--() noexcept {
      node_ = node_->prev_;
      return *this;
    }

    /**
     * @brief Постфиксный декремент итератора.
     * @return Копия итератора до декремента.
     */
    iterator &operator--(int) noexcept {
      iterator tmp(node_);
      --(*this);
      return tmp;
    }

    /**
     * @brief Сравнение двух итераторов на равенство.
     * @param other Итератор для сравнения.
     * @return true, если итераторы указывают на один и тот же узел, иначе
     * false.
     */
    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    }

    /**
     * @brief Сравнение двух итераторов на неравенство.
     * @param other Итератор для сравнения.
     * @return true, если итераторы указывают на разные узлы, иначе false.
     */
    bool operator!=(const iterator &other) const noexcept {
      return node_ != other.node_;
    }

    // Указатель на текущий узел списка.
    node_type *node_;
  };

  /**
   * @struct ListIteratorConst
   * @brief Константный итератор для двунаправленного списка, не позволяющий
   * изменять значения элементов списка.
   *
   * `ListIteratorConst` предоставляет возможность обхода элементов списка в
   * обоих направлениях, но не позволяет изменять значения этих элементов. Это
   * делает его безопасным для использования в ситуациях, где необходимо
   * гарантировать неизменность данных.
   *
   * @note В отличие от `ListIterator`, `ListIteratorConst` не позволяет
   * изменять значения элементов списка, что делает его подходящим для
   * использования в функциях, которые должны быть читаемыми и не изменять
   * данные.
   *
   * @see ListIterator
   */
  struct ListIteratorConst {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = list::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    ListIteratorConst() = delete;

    explicit ListIteratorConst(const node_type *node) : node_(node) {}

    ListIteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->value_; }

    const_iterator &operator++() noexcept {
      node_ = node_->next_;
      return *this;
    }

    const_iterator &operator++(int) noexcept {
      const_iterator tmp(node_);
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() noexcept {
      node_ = node_->prev_;
      return *this;
    }

    const_iterator &operator--(int) noexcept {
      const_iterator tmp(node_);
      --(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ != it2.node_;
    }

    const node_type *node_;
  };

  // Указатель на голову списка
  node_type *head_;
  // Размерность списка
  size_type size_;
};

}  // namespace s21

#endif