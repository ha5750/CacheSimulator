/* Cache class implementation file
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */
#include "cache.h"
#include "set.h"
#include "stdlib.h" //generating random number
#include "block.h"
#include "assert.h"

//Cache class contains a vector of set classes, completing the cache structure
Cache::Cache(unsigned int num_bytes, bool write_allocate, bool write_through, bool lru, bool fifo) {
  this->write_allocate = write_allocate;
  this->write_through = write_through;
  this->lru = lru;
  this->fifo = fifo;
  this->num_bytes = num_bytes;
  s_hit = 0;
  s_miss = 0;
  l_hit = 0;
  l_miss = 0;
  s_total = 0;
  l_total = 0;
  total_cycles = 0;
}

Cache::~Cache() {
  int size = cache_vec.size();
  for (int i = 0; i < size; i++) {
    delete cache_vec[i];
  }
  cache_vec.clear();
}



void Cache::create_cache_vec(unsigned int num_sets, unsigned int num_blocks) { //vector of sets  
  unsigned int rows = num_sets;
  unsigned int cols = num_blocks;
  for (unsigned int i = 0; i < rows; i++) {
    Set * set = new Set(cols); //initialize new set
    for (unsigned int j = 0; j < cols; j++) {
      Block * block = new Block(0,0,int(num_blocks)-1-j,0,0); //tag, valid, time, bits, dirty
      set->setBlock(j, block);
    }
    this->cache_vec.push_back(set);
  }
}

int Cache::load(unsigned int index, unsigned int tag, unsigned int num_blocks) {
  l_total++;
  Block * block;
  for (unsigned int i = 0; i < num_blocks; i++) {
    block = cache_vec[index]->getBlock(i);
    if (block->getTag() == tag && block->getValid() == 1) {
      l_hit++;
      total_cycles++;
      cache_vec[index]->updateBits(i, num_blocks); //for lru
      return 1;
    }
  }

  l_miss++;
  //choose a block in the specific set to fill the cache
  for (unsigned int i = 0; i < num_blocks; i++) {
    block = cache_vec[index]->getBlock(i);
    if (block->getValid() == 0) { //found a cache space to fill
      total_cycles = total_cycles + 100 * (int)num_bytes/4; //read the data from main memory
      total_cycles++; //store it to cache
      block->setTag(tag);
      block->setValid(1);
      cache_vec[index]->updateTime(i, num_blocks);
      cache_vec[index]->updateBits(i, num_blocks);
      return 0; 
    }
  }
  //cache_vec[index] is filled alreay -> must evict a block
  //find which block to be evicted
  int evict_col;
  if (this->lru) {
    evict_col = cache_vec[index]->lru_eviction(num_blocks); //returns the location of which block to evict
  } else if (this->fifo) {
    evict_col = cache_vec[index]->fifo_eviction(num_blocks);
  } else { //direct mapped
    evict_col = rand() % num_blocks;
  }
  block = cache_vec[index]->getBlock(evict_col); //block to be evicted
  if (!this->write_through) { //if write-back, check if the block being evicted is DIRTY
    if (block->getDirty() == 1) { //if dirty
      total_cycles = total_cycles + 100 * (int)num_bytes/4;
    }
  }
  
  block->setTag(tag); //gets new tag
  block->setDirty(0);
  cache_vec[index]->updateTime(evict_col, num_blocks);
  cache_vec[index]->updateBits(evict_col, num_blocks);
  total_cycles++;
  total_cycles = total_cycles + 100 * (int)num_bytes/4; //read data from main memory
  return 0;
}

int Cache::store(unsigned int index, unsigned int tag, unsigned int num_blocks) {
  s_total++;
  Block * block;
  for (unsigned int i = 0; i < num_blocks; i++) {
    block = cache_vec[index]->getBlock(i);
    if (block->getTag() == tag && block->getValid() == 1) { //store hit
      s_hit++;
      cache_vec[index]->updateBits(i, num_blocks);
      if (!this->write_through) { //if write-back
	//writes to cache only
	block->setDirty(1);
	total_cycles++;
      } else { //write_through
	total_cycles = total_cycles + 100; //updates to cache results in update in main memory
      }
      return 1;
    }
  }

  s_miss++;
  if (!this->write_allocate) { //no-write allocate, write directly to memory 500
    total_cycles = total_cycles + 100;
    return 0;
  } else { //write_allocate
    for (unsigned int i = 0; i < num_blocks; i++) {
      block = cache_vec[index]->getBlock(i);
      if (block->getValid() == 0) {
	total_cycles = total_cycles + 100 * (int) num_bytes / 4; //must load a data from main mem to cache
	block->setValid(1);
	block->setTag(tag);
	cache_vec[index]->updateTime(i, num_blocks);
        cache_vec[index]->updateBits(i, num_blocks);
	if (!this->write_through) { //if write back,set dirty bit
	  total_cycles++;
	  block->setDirty(1);
	} else {
	  total_cycles = total_cycles + 100;
	}
	return 0;
      }
    }
  }
  
  //handle the case where all blocks are occupied
  int evict_col;
  if (this->lru) {
    evict_col = cache_vec[index]->lru_eviction(num_blocks); //returns the location of which block to evict
  } else if (this->fifo) {
    evict_col = cache_vec[index]->fifo_eviction(num_blocks);
  } else { //direct mapped
    evict_col = rand() % num_blocks;
  }

  block = cache_vec[index]->getBlock(evict_col); //a chosen block to be evicted 

  if (!this->write_through) { //if write-back, check if the block being evicted is DIRTY
    if (block->getDirty() == 1) { //if dirty, write to main memory
      total_cycles = total_cycles + 100 * (int) num_bytes /4;
    }
  }

  total_cycles = total_cycles + 100 * (int) num_bytes / 4; //load data from memory
  cache_vec[index]->updateTime(evict_col, num_blocks);
  cache_vec[index]->updateBits(evict_col, num_blocks);
  block->setTag(tag);
  if (!this->write_through) {
    block->setDirty(1);
    total_cycles++;
  } else {
    total_cycles = total_cycles + 100;
  }
  return 0;
}

int Cache::get_store_hit() {
  return s_hit;
}
int Cache::get_store_miss() {
  return s_miss;
}
int Cache::get_store_total() {
  return s_total;
}
int Cache::get_load_hit() {
  return l_hit;
}
int Cache::get_load_miss() {
  return l_miss;
}
int Cache::get_load_total() {
  return l_total;
}
int Cache::get_total_cycles() {
  return total_cycles;
}












