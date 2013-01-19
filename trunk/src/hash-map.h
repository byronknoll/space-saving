#ifndef HASH_MAP_H
#define HASH_MAP_H

class Child;

class HashMap {
 public:
  HashMap(const unsigned long long& buckets, const double& load_factor);
  ~HashMap();
  void Insert(const unsigned long long& key, Child* value);
  void Remove(const unsigned long long& key);
  bool Find(const unsigned long long& key, unsigned long long* index);
  unsigned long long* keys_;
  Child** values_;
  unsigned long long size_;
  long long half_;
};

#endif
