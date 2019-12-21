#include <vector>
#include <utility>

#include "gameObj.h"

typedef std::vector<bool (*)(gameObj*)> animVector;
typedef std::pair<animVector, int> animPair;
