#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

class Rubikscube{
	private:
		//5 bit identity, 3 x,y,z
		char left[8];//8 movable cubes on left face
		char right[8];//8 movable cubes on right face
		char mid[4];//4 movable in mid

	public:
        Rubikscube();
		void reset();
		void print();
        void random();
        Rubikscube clone();
        void setCubie(int,char);
        char getCubie(int);
        void turn(int,int);
        void turnLeft();
        void turnRight();
        void turnTop();
        void turnBottom();
        void turnFront();
        void turnBack();
        bool isSolved();
        std::vector<Rubikscube> getNext();
        bool isMaxOriented(int);
        void setMaxOrientation(int);
        void setMinOrientation(int);
        int getPosition(int);
        int getOrientation(int);
    private:
        void changePositionTo(int,int);
        char swapXY(char);
        char swapXZ(char);
        char swapYZ(char);
};