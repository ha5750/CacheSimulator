/* Block class header file
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */
#ifndef BLOCK_H
#define BLOCK_H

class Block { //block class will have information regarding tag / validity / timestamp(FIFO) / n-1 bit (LRU)  

 private:
  
  bool valid;
  unsigned int tag;
  int time;
  unsigned int bits;
  int dirty;

 public:

  Block(unsigned int tag, bool valid, int time, unsigned int bits, int dirty);

  bool getValid();
  unsigned int getTag();
  int getTime();
  int getBits();
  int getDirty();
  void setValid(bool valid);
  void setTag(long tag);
  void setTime(int time);
  void setBits(int bits);
  void setDirty(int dirty);
  ~Block(){
  }
};


#endif
