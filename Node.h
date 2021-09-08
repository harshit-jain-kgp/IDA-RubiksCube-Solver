#pragma once

#include "Rubikscube.h"


class Node{
	public:
		Node() {}
		Node(Rubikscube);
		Node(Rubikscube,Node *,int,int);
		Node *parent;
		int action;
		

	public:    
		Rubikscube state;
		int cost;    
		bool isSolved();
        Node getChild(int);
        int numberOfChildren();
        std::vector<int> extractSolution();
};