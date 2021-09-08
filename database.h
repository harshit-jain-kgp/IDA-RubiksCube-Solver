#pragma once

#include "Rubikscube.h"


//https://web.mit.edu/sp.268/www/rubik.pdf
class database{
	private:
		std::vector<char> corners/*(264539520)*/;	    //8!  *  3^8
		std::vector<char> edgeslf/*(42577920)*/;		//12!/6!  *  2^6 	//4 left + 2 front mid
		std::vector<char> edgesrb/*(42577920)*/;		//12!/6!  *  2^6 	//4 right+ 2 back mid
		
	public:
		int heuristic(Rubikscube);
		void initializeAll();
        void initializeAllWithFiles();
    private:
        void initializeCorners();
        void initializeEdges(int);
        void initializeCornersWithFile();
        void initializeEdgesWithFile(int);

        //Methods to rank permutation of corners cubies
        int rankC(Rubikscube);      
        int rankCornersP(Rubikscube);   //Auxiliary method to rankC
        int rankCornersO(Rubikscube);     //Auxiliary method to rankC

        //Methods to unrank permutation of corners cubies
        Rubikscube unrankC(int);
        Rubikscube unrankCornersP(int);     //Corner Positions 
        Rubikscube unrankCornersO(int);     //Corner Orientations

        //Methods to rank permutation of edges cubies
        int rankE(int,Rubikscube);
        int rankEdgesP(int,Rubikscube);
        void auxiliaryRankEdgesP(int *,int *,int *);
        int rankEdgesO(int,Rubikscube);

        //Methods to unrank permutation of edges cubies
        Rubikscube unrankE(int,int);
        Rubikscube unrankEdgesP(int,int);          //Edge Positions 
        Rubikscube unrankEdgesAux(int,int,Rubikscube);  //Auxiliary method

};