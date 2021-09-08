#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include "IDA.h"
#include "Node.h"
#include "database.h"

using namespace std;

database db;


//iterative dfs
Solution IDdfs(Node node, int limit) {
    int gcost = node.cost;
    int hcost = db.heuristic(node.state);

    if (gcost + hcost > limit) {
        Solution sol;
        sol.cost = gcost + hcost;
        return sol;
    }

    if (node.isSolved()) {
        Solution sol;
        sol.path = node.extractSolution();
        sol.cost = gcost;
        return sol;
    }

    int newlimit = INT_MAX;
    int number_children = node.numberOfChildren();
    int i;
    for (i = 0; i < number_children; i++) {
        Node node1 = node.getChild(i);
        Solution sol = IDdfs(node1, limit);
        if (! sol.path.empty()) 
            return sol;

        newlimit = min(sol.cost, newlimit);
    }

    Solution sol;

    sol.cost = newlimit;

    return sol;
    
};


//main part of algo
//takes problem to be solved return Optimal solution
Solution IDAstar(Rubikscube problem, database pdb) {
    db = pdb;
    Node node(problem);
    int limit = db.heuristic(node.state);

    while (limit < INT_MAX) {
        Solution sol = IDdfs(node, limit);
        if  (! sol.path.empty()) 
            return sol;
        limit  = sol.cost;
    }

    Solution fail;
    fail.cost = INT_MAX;

    return fail;
};


void print_solution(Solution solution) {
    //When cube can't be solved
    if (solution.cost == INT_MAX) {
        printf("The cube can't be solved\n");
        return;
    }

    int moves_made = solution.path.size() - 1;
    printf("-------Total number of moves maded :  %d\n", moves_made);
    for (auto it = solution.path.rbegin();it != solution.path.rend();++it) {
         
        switch (*(it)) {
            //left face
            case 1: printf("Move left face 90 deg ckw\n");break;
            case 2: printf("Move left face 180 deg ckw\n");break;
            case 3: printf("Move left face 90 deg ackw\n");break;

            //right face
            case 4: printf("Move right face 90 deg ckw\n");break;
            case 5: printf("Move right face 180 deg ckw\n");break;
            case 6: printf("Move right face 90 deg ackw\n");break;

            //top face
            case 7: printf("Move top face 90 deg ckw\n");break;
            case 8: printf("Move top face 180 deg ckw\n");break;
            case 9: printf("Move top face 90 deg ackw\n");break;

            //bottom face
            case 10: printf("Move bottom face 90 deg ckw\n");break;
            case 11: printf("Move bottom face 180 deg ckw\n");break;
            case 12: printf("Move bottom face 90 deg ackw\n");break;

            //front face
            case 13: printf("Move front face 90 deg ckw\n");break;
            case 14: printf("Move front face 180 deg ckw\n");break;
            case 15: printf("Move front face 90 deg ackw\n");break;

            //back face
            case 16: printf("Move back face 90 deg ckw\n");break;
            case 17: printf("Move back face 180 deg ckw\n");break;
            case 18: printf("Move back face 90 deg ackw\n");break;

        }
    }
}
