#pragma once

template <typename T, size_t SIZE>
class CircularBuffer
{
public:
  CircularBuffer()
    : m_size(0), m_end(0), m_head(0)
  {}

  /* Adds a new element of T to the end of the list
   * Returns True if successfully added otherwise False
   */
  bool Push(T t) {
    if (this->Full()) {
      return false;
      //or just Pop!
    }

    m_data[m_end++] = t;

    if(m_end == SIZE)
      m_end = 0;

    ++m_size;
    return true;
  }


  /* Removes the oldest element
   * Returns True if successfully removed otherwise False
   */
  bool Pop(T& e) {
    if (this->Empty()) {
      return false;
    }

    e = m_data[m_head];

    if (++m_head == SIZE)
      m_head = 0;

    --m_size;
    return true;
  }

  /* Fetches the index value into `t`.
   * If the index is not valid it returns false otherwise it returns true.
   */
  bool Peek(const size_t& index, T& t) const {
    if (index >= this->Size())
      return false;

    t = m_data[this->RelativeIndex(index)];
    return true;
  }

  //Peek without index checking
  inline
  void UnsafePeek(const size_t& index, T& t) const {
    t = m_data[this->RelativeIndex(index)];
  }

  /*
   * Returns the Current number of keeping elements.
   */
  size_t Size() const {
    return m_size;
  }

  /*
   * Returns true if it can't add new elements otherwise false.
   */
  inline
  bool Full() const {
    return this->Size() == SIZE;
  }

  /*
   * Returns true if it doesn't contain any value.
   */
  inline
  bool Empty() const {
    return !m_size;
  }

  /*
   * Total capacity of the container.
   */
  constexpr
  size_t MaxSize() const {
    return SIZE;
  }

private:
  size_t RelativeIndex(const size_t &index) const {
    const size_t relativeIndex = m_head + index;
    const ssize_t leftOut = relativeIndex - SIZE;

    if (leftOut >= 0)
      return leftOut;

    return relativeIndex;
  }

private:
  size_t m_head;
  size_t m_end;
  size_t m_size;
  T m_data[SIZE];
};
