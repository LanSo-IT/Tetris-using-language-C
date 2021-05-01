#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>
#include "shapes_type.h"

typedef struct Block Block;

struct Block{
  enum ShapeType type;
  bool active;
  bool center;
  int row;
  int column;
  //Coordinate coordinate[4];
};

#endif
