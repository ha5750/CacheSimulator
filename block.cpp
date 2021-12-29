
/* Block class implementation
 * CSF Assignment 3
 * Chanha Kim
 * ckim135@jhu.edu
 */

#include "block.h"

Block::Block(unsigned int tag, bool valid, int time, unsigned int bits, int dirty) {
    this->tag = tag;
    this->valid = valid;
    this->time = time;
    this->bits = bits;
    this->dirty = dirty;
}

bool Block::getValid() {
    return valid;
}
unsigned int Block::getTag() {
    return tag;
}
int Block::getTime() {
    return time;
}
int Block::getBits() {
    return bits;
}
int Block::getDirty() {
    return dirty;
}
void Block::setValid(bool valid) {
    this->valid = valid;
}
void Block::setTag(long tag) {
    this->tag = tag;
}
void Block::setTime(int time) {
    this->time = time;
}
void Block::setBits(int bits) {
    this->bits = bits;
}
void Block::setDirty(int dirty) {
    this->dirty = dirty;
}
