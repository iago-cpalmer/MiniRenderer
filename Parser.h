#ifndef PARSER_H
#define PARSER_H

#include "Vector3f.h"
#include "Vector3Int.h"
#include "Vector2f.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <math.h>
#include "Model.h"
#include <algorithm>

Model* ParseObj(std::string filePath);

#endif