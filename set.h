/* Set class header file
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */
#ifndef SET_H
#define SET_H

#include <vector>

class Block;

//Set Class will contain a vector of blocks, which represent a single row given by index of a cache
class Set {

private:
  std::vector<Block*> set; 
  unsigned int num_blocks; 

public:
  Set(unsigned int num_blocks);
  ~Set();
  
  Block * getBlock(int index); //returns a block at a specified index < num_blocks

  //functions regarding eviction strategy
  void updateTime(int index, unsigned int num_blocks);
  int fifo_eviction(unsigned int num_blocks);
  void updateBits(int index, unsigned int num_blocks);
  int lru_eviction(unsigned int num_blocks);

  void setBlock(int index, Block * block);
  
  
};


#endif
