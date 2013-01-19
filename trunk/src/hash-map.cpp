#include "hash-map.h"

HashMap::HashMap(const unsigned long long& buckets, const double& load_factor) {
  size_ = 10 + (buckets / load_factor);
  keys_ = new unsigned long long[size_];
  values_ = new Child*[size_];
  half_ = size_ / 2;
  for (unsigned long long i = 0; i < size_; ++i) {
    keys_[i] = 0;
  }
}

HashMap::~HashMap() {
  delete[] keys_;
  delete[] values_;
}

void HashMap::Insert(const unsigned long long& key, Child* value) {
  unsigned long long i = key % size_;
  while (keys_[i] != 0) {
    i = (i == size_ - 1) ? 0 : i + 1;
  }
  keys_[i] = key;
  values_[i] = value;
}

void HashMap::Remove(const unsigned long long& key) {
  unsigned long long i = key % size_;
  while (keys_[i] != key) {
    i = (i == size_ - 1) ? 0 : i + 1;
  }
  keys_[i] = 0;
  // Fill in the deletion gap.
  unsigned long long j = (i == size_ - 1) ? 0 : i + 1;
  while (keys_[j] != 0) {
    long long dis = (keys_[j] % size_) - i;
    if (dis < -half_) dis += size_;
    if (dis > half_) dis -= size_;
    if (dis <= 0) {
      keys_[i] = keys_[j];
      values_[i] = values_[j];
      keys_[j] = 0;
      i = j;
    }
    j = (j == size_ - 1) ? 0 : j + 1;
  }
}

bool HashMap::Find(const unsigned long long& key, unsigned long long* index) {
  unsigned long long i = key % size_;
  while (true) {
    if (!keys_[i]) {
      return false;
    }
    if (keys_[i] == key) {
      *index = i;
      return true;
    }
    i = (i == size_ - 1) ? 0 : i + 1;
  }
  return false;
}
