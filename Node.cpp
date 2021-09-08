#include "Node.h"

Node::Node() {
	
}

Node::Node(Rubikscube state){
	this->state=state;
	parent=nullptr;
	action=0;
	cost=0;
}

Node::Node(Rubikscube state,Node *parent,int action,int cost){
	this->state=state;
	this->parent=parent;
	this->action=action;
	this->cost=cost;
}
        
bool Node::isSolved(){
	return state.isSolved();
}

Node Node::getChild(int child){
	int excl1=-1,excl2=-1;

	switch(action){
		case  1: case  2: case  3: excl1 = 0; excl2 = 0; break;//left
		case  4: case  5: case  6: excl1 = 0; excl2 = 1; break;//right
		case  7: case  8: case  9: excl1 = 2; excl2 = 2; break;//top
		case 10: case 11: case 12: excl1 = 2; excl2 = 3; break;//bottom
		case 13: case 14: case 15: excl1 = 4; excl2 = 4; break;//front
		case 16: case 17: case 18: excl1 = 4; excl2 = 5; break;//back
	}

	int i,j;
	for(j=0,i=-1;(j<18) && (i<child);j++){
		if(j/3 == excl1 || j/3 == excl2)
			continue;
		i++;
	}
	j--;

	int face = j/3;
	int times = (j%3)+1;

	Rubikscube cube=state.clone();
	cube.turn(face,times);

	Node nn(cube,this,j+1,cost+1);
	return nn;
}

int Node::numberOfChildren(){
	switch(action){
		case  0: return 18;
		case  1: case  2: case  3: return 15;
		case  4: case  5: case  6: return 12;
		case  7: case  8: case  9: return 15;
		case 10: case 11: case 12: return 12;
		case 13: case 14: case 15: return 15;
		case 16: case 17: case 18: return 12;
	}

	return 0;
}

std::vector<int> Node::extractSolution(){
	std::vector<int> path;
	
	Node *tmp = this;

	while(tmp!=NULL){
		path.push_back(tmp->action);
		tmp=tmp->parent;
	}

	return path;
}