#include "Rubikscube.h"

void Rubikscube::setCubie(int i,char val){
	i%=20;
	if(i<8)
		left[i] = val;
	else if(i<16)
		right[i-8] = val;
	else if(i<20)
		mid[i-16] = val;
}

char Rubikscube::getCubie(int i){
	i%=20;

	if(i<8)
		return left[i];
	else if(i<16)
		return right[i-8];
	else if(i<20)
		return mid[i-16];
}

/*
 Cubie       Pos   Ornt 	Num
 left[0]     00000 001		0      
 left[1]     00001 001		1      
 left[2]     00010 001		2
 left[3]     00011 010		3
 left[4]     00100 001		4
 left[5]     00101 001		5
 left[6]     00110 001		6
 left[7]     00111 010		7

 right[0]    01000 001		8     
 right[1]    01001 001		9    
 right[2]    01010 001		10
 right[3]    01011 010		11
 right[4]    01100 001		12
 right[5]    01101 001		12
 right[6]    01110 001		14
 right[7]    01111 010		15

 mid[0]		 10000 001      16
 mid[1]		 10001 001      17
 mid[2]		 10010 001 		18
 mid[3]		 10011 001 		19
*/

void Rubikscube::reset(){
	for(int i=0;i<16;i++){
		char orientation =1;
		if(i%4==3)
			orientation = 2;
		
		setCubie(i, (i<<3) | orientation);
	}

	for(int i=16;i<20;i++){
		setCubie(i, (i<<3) | 1);
	}
}


int Rubikscube::getPosition(int i){
	return (getCubie(i) >> 3);//left 5 bits is identity
}

int Rubikscube::getOrientation(int i){
	return (getCubie(i) & 7);//right 3 bits is orientation
}

void Rubikscube::print(){

	printf("\n                 position     orientation\n");
	for(int i=0;i<8;i++)
	printf("    Left[%d]  =     %d            %d      \n",i,getPosition(i),getOrientation(i));
	
	for(int i=8;i<16;i++)
	printf("   Right[%d]  =     %d            %d      \n",i-8,getPosition(i),getOrientation(i));

	
	for(int i=0;i<8;i++)
	printf("     Mid[%d]  =     %d            %d      \n\n",i-16,getPosition(i),getOrientation(i));
	
}

char Rubikscube::swapXY(char id){
	char x = id & 4;				//100
	char y = id & 2;				//010
	char z = id & 1;				//001
	char mask =(char)((int)(1<<9)-1-(1+2+4));	//1111 1000
	char newid = id & mask;
	x>>=1;
	y<<=1;
	return (newid | x | y | z);
}

char Rubikscube::swapYZ(char id){
	char x = id & 4;				//100
	char y = id & 2;				//010
	char z = id & 1;				//001
	char mask = (char)((int)(1<<9)-1-(1+2+4));	//1111 1000
	char newid = id & mask;
	y>>=1;
	z<<=1;
	return (newid | x | y | z);
}

char Rubikscube::swapXZ(char id){
	char x = id & 4;				//100
	char y = id & 2;				//010
	char z = id & 1;				//001
	char mask = (char)((int)(1<<9)-1-(1+2+4));	//1111 1000
	char newid = id & mask;
	x>>=2;
	z<<=2;
	return (newid | x | y | z);
}

void Rubikscube::changePositionTo(int i,int position){
	char newid = (position<<3) | getOrientation(i);
	setCubie(i,newid);
}

void Rubikscube::turnLeft(){
	for(int i=0;i<20;i++){									// 6 5 4
		if(getPosition(i) < 8){								// 7   3
			setCubie(i,swapYZ(getCubie(i)));				// 0 1 2
			changePositionTo(i,(getPosition(i)+6)%8);	
		}
	}
}

void Rubikscube::turnRight(){
	for(int i=0;i<20;i++){									// 14 13 12
		if(getPosition(i)>=8 && getPosition(i) < 16){		// 15    11
			setCubie(i,swapYZ(getCubie(i)));				//  8  9 10
			changePositionTo(i,8 + (getPosition(i)+6)%8);	
		}
	}
}

void Rubikscube::turnTop(){
	for(int i=0;i<20;i++){
		int posto=0;
		switch(getPosition(i)){
            case 4:   posto = 6; 	break;					// 6 18 12
            case 5:   posto = 18;	break;					// 5    13
            case 6:   posto = 12;	break;					// 4 17 14
            case 18:  posto = 13;	break;
            case 12:  posto = 14;	break;
            case 13:  posto = 17;	break;
            case 14:  posto = 4;	break;
            case 17:  posto = 5;	break;
		}
		if(posto){
			setCubie(i,swapXY(getCubie(i)));
			changePositionTo(i,posto);
		}
	}
}

void Rubikscube::turnBottom(){
	for(int i=0;i<20;i++){
		int posto=-1;
		switch(getPosition(i)){
            case 0:  posto = 2;		break;					// 0 19 10
            case 1:  posto = 16;	break;					// 1     9
            case 2:  posto = 8;		break;					// 2 16  8
            case 16: posto = 9;		break;
            case 8:  posto = 10;	break;					//(according to top) so rotate ackw for actual ckw
            case 9:  posto = 19;	break;
            case 10: posto = 0;		break;
            case 19: posto = 1;		break;
		}
		if(posto!=-1){
			setCubie(i,swapXY(getCubie(i)));
			changePositionTo(i,posto);
		}
	}
}
   
void Rubikscube::turnFront(){
	for(int i=0;i<20;i++){
		int posto=0;
		switch(getPosition(i)){
            case 2:  posto = 4;		break;					// 4 17 14
            case 3:  posto = 17;	break;					// 3    15
            case 4:  posto = 14;	break;					// 2 16  8 
            case 17: posto = 15;	break;
            case 14: posto = 8; 	break;
            case 15: posto = 16;	break;
            case 8:  posto = 2;		break;
            case 16: posto = 3;		break;
		}
		if(posto){
			setCubie(i,swapXZ(getCubie(i)));
			changePositionTo(i,posto);
		}
	}
}

void Rubikscube::turnBack(){
	for(int i=0;i<20;i++){
		int posto=-1;
		switch(getPosition(i)){
            case 0:  posto = 10;	break;					// 6 18 12
            case 7:  posto = 19;	break;					// 7    11
            case 6:  posto = 0; 	break;					// 0 19 10
            case 18: posto = 7; 	break;
            case 12: posto = 6; 	break;					//(according to front) so rotate ackw for actual ckw
            case 11: posto = 18; 	break;
            case 10: posto = 12;	break;
            case 19: posto = 11;	break;
		}
		if(posto!=-1){
			setCubie(i,swapXZ(getCubie(i)));
			changePositionTo(i,posto);
		}
	}
}

//for generating 100 move random config
void Rubikscube::random(){
	reset();
	//transformToGoal();
	int moves = 100;

	srand(time(NULL));
	for(int i=0;i<moves;i++){
		srand(rand());
		int face = rand()%6 +1;

		int times = (rand()%3) +1;//+90,+180,-90~+270

		while(times--){
			switch(face){
				case 1 : 
					turnLeft();
                    break;
                case 2 : 
                	turnRight();
                    break;
                case 3 : 
                	turnTop();
                    break;
                case 4 : 
                	turnBottom();
                    break;
                case 5 : 
                	turnFront();
                    break;
                case 6 : 
                	turnBack();
                    break;
			}
		}
	}
}


void Rubikscube::turn(int face, int times) {
    //Moves that can be maded
    void (Rubikscube::*moves[6]) () = {
      &Rubikscube::turnLeft,     
      &Rubikscube::turnRight, 
      &Rubikscube::turnTop,  
      &Rubikscube::turnBottom,
      &Rubikscube::turnFront,
      &Rubikscube::turnBack,
    };

    int i;
    for (i = 0; i < times; i++)
        (this->*moves[face])();

}

Rubikscube Rubikscube::clone(){
	Rubikscube clone;

	for(int i=0;i<20;i++){
		clone.setCubie(i,getCubie(i));
	}

	return clone;
}

bool Rubikscube::isSolved(){

	for(int i=0;i<16;i++){
		if(i!=getPosition(i))
			return false;

		if(i%4==3){
			if(getOrientation(i)!=2)
				return false;
		}
		else{
			if(getOrientation(i)!=1)
				return false;
		}
		
	}

	for(int i=16;i<20;i++){
		if(i!=getPosition(i))
			return false;

		if(getOrientation(i)!=1)
			return false;
	}

	return true;
}



std::vector<Rubikscube> Rubikscube::getNext(){

	std::vector<Rubikscube> next;
	
	Rubikscube cll,clr,clt,clbm,clf,clb;
	clb = clone();
	cll=clr=clt=clbm=clf=clb;

	cll.turnLeft();next.push_back(cll);
	cll.turnLeft();next.push_back(cll);
	cll.turnLeft();next.push_back(cll);

	clr.turnLeft();next.push_back(clr);
	clr.turnLeft();next.push_back(clr);
	clr.turnLeft();next.push_back(clr);

	clt.turnLeft();next.push_back(clt);
	clt.turnLeft();next.push_back(clt);
	clt.turnLeft();next.push_back(clt);

	clbm.turnLeft();next.push_back(clbm);
	clbm.turnLeft();next.push_back(clbm);
	clbm.turnLeft();next.push_back(clbm);

	clf.turnLeft();next.push_back(clf);
	clf.turnLeft();next.push_back(clf);
	clf.turnLeft();next.push_back(clf);

	clb.turnLeft();next.push_back(clb);
	clb.turnLeft();next.push_back(clb);
	clb.turnLeft();next.push_back(clb);

	return next;
}

bool Rubikscube::isMaxOriented(int i) {
    if(getPosition(i)<16)
        return (getOrientation(i) == 4) ? true : false;
    else
        return (getOrientation(i) == 2) ? true : false;
}

void Rubikscube::setMaxOrientation(int i){

    char mask = (char)((int)(1<<9)-1-(1+2+4));  		// 1111 1 000
    char cubie = getCubie(i) & mask;

    if(getPosition(i)<16)
        setCubie(i, cubie | 4);  //x
    else 
        setCubie(i, cubie | 2);	//y
}

void Rubikscube::setMinOrientation(int i){
    int position = getPosition(i);
    char mask = (char)((int)(1<<9)-1-(1+2+4)); 			// 1111 1 000
    char cubie = getCubie(i) & mask;

    if((getPosition(i)+1)%4)
    	setCubie(i, cubie | 1);    //z
    else
    	setCubie(i, cubie | 2);
}