#include "includes/client/engine/engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

// add main entry
int main()
{
    srand(time(NULL));
    Engine engine;
    engine.run();
    return 0;
}