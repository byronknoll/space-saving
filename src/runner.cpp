#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "hasher.h"
#include "space-saving.h"
#include "hash-map.h"

int FirstPass(char* filename, Hasher* hasher, SpaceSaving* space_saving) {
  unsigned int buffer_size = 104857600;  // 100MiB
  std::ifstream is;
  is.open(filename, std::ios::binary);
  if (!is.is_open()) {
    std::cout << "Invalid file name." << std::endl;
    return -1;
  }
  is.seekg(0, std::ios::end);
  unsigned long long len = is.tellg();
  if (len < buffer_size) buffer_size = len;
  char* buffer = new char[buffer_size];
  is.seekg(0, std::ios::beg);
  unsigned long long pos = 0;
  while (pos < len) {
    is.read(buffer, buffer_size);
    unsigned long long lim = buffer_size;
    if (pos + buffer_size > len) lim = len - pos;
    for (unsigned long long i = 0; i < lim; ++i) {
      space_saving->Process(hasher->Hash(buffer[i]));
    }
    pos += buffer_size;
  }
  is.close();
  delete[] buffer;
  return 0;
}

int SecondPass(char* filename, Hasher* hasher, SpaceSaving* space_saving, const long long& output_counters) {
  HashMap* map = new HashMap(output_counters, 0.75);
  space_saving->ExtractTop(output_counters, map);
  delete space_saving;
  space_saving = new SpaceSaving(map);
  char** ngrams = new char*[map->size_];
  for (unsigned long long i = 0; i < map->size_; ++i) {
    ngrams[i] = NULL;
  }
  hasher->Clear();
  unsigned int buffer_size = 104857600;  // 100MiB
  std::ifstream is;
  is.open(filename, std::ios::binary);
  if (!is.is_open()) {
    std::cout << "Invalid file name." << std::endl;
    return -1;
  }
  is.seekg(0, std::ios::end);
  unsigned long long len = is.tellg();
  if (len < buffer_size) buffer_size = len;
  char* buffer = new char[buffer_size];
  is.seekg(0, std::ios::beg);
  unsigned long long pos = 0;
  unsigned long long index;
  while (pos < len) {
    is.read(buffer, buffer_size);
    unsigned long long lim = buffer_size;
    if (pos + buffer_size > len) lim = len - pos;
    for (unsigned long long i = 0; i < lim; ++i) {
      if (map->Find(hasher->Hash(buffer[i]), &index)) {
        if (ngrams[index] == NULL) {
          ngrams[index] = hasher->GetString();
        }
        space_saving->Increment(map->values_[index]);
      }
    }
    pos += buffer_size;
  }
  is.close();
  delete[] buffer;
  space_saving->Print(ngrams);
  for (unsigned long long i = 0; i < map->size_; ++i) {
    if (ngrams[i] != NULL) delete[] ngrams[i];
  }
  delete[] ngrams;
  delete hasher;
  delete space_saving;
  return 0;
}

int main (int argc, char* argv[]) {
  if (argc != 5) {
    std::cout << "Four command-line arguments expected:" << std::endl;
    std::cout << "(1) file name" << std::endl;
    std::cout << "(2) context size (integer > 0)" << std::endl;
    std::cout << "(3) number of internal counters (integer > 0)" << std::endl;
    std::cout << "(4) number of output counters (integer > 0 and <= number of internal buckets)" << std::endl;
    return -1;
  }
  long long context_size = atol(argv[2]);
  long long internal_counters = atol(argv[3]);
  long long output_counters = atol(argv[4]);
  if (context_size <= 0 || internal_counters <= 0 || output_counters <= 0 || output_counters > internal_counters) {
    std::cout << "Invalid argument." << std::endl;
    return -1;
  }
  Hasher* hasher = new Hasher(context_size);
  SpaceSaving* space_saving = new SpaceSaving(internal_counters);
  int result = FirstPass(argv[1], hasher, space_saving);
  if (result != 0) return result;
  return SecondPass(argv[1], hasher, space_saving, output_counters);
}
