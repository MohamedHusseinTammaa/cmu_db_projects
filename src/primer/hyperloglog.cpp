//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"
#include <bitset>
namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0) {
  if(n_bits<0)number_of_bits=0;
  else number_of_bits=n_bits;
  number_of_buckets=std::pow(2,number_of_bits);
  registers=std::vector<uint64_t>(number_of_buckets,0);
}

/**
 * @brief Function that computes binary.
 *
 * @param[in] hash
 * @returns binary of a given hash
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  std::bitset<64>b(hash);
  return b;
}

/**
 * @brief Function that computes leading zeros.
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
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

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
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

/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  double upper = std::pow(number_of_buckets,2);
  double sum =0;
  for(int i=0;i<number_of_buckets;i++){
    sum += std::pow(2.0,-static_cast<double>(registers[i]));
  }
  double lower =sum;
  cardinality_=std::floor(CONSTANT*upper/lower); 

}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
