/* Set class header file
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */
#include "block.h"
#include "set.h"
#include "stdlib.h"
#include "math.h"
#include <vector>
#include <algorithm>

Set::Set(unsigned int num_blocks) {
  this->num_blocks = num_blocks;
  for (unsigned int i = 0; i < num_blocks; i++) {
    this->set.push_back(nullptr); //populates the set with nullptr
  }
}

Set::~Set() {
  int size = set.size();
  for (int i = 0; i < size; i++) {
    delete set[i];
  }
  set.clear();
}

void Set::updateTime(int index, unsigned int num_blocks) {
  set[index]->setTime(0);
  for (int i = 0; i < (int) num_blocks; i++) {
    if (i != index) {
      set[i]->setTime(set[i]->getTime() + 1);
    }
  }
}

int Set::fifo_eviction(unsigned int num_blocks) {
  for (int i = 0; i < (int) num_blocks; i++) {
    if (set[i]->getTime() == (int)num_blocks - 1) {
      return i;
    }
  }
  return 0;
}


void Set::updateBits(int index, unsigned int num_blocks) {
  set[index]->setBits(0);
  for (int i = 0; i < (int)num_blocks; i++) {
    if (i != index) {
      set[i]->setBits(set[i]->getBits() + 1);
    }
  }
}

int Set::lru_eviction(unsigned int num_blocks) { //returns which column to evict according to fifo rule
  std::vector<int> a;
  for (int i = 0; i < (int)num_blocks; i++) {
    a.push_back(set[i]->getBits());
  }
  int max_index = std::max_element(a.begin(),a.end()) - a.begin();
  a.clear();
  return max_index;
}

Block * Set::getBlock(int index) {
  return set[index];
}

void Set::setBlock(int index, Block * block) {
  set[index] = block;
}


