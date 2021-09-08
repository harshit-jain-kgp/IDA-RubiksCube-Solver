#include "database.h"
#include <deque>
#include <iostream>
#include <fstream>


using namespace std;

//g++ -o database_generator generate_database.cpp database.cpp Rubikscube.cpp Node.cpp IDA.cpp

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int pow(int b, int e) {
    int pow = 1;

    while(e){
    	if(e%2)
    		pow=pow*b;
    	b*=b;
    	e/=2;
    }

    return pow;
}

int rankAux(int k,int n,int *seq,int *inverse){

    if (n==k)
        return 0;

    int s = seq [n-1];
    swap(&seq[n-1], &seq[inverse[n-1]]);
    swap(&inverse[s], &inverse[n-1]);

    return s + n*(rankAux(k,n-1, seq, inverse));
}

void unrankAux(int k,int n,int r,int *identity){

    if (n > k) {
        int tmp = identity[n-1];
        identity[n-1] = identity[r % n];
        identity[r % n] = tmp;
        unrankAux(k,n-1, r / n, identity);
    }
}

// The maximum value between the three pattern databases
int database::heuristic(Rubikscube cube) {
    int rankc = rankC(cube.clone());
    int ranke1 = rankE(1, cube.clone());
    int ranke2 = rankE(2, cube.clone());

    int hcorners = corners[rankc];
    int he1 = edgeslf[ranke1];
    int he2 = edgesrb[ranke2];

    //Get maximum value
    
    int h = max(hcorners, he1);
    h = max(h, he2);

    return h;
};

//Initializes corners, edgeslf, and edgesrb
void database::initializeAllWithFiles() {
    printf("\nInitializing pattern databases from files\n\n");
    initializeCornersWithFile();
    initializeEdgesWithFile(1);
    initializeEdgesWithFile(2);
};

//Initializes corners
void database::initializeCornersWithFile() {
    printf("----Initializing corners\n");
    ifstream ifm;
    ifm.open("corners.bin");

    
    for (int i=0;i<264539520;i++) {
        int x;
        ifm>>x;
        corners.push_back(x);
    }

    ifm.close();
};

//Initializes one array of edges
//edgeslf table = 1 	edgesrb table == 2
void database::initializeEdgesWithFile(int table) {
    printf("----Initializing edges%d\n",table);
    //vector<char> edges[2] = {edgeslf, edgesrb}; 
    int t = table - 1;  //index of edges

    ifstream ifm;
    if (table == 1) 
        ifm.open("edgeslf.bin");
    else 
        ifm.open("edgesrb.bin");

    
    for (int i=0;i<42577920;i++) {
        int x;
        ifm>>x;
        (table ? edgesrb : edgeslf).push_back(x);
    }

    ifm.close();
};

//Initializes corners, edgeslf, edgesrb
void database::initializeAll() {
    printf("\nInitializing pattern databases\n\n");
    initializeCorners();
    initializeEdges(1);  //edgeslf  
    initializeEdges(2);  //edgesrb
};

//Initializes pattern database of corner permutations
void database::initializeCorners() {
    printf("----Initializing corners\n");
    //initialize every cost in 255 
    
    for (int i=0;i<264539520;i++) 
        corners.push_back(255);  

    Rubikscube goalcube;
    goalcube.reset();
    int goal = rankC(goalcube);
    corners[goal] = 0;

    deque<int> open;
    open.push_back(goal);
    
    while (!open.empty()) {
        int parent = open.front();        //top
        open.pop_front();                 //pop 

        Rubikscube cube = unrankC(parent);
        vector<Rubikscube> s = cube.getNext();    //next possiblities

        for (vector<Rubikscube>::iterator it = s.begin();it != s.end();it++) {
            int child = rankC(*it);

            //Already closed or in open queue
            if (corners[child] != 255)  
                continue;
            
            corners[child] = corners[parent] + 1;
            open.push_back(child);
        }
    }

    //Printing to binary file corners.bin
    ofstream ofm;
    ofm.open("corners.bin");

    for (int i=0;i<264539520;i++) {
        int x = corners[i];
        ofm<<x;
    }

    ofm.close(); 

};

//Initializes pattern database of edge permutations 
//table : 1 for edgeslf, 2 for edgesrb
void database::initializeEdges(int table) {
    printf("----Initializing edges%d\n",table);
    //vector<char> edges[2] = {edgeslf, edgesrb}; 
    int t = table - 1;  //index of edges

    //Default values
    
    for (int i=0;i<42577920;i++) {
        (table ? edgesrb : edgeslf).push_back(255);  
    }

    Rubikscube goalcube;
    goalcube.reset();
    int goal = rankE(table, goalcube);

    (table ? edgesrb : edgeslf)[goal] = 0;

    deque<int> open;     //open queue for edgeslf
    open.push_back(goal);

    while (!open.empty()) {
        int parent = open.front();  //top
        open.pop_front();           //pop

        //Get cube
        Rubikscube cube = unrankE(table, parent);

        vector<Rubikscube> s = cube.getNext();  //next possibilities

        for (vector<Rubikscube>::iterator it = s.begin();it != s.end();it++) {
            int child = rankE(table, (*it));

            //Already close or in open queue
            if ((table ? edgesrb : edgeslf)[child] != 255)
                continue;
            
            (table ? edgesrb : edgeslf)[child] = (table ? edgesrb : edgeslf)[parent] + 1;

            open.push_back(child);
        }
    }

    //Printing to binary file edgeslf.bin or edgesrb.bin
    ofstream ofm;
    if (table == 1) 
        ofm.open("edgeslf.bin");
    else
        ofm.open("edgesrb.bin");

    for (int i=0;i<42577920;i++) {
        int x = (table ? edgesrb : edgeslf)[i];
        ofm<<x;
    }

    ofm.close(); 
};

//Ranks a Rubik's cube partly by taking only corners
int database::rankC(Rubikscube cube) {
    int x = rankCornersP(cube);     //Permutation of positions
    int y = rankCornersO(cube);     //Permutation of orientations

    return x*(pow(3,8)) + y;
};

//Ranks a Rubik's cube partly by taking only corner positions 
//It uses rankAux to rank a permutation of integers into an integer
//value between 0 and 40319
int database::rankCornersP(Rubikscube cube) {
    int positions[8];
    int k = 0;
    
    //Transforms a Rubik's cube into a permutation of corners
    
    for (int i=0;i<16;i++) {
        if (i % 2 == 0) {
            positions[k] = (cube.getPosition(i)) / 2;
            k++;
        }
    }

    //Pre calculate inverse
    int inverse[8];
    for (int i=0;i<8;i++)
      inverse[positions[i]] = i;

    return (rankAux(1, 8, positions, inverse));  //rank a sequence of integers
};

//Ranks a Rubik's cube partly by taking only corner orientations 
//3 orientations X,Y,Z: 100, 010, 001	represent as 0,1, or 2 that's why % 4 is used!
//value between 0 and 6560)
int database::rankCornersO(Rubikscube cube) {
    int rank = 0;
    
    
    for (int i=0;i<16;i++) {
        if (i % 2 == 0) {
            //represent orientations as 0,1, or 2
            int orientation = cube.getOrientation(i) % 4;
            rank = (rank*3) + orientation;
        }
    }

    return rank;
};

//Unranks a permutation of corner cubies
//value between 0 and 264,539,519
//return Rubik's cube configuration with null values in edge cubies
Rubikscube database::unrankC(int p) {
    int t = 6561;    //3^8
    Rubikscube cube;  
    cube.reset();

    Rubikscube positions = unrankCornersP(p / t);
    Rubikscube orientations = unrankCornersO(p % t);
    
    
    for (int i=0;i<20;i++) {
        //Merge ID with orientation to get final cubie
        char cubie = positions.getCubie(i) | orientations.getCubie(i);
        cube.setCubie(i, cubie);
    }

    return cube;
};
 
//Gets a Rubik's cube configuration without orientations (only positions) 
//uses unrankAux to unrank an intenger into a sequence of integers
//value between 0 and 40319 (ranked ID permutation)
Rubikscube database::unrankCornersP(int x) {
    Rubikscube cube;
    cube.reset();
    int identity[8] = {0,1,2,3,4,5,6,7};
    

    unrankAux(0, 8, x, identity);   //Put in identity the unranked permutation

    //Transform sequence into a Rubik's cube configuration 
    for (int i=0;i<8;i++) {
        char cubie = identity[i]*2;
        cubie = cubie<<3;
        cube.setCubie(i*2, cubie);       //i*2 because these are corner cubies
    }

    return cube;
};

//Gets a Rubik's cube configuration without positions (only orientations)
//value between 0 and 6560 (ranked orientation permutations)
Rubikscube database::unrankCornersO(int x) {
    Rubikscube cube;
    cube.reset();
    int y = x;
    
    int k = 0;

    for (int i=7;i >= 0;i--) {
        //calculate orientation of di with pure magic
        int power = pow(3, i);
        int di = y / power;     
        y = y-(di*power);     

        if (di == 0)
            di = 4;    //di was oriented to X-axis, wich is 100 in Rubikscube class

        char cubie = di; 
        cube.setCubie(k*2, cubie);
        k++;
    }

    return cube;
};

//Ranks a Rubik's cube partly by taking only edges  (edgeslf)
//table : what table is going to be used	cube : Rubik's cube configuration
//value between 0 and 42,577,920
int database::rankE(int table, Rubikscube cube) {
    int x = rankEdgesP(table, cube);     //Permutation of positions
    int y = rankEdgesO(table, cube);     //Permutation of orientations

    return x*(pow(2,6)) + y;
};

//Ranks a Rubik's cube partly by taking only the positions of six edges 
//uses rankAux to rank a permutation of 	integers into an integer
//table: Table we are ranking (1 for edgeslf, or 2 for edgesrb)	cube: Rubik's cube configuration
//value between 0 and 665,279
int database::rankEdgesP(int table, Rubikscube cube) {
    int positions[6];

    //With table 2, first cubie is 8-th cubie 
    int offset = (table-1)*8;     
    
    int k = 0;
    for (int i=offset;i<8 + offset;i++) {
        if (i % 2 != 0) {
            int p = cube.getPosition(i);
            int elem = p / 2;
            if (p > 16) {
                elem = p - 8;
            }
            positions[k] = elem;
            k++;
        }
    }

    //Same goes for edges in middle face
    int firstmiddle = 16 + (offset / 4); 
    k =  4;
    for (int i=firstmiddle;i<= firstmiddle + 1;i++) {
        int p = cube.getPosition(i);
        int elem = p / 2;
        if (p > 16) {
            elem = p - 8;
        }
        positions[k] = elem;
        k++;
    }

    int set[12], inverse[12];
    auxiliaryRankEdgesP(positions, set, inverse);

    return (rankAux(6, 12, set, inverse));  //rank a sequence of integers
};

//Set the three elementes necessary to do k-permutations of an n-set
//positions: K elements to be permuted	set: Array of integers to be ranked 	inverse: Inverse of set
void database::auxiliaryRankEdgesP(int *positions, int *set, int *inverse) {
    int i, k;
    //Get subset
    int appear[12];

    for (int i=0;i<12;i++)
        appear[i] = 0;

    for (int i=0;i<6;i++)
        appear[positions[i]] = 1;

    //Positions elements are the last K-elements 
    for (int i=6;i<12;i++)    
        set[i] = positions[i % 6];

    k = 0;
    for (int i=0;i<12;i++) {
        if (appear[i] == 0) {
            set[k] = i;
            k++;
        }
    }

    //Calculate inverse 
    for (int i=0;i<12;i++)
        inverse[set[i]] = i;
};

//Ranks a Rubik's cube partly by taking only six edges orientations 
//Edge cubies have only two possible directions in a certain position
//value between 0 and 63
int database::rankEdgesO(int table, Rubikscube cube) {
    int offset = (table-1)*8;
    int rank = 0;

    //Left or Right face
    
    for (int i=1;i<= 7;i = i + 2) {
        if (cube.isMaxOriented(i + offset)) 
            rank = rank + 1;
        rank = rank*2;
    }

    //Middle face
    for (int i=0;i<2;i++) {
        if (cube.isMaxOriented(16 + i + (offset / 4)))
            rank = rank + 1;
        rank = rank * 2;
    }

    rank = rank / 2;

    return rank;
};

//Unranks a permutation of edge cubies
//p: value between 0 and 42,577,919
Rubikscube database::unrankE(int table, int p) {
    int t = 64;   //2^6
    Rubikscube cube;
    cube.reset();

    Rubikscube positions = unrankEdgesP(table, p / t);
    Rubikscube all = unrankEdgesAux(table, p % t, positions);

    return all;
};

//Gets a Rubik's cube configuration without orientations (only position) 
//uses unrankAux to unrank an intenger into value between 0 and 665,279 (ranked permutation of positions)
//x: permutation of corner positions represented as an int
Rubikscube database::unrankEdgesP(int table, int x) {
    Rubikscube cube;
    cube.reset();
    
    int offset = (table-1)*8;
    int identity[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

    unrankAux(6, 12, x, identity);

    //Transform sequence into a Rubik's cube configuration
    int k = 0;
    
    for (int i=6;i<10;i++) {
        char cubie = identity[i]*2 + 1;
        if (identity[i] >= 8) {
            cubie = identity[i] + 8;  
        }
        cubie = cubie<<3;
        cube.setCubie(k*2 + 1 + offset, cubie);
        k++;
    }
    
    int middle = 16 + (offset / 4);
    for (int i=10;i<12;i++) {
        char cubie = identity[i]*2 + 1;
        if (identity[i] >= 8)
            cubie = identity[i] + 8;

        cubie = cubie<<3;
        cube.setCubie(middle, cubie);
        middle++;
    } 

    return cube;
};

//Gets a Rubik's cube configuration mergin positions and orientations
//This is an auxiliary method for unrankE
//x: permutation of corner orientations(int)	positions: Rubik's cube configuration with its positions
Rubikscube database::unrankEdgesAux(int table, int x, Rubikscube positions) {
    Rubikscube cube;
    cube = positions.clone();
    
    int y = x;
    int offset = (table-1)*8;
    int k = 1 + offset;
    for (int i=5;i > 1;i--) {
        //calculate orientation of di with pure magic
        int power = pow(2, i);
        int di = y / power;
        y = y - (di*power);
        
        char cubie;

        if (di == 1) {
            cube.setMaxOrientation(k);
        }
        else {
            cube.setMinOrientation(k);
        }

        k = k + 2;
    }

    //Last two cubies middle[x], middle[x+1]
    k = 16 + (offset / 4); 
    int di = y / 2;
    y = y - (di*2);

    if (di == 1) {
        cube.setMaxOrientation(k);
    }
    else {
        cube.setMinOrientation(k);
    }

    k++;

    di = y;
    if (di == 1) {
        cube.setMaxOrientation(k);
    }
    else {
        cube.setMinOrientation(k);
    }
    
    return cube;
};
