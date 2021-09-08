#include "IDA.h"

using namespace std;

int main() {

    database rpd;
    rpd.initializeAllWithFiles(); 
    
    Rubikscube cube;
    cube.random();  

    printf("\n\n-----------------------------------------------\n");
    printf("Solving this random Rubik's cube configuration: \n");
    cube.print();
    printf("-----------------------------------------------\n");

    Solution solution;
    solution = IDAstar(cube, rpd);
    print_solution(solution);

    
}
