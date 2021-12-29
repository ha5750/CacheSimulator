/* Cache class header file
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */
#ifndef CACHE_H
#define CACHE_H

#include <vector>
class Block;
class Set;
class Cache {
 private:

  unsigned int num_bytes;

  bool write_allocate;
  bool write_through;
  bool lru;
  bool fifo;

  int l_total;
  int s_total;
  int s_hit;
  int s_miss;
  int l_hit;
  int l_miss;
  int total_cycles;
  std::vector<Set *> cache_vec;
    
 public:
  Cache(unsigned int num_bytes, bool write_allocate, bool write_through, bool lru, bool fifo);
  ~Cache();

  void create_cache_vec(unsigned int num_sets, unsigned int num_blocks);
  
  int load(unsigned int index, unsigned int tag, unsigned int num_blocks); 
  int store(unsigned int index, unsigned int tag, unsigned int num_blocks);
  
  int get_store_hit();
  int get_store_miss();
  int get_load_hit();
  int get_load_miss();
  int get_store_total();
  int get_load_total();
  int get_total_cycles();
  
};

#endif
