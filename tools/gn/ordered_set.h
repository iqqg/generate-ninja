// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_ORDERED_SET_H_
#define TOOLS_GN_ORDERED_SET_H_

#include <stddef.h>

#include <deque>
#include <unordered_set>

// An ordered set of items. Only appending is supported. Iteration is designed
// to be by index.
template<typename T>
class OrderedSet {
 private:
  typedef std::unordered_set<T, BASE_HASH_NAMESPACE::hash<T> > set_type;
  typedef typename set_type::const_iterator set_iterator;
  typedef std::deque<set_iterator> vector_type;

 public:
  static const size_t npos = static_cast<size_t>(-1);

  OrderedSet() {}
  ~OrderedSet() {}

  const T& operator[](size_t index) const {
    return *ordering_[index];
  }
  size_t size() const {
    return ordering_.size();
  }
  bool empty() const {
    return ordering_.empty();
  }

  bool has_item(const T& t) const {
    return set_.find(t) != set_.end();
  }

  // Returns true if the item was inserted. False if it was already in the
  // set.
  bool push_back(const T& t) {
    std::pair<set_iterator, bool> result = set_.insert(t);
    if (result.second)
      ordering_.push_back(result.first);
    return result.second;
  }

  // Returns true if the item was inserted. False if it was already in the
  // set.
  bool push_front(const T& t) {
    std::pair<set_iterator, bool> result = set_.insert(t);
    if (result.second)
      ordering_.push_front(result.first);
    return result.second;
  }

  // Appends a range of items, skipping ones that already exist.
  template<class InputIterator>
  void append(const InputIterator& insert_begin,
              const InputIterator& insert_end) {
    for (InputIterator i = insert_begin; i != insert_end; ++i) {
      const T& t = *i;
      push_back(t);
    }
  }

  // Appends all items from the given other set.
  void append(const OrderedSet<T>& other) {
    for (size_t i = 0; i < other.size(); i++)
      push_back(other[i]);
  }

  // Pre-appends a range of items, skipping ones that already exist.
  template<class InputIterator>
  void preappend(const InputIterator& insert_begin,
                 const InputIterator& insert_end) {
    InputIterator i = insert_end;
    while (i != insert_begin) {
      const T& t = *--i;
      push_front(t);
    }
  }

  // Pre-appends all items from the given other set.
  void preappend(const OrderedSet<T>& other) {
    for (size_t i = other.size(); i > 0; i--)
      push_front(other[i-1]);
  }

 private:
  set_type set_;
  vector_type ordering_;
};

#endif  // TOOLS_GN_ORDERED_SET_H_
