#include "Node.h"
#include "database.h"


typedef struct Solution Solution;

struct Solution {
    std::vector<int> path;
    int cost;
};


extern Solution IDdfs(Node,int);

extern Solution IDAstar(Rubikscube,database);

extern void print_solution(Solution);