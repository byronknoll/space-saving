#include "hasher.h"

const unsigned int kPower = 31;

Hasher::Hasher(const int& context_size) : context_pos_(0), context_size_(context_size), hash_(0) {
  context_ = new unsigned char[context_size];
  pow_ = 1;
  for (unsigned int i = 0; i < context_size_; ++i) {
    pow_ *= kPower;
    context_[i] = 0;
  }
}

Hasher::~Hasher() {
  delete[] context_;
}

unsigned long long Hasher::Hash(const unsigned char& c) {
  hash_ = hash_ * kPower + c;
  unsigned char old_val = context_[context_pos_];
  context_[context_pos_] = c;
  ++context_pos_;
  context_pos_ %= context_size_;
  hash_ -= pow_ * old_val;
  return hash_;
}

char* Hasher::GetString() {
  char* str = new char[context_size_ + 1];
  for (unsigned int i = 0; i < context_size_; ++i) {
    str[i] = context_[(i + context_pos_) % context_size_];
  }
  str[context_size_] = '\0';
  return str;
}

void Hasher::Clear() {
  context_pos_ = 0;
  hash_ = 0;
  for (unsigned int i = 0; i < context_size_; ++i) {
    context_[i] = 0;
  }
}
