#include <iostream>
using namespace std;
double baseReward[16] = { 0.0,0.0,6.0,80.0,0.0,2.0,0.0,5.0,45.0,12.0,1.0,0.0,8.0,0.0,0.0,4.0 };

double actionValue(int index, int direction, double currentReward[]) {  //retrieve the value of successfully going in a direction from initial point based on passed in currentReward[]
	double value = currentReward[index];  //if direction logic fails (bounces off a wall) value will just be reward at current spot
	if (direction == 0)  //if trying to go up
		if (index - 4 >= 0)  //and up is in the state space
			value = currentReward[index-4];  //return value will be up
	if (direction == 1) //if trying to go right
		if (index + 1 < 16 && (index+1)%4!=0)  //and right is in state space and not the end of a line (4,8,12,16 all %4 = 0)
			value = currentReward[index + 1];  //return value will be right
	if (direction == 2) //if trying to go down
		if (index + 4 < 16)  //and down is in state space
			value = currentReward[index + 4]; //return value will be down
	if (direction == 3)  //if trying to go left
		if (index - 1 >= 0 && index % 4 != 0 )  //and left is in state space and not the start of a line (1,5,9,13 are index 0,4,8,12 all %4 = 0)
			value = currentReward[index - 1]; //return value will be left
	return value;
}

double evalState(int index, double currentReward[], int direction[]) {  //determine and return the value and direction of the best direction to attempt
	double best = 0;
	for (int i = 0; i < 4; i++) {  //check each direction
		double sum = .7*actionValue(index,i, currentReward) + .2*actionValue(index,(i+2)%4, currentReward) + .1*currentReward[index]; //.7*success + .2*opposite + .1*current spot
		if (sum > best){ //if current direction is better than best found
			best = sum;  //log this value
			direction[index] = i;  //direction array for this index is written to current direction integer
			//Note: before being sent to this function, direction is set to all 0, therefore if above logic is never triggered, direction will be 0 (UP)
		}
	}
	return best;
}

void makeNewReward(double oldReward[], double newReward[], int direction[], double gamma) { 
	for (int i = 0; i < 16; i++) { //for each state evaluate new results based on results of previous iteration
		newReward[i] = baseReward[i] + gamma*evalState(i, oldReward, direction); //newReward is value of being in the current state + gamma*(optimal potential derived from last iterations reward array)
	}
}

void printArrays(double value[],int direction[]) { //display results
	for (int i = 0; i < 16; i++) {
		cout << value[i] << "\t";
		if((i+1)%4 == 0)
			cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 16; i++) {
		if(direction[i] == 0)  //up
			cout << "^\t";
		if (direction[i] == 1)  //right
			cout << ">\t";
		if (direction[i] == 2)  //down
			cout << "v\t";
		if (direction[i] == 3) //left
			cout << "<\t";
		if ((i + 1) % 4 == 0)
			cout << endl;
	}
	cout << endl;
}

void copyArray(double source[], double target[]) {  //was paranoid about array = array call so I decided to write a simple function to be 100% sure it would do what I wanted
	for (int i = 0; i < 16; i++) 
		target[i] = source[i];
}

void clearDoubleArray(double A[]) {  
	for (int i = 0; i < 16; i++)
		A[i]=0;
}

void clearIntArray(int A[]) {
	for (int i = 0; i < 16; i++)
		A[i] = 0;
}

int main() {
	double oldReward[16];
	double newReward[16];
	int direction[16];
	copyArray(baseReward,oldReward);  //for V^1 oldReward is just baseReward

	for(int i=1; i <=6; i++){	//for V^1 to V^6
		clearDoubleArray(newReward);  //clear newReward
		clearIntArray(direction);	//clear direction
		makeNewReward(oldReward, newReward,direction,1);  //calculate V^i  (with gamma as 1, no decay)
		copyArray(newReward,oldReward);	//newReward on this cycle will be used as the oldReward for the next cycle
	}
	cout << "V^6" << endl;
	printArrays(newReward, direction); //display V^6 results

	copyArray(baseReward, oldReward);
	for (int i = 1; i <= 1000; i++) {  //for a sufficiently excessive number of iterations to ensure convergence
		clearDoubleArray(newReward);
		clearIntArray(direction);
		makeNewReward(oldReward, newReward, direction, .96);  //calculate V^i  (with gamma as 0.96)
		copyArray(newReward, oldReward); //newReward on this cycle will be used as the oldReward for the next cycle
	}
	cout << "V*" << endl;
	printArrays(newReward, direction);  //print results of V^1000 which is used as V* 
	// (Actual inspection of results show basically no change in value for hundreds of iterations, but since this executes in a trivial amount of time, I don't much care)
	return 0;
}
