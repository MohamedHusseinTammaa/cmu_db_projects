//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.cpp
//
// Identification: src/primer/hyperloglog_presto.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog_presto.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits) : cardinality_(0) {  
  if(n_bits<0)number_of_bits=0;
  else number_of_bits=n_bits;
  number_of_buckets=std::pow(2,number_of_bits);
  dense_bucket_=std::vector<int4_t>(number_of_buckets,0);
}

auto Hyp<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  uint64_t counter=0;
  for (int i = 63; i >= 0; i--) {
    if (bset[i]) {
      break;
    }
    counter++;
  }

  return counter+1;
}
/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  std::lock_guard<std::mutex> lock(mtx_);
  hash_t hash = CalculateHash(val);
  uint64_t bucket_index ;
  uint64_t remaining;
  if (number_of_bits==0){
    bucket_index=0;
    remaining = hash;
  }else {
    bucket_index = hash >> (64 - number_of_bits);
    remaining = (hash <<number_of_bits);
  }
  auto bits = ComputeBinary(remaining);
  auto counter = PositionOfLeftmostOne(bits);
  registers[bucket_index]=std::max(registers[bucket_index],counter);
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
}


template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub
