#include <cstddef>
#include <iostream>

#include "space-saving.h"

Parent::Parent() : left_(NULL), right_(NULL), child_(NULL), value_(0), size_(0) {}

void Parent::Add(Child* c) {
  c->parent_ = this;
  ++size_;
  if (size_ == 1) {
    child_ = c;
    c->left_ = c;
    c->right_ = c;
    return;
  }
  c->right_ = child_;
  c->left_ = child_->left_;
  child_->left_->right_ = c;
  child_->left_ = c;
}

Child::Child() : parent_(NULL), left_(NULL), right_(NULL), element_(0) {}

void Child::Detach(Parent** smallest) {
  if (parent_->size_ == 1) {
    if (parent_ == *smallest) {
      *smallest = parent_->right_;
    }
    parent_->right_->left_ = parent_->left_;
    if (parent_->left_ != NULL) {
      parent_->left_->right_ = parent_->right_;
    }
    delete parent_;
    return;
  }
  if (parent_->child_ == this) {
    parent_->child_ = right_;
  }
  left_->right_ = right_;
  right_->left_ = left_;
  --(parent_->size_);
}

SpaceSaving::SpaceSaving(const unsigned long long& num_counters) {
  map_ = new HashMap(num_counters, 0.75);
  smallest_ = new Parent();
  largest_ = smallest_;
  for (unsigned long long i = 0; i < num_counters; ++i) {
    smallest_->Add(new Child());
  }
}

SpaceSaving::SpaceSaving(HashMap* map) {
  map_ = map;
  smallest_ = new Parent();
  largest_ = smallest_;
  for (unsigned long long i = 0; i < map->size_; ++i) {
    if (map->keys_[i] != 0) {
      smallest_->Add(map->values_[i]);
    }
  }
}

SpaceSaving::~SpaceSaving() {
  for (unsigned long long i = 0; i < map_->size_; ++i) {
    if (map_->keys_[i] != 0) {
      delete map_->values_[i];
    }
  }
  while (smallest_ != NULL) {
    Parent* tmp = smallest_->right_;
    delete smallest_;
    smallest_ = tmp;
  }
  delete map_;
}

void SpaceSaving::Increment(Child* bucket) {
  unsigned long long count = bucket->parent_->value_ + 1;
  Parent* next = bucket->parent_->right_;
  if (next != NULL && next->value_ == count) {
    bucket->Detach(&smallest_);
    next->Add(bucket);
  } else if (bucket->parent_->size_ == 1) {
    bucket->parent_->value_ = count;
  } else {
    bucket->Detach(&smallest_);
    Parent* p = new Parent();
    p->left_ = bucket->parent_;
    p->value_ = count;
    bucket->parent_->right_ = p;
    if (next != NULL) {
      p->right_ = next;
      next->left_ = p;
    } else {
      largest_ = p;
    }
    p->Add(bucket);
  }
}

void SpaceSaving::Process(const unsigned long long& element) {
  unsigned long long index;
  if (!map_->Find(element, &index)) {
    Child* bucket = smallest_->child_;
    map_->Remove(bucket->element_);
    bucket->element_ = element;
    map_->Insert(element, bucket);
    Increment(bucket);
  } else {
    Increment(map_->values_[index]);
  }
}

void SpaceSaving::ExtractTop(const unsigned long long output_counters, HashMap* map) {
  unsigned long long count = 0;
  while(true) {
    Child* c = largest_->child_;
    do {
      Child* c2 = new Child();
      c2->element_ = c->element_;
      map->Insert(c->element_, c2);
      ++count;
      if (count == output_counters) break;
      c = c->right_;
    } while (c != largest_->child_);
    if (count == output_counters) break;
    largest_ = largest_->left_;
    if (largest_->value_ == 0) break;
  }
}

void SpaceSaving::Print(char** ngrams) {
  unsigned long long index;
  while (largest_ != NULL) {
    Child* c = largest_->child_;
    do {
      map_->Find(c->element_, &index);
      c = c->right_;
      std::cout << ngrams[index] << "\t" << largest_->value_ << std::endl;
    } while (c != largest_->child_);
    largest_ = largest_->left_;
  }
}
