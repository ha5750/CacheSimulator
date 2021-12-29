/* Main Source File
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */

#include <iostream>
#include <sstream>
#include <cstring>
#include "cache.h"
#include <assert.h>
#include "set.h"
#include "block.h"

//two helper math functions
bool ispoweroftwo(unsigned int x) {
  return (x & (x - 1)) == 0;
}
int two_exp(unsigned int x) {
  int count = 0;
  while (x != 1) {
    x = x >> 1;
    count++;
  }
  return count;
}

//main function
int main(int argc, char *argv[]) {
  if ( (argc < 6) || (argc > 7) ) { //six or seven arguments
    std::cerr << "Error: Invalid command-line arguments" << std::endl;
    return 1;
  }
  
  unsigned int num_sets; //number of sets in the cache, must be power of 2
  unsigned int num_blocks; //number of blocks in each set, must be power of 2
  unsigned int num_bytes; //num bytes per block, must be at least 4, and posivitve power of 2
 
  std::stringstream s1(argv[1]);
  std::stringstream s2(argv[2]);
  std::stringstream s3(argv[3]);

  s1 >> num_sets;
  s2 >> num_blocks;
  s3 >> num_bytes;
  
  if (!(ispoweroftwo(num_sets) && ispoweroftwo(num_blocks) && ispoweroftwo(num_bytes))) {
    std::cerr << "Error: number of sets or blocks or bytes is not power of two" << std::endl;
    return 1;
  }
  if (num_bytes < 4) {
    std::cerr << "Error: number of bytes have to be at least 4" << std::endl;
    return 1;
  }
  
  bool write_allocate; //bool no_write_allocate;
  bool write_through; //bool write_back;
  bool lru = false; bool fifo = false;
  if (0 == strcmp("write-allocate", argv[4])) {
    write_allocate = true;
  } else if (0 == strcmp("no-write-allocate", argv[4])){
    write_allocate = false;
  } else {
    std::cerr << "Error: fourth argumenet is invalid" << std::endl;
  }
  if (0 == strcmp("write-through", argv[5])) {
    write_through = true;
  } else if (0 == strcmp("write-back", argv[5])){
    write_through = false;
  } else {
    std::cerr << "Error: fifth argumenet is invalid" << std::endl;
    return 1;
  }
  if (!(write_allocate) && !(write_through)) {
    std::cerr << "Error: no-write-allocate and write-back is non-sense" << std::endl;
    return 1;
  }
  if (num_blocks != 1) { //if not direct mapped
    if (0 == strcmp("lru", argv[6])) {
      lru = true;
    } else if (0 == strcmp("fifo", argv[6])) {
      fifo = true;
    } else {
      std::cerr << "Error: sixth argument is invald" << std::endl;
      return 1;
    }
  }

  //reading in from the command-line arguments and error checking them is finished

  //construct cache
  Cache * cache = new Cache(num_bytes, write_allocate, write_through, lru, fifo);
  cache->create_cache_vec(num_sets, num_blocks);

  //read standard input
  bool load = true;
  std::string line; //parse
  while (std::getline(std::cin, line)) { //reads cin line by line until eof
    std::stringstream s(line); //string line is put into stream s
    unsigned int index; unsigned int tag; unsigned int hexaddress;
    std::string format; std::string address; std::string junk;
    s >> format; s >> address; s >> junk;
    
    if ("s" == format) {
      load = false;
    } else if ("l" == format) {
      load = true;
    } else {
      std::cerr << "Error: invalid load or store character." << std::endl;
    }
    assert(address.size() == 10);

    if (address == "0x00000000") {
      hexaddress = 0;
    } else {
      assert(hexaddress = std::stoul(address, nullptr, 16));
    }
    
    tag = hexaddress >> (two_exp(num_sets) + two_exp(num_bytes));
    int tag_len = 32 - two_exp(num_sets) - two_exp(num_bytes);
    assert(tag >> tag_len == 0);
    
    index = ((hexaddress << tag_len) >> tag_len) >> two_exp(num_bytes);
    assert(index >> two_exp(num_sets) == 0);
    //now tag and index value was found
    
    if (load) {
      cache->load(index, tag, num_blocks);
    } else { //store
      cache->store(index, tag, num_blocks);
    }
  }

  std::cout << "Total loads: " << cache->get_load_total() << std::endl;
  std::cout << "Total stores: " << cache->get_store_total() << std::endl;
  std::cout << "Load hits: " << cache->get_load_hit() << std::endl;
  std::cout << "Load misses: " << cache->get_load_miss() << std::endl;
  std::cout << "Store hits: " << cache->get_store_hit() << std::endl;
  std::cout << "Store misses: " << cache->get_store_miss() << std::endl;
  std::cout << "Total cycles: " << cache->get_total_cycles() << std::endl;

  delete cache;
  
  return 0;
}

  


  

  
  
 
