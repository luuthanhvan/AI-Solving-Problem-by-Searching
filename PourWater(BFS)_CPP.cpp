#include <iostream>
#include <queue>
#include <stack>
#include <malloc.h>
#define tankcapacity_X 9
#define tankcapacity_Y 4
#define tankempty 0
#define goal 6
using namespace std;

const char* action[] = {"First State", "pour Water Full X", "pour Water Full Y", "pour Water Empty X",
"pour Water Empty Y", "pour Water X to Y", "pour Water Y to X"};

typedef struct{
	int x, y;
} State;

void makeNullState(State *state){
	state->x = tankempty;
	state->y = tankempty;
}

void printState(State state){
	cout << endl << "   X:" << state.x << " --- " << "Y:" << state.y;
}

int goalCheck(State state){
	return (state.x == goal || state.y == goal);
}

int pourWaterFullX(State curState, State *result){
	if(curState.x < tankcapacity_X){
		result->x = tankcapacity_X;
		result->y = curState.y;
		return 1;
	}
	return 0;
}

int pourWaterFullY(State curState, State *result){
	if(curState.y < tankcapacity_Y){
		result->x = curState.x;
		result->y = tankcapacity_Y;
		return 1;
	}
	return 0;
}

int pourWaterEmptyX(State curState, State *result){
	if(curState.x > tankempty){
		result->x = tankempty;
		result->y = curState.y;
		return 1;
	}
	return 0;
}

int pourWaterEmptyY(State curState, State *result){
	if(curState.y > tankempty){
		result->x = curState.x;
		result->y = tankempty;
		return 1;
	}
	return 0;
}

int min(int num1, int num2){
	return num1 < num2 ? num1:num2;
}

int max(int num1, int num2){
	return num1 > num2 ? num1:num2;
}

int pourWaterXY(State curState, State *result){
	if(curState.x > tankempty && curState.y < tankcapacity_Y){
		result->x = max(tankempty, (curState.x - (tankcapacity_Y - curState.y)));
		result->y = min(tankcapacity_Y, (curState.x + curState.y));
		return 1;
	}
	return 0;
}

int pourWaterYX(State curState, State *result){
	if(curState.y > 0 && curState.x < tankcapacity_X){
		result->x = min(tankcapacity_X, (curState.x + curState.y));
		result->y = max(tankempty, (curState.y - (tankcapacity_X - curState.x)));
		return 1;
	}
	return 0;
}

int callOperator(State curState, State *result, int option){
	switch(option){
		case 1:
			return pourWaterFullX(curState, result);
		case 2:
			return pourWaterFullY(curState, result);
		case 3:
			return pourWaterEmptyX(curState, result);
		case 4:
			return pourWaterEmptyY(curState, result);
		case 5:
			return pourWaterXY(curState, result);
		case 6:
			return pourWaterYX(curState, result);
		default:
			cout << "Error calls operator";
			return 0;	
	}
}

typedef struct Node{
	State state;
	struct Node* parent;
	int noFunction;
} Node;

int compareStates(State state1, State state2){
	return (state1.x == state2.x && state1.y == state2.y);
}

int findState(State state, queue<Node*> openQueue){
	while(!openQueue.empty()){
		Node* curNode = openQueue.front();
		if(compareStates(state, curNode->state))
			return 1;
		openQueue.pop();
	}
	return 0;
}

Node* BFSAlgorithm(State state){
	queue<Node*> openBFS;
	queue<Node*> closeBFS;
	
	Node* root = (struct Node*)malloc(sizeof(struct Node));
	root->state = state;
	root->parent = NULL;
	root->noFunction = 0;
	
	openBFS.push(root);
	
	while(!openBFS.empty()){
		Node* node = openBFS.front();
		openBFS.pop();
		
		closeBFS.push(node);
		if(goalCheck(node->state))
			return node;
		int opt;
		for(opt = 1; opt <= 6; opt++){
			State newState;
			makeNullState(&newState);
			if(callOperator(node->state, &newState, opt)){
				if(findState(newState, openBFS) || findState(newState, closeBFS))
					continue;
				Node* newNode = (struct Node*)malloc(sizeof(struct Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				openBFS.push(newNode);
			}
		}
	}
	return NULL;
}

void printWaysToGetGoal(Node *node){
	stack<Node*> stackPrint;
	while(node->parent != NULL){
		stackPrint.push(node);
		node = node->parent;
	}
	stackPrint.push(node); // start node
	int noAction = 0;
	while(!stackPrint.empty()){
		Node* curNode = stackPrint.top();
		stackPrint.pop();
		cout << endl << "Action " << noAction << ": " << action[curNode->noFunction];
		printState(curNode->state);
		noAction++;
	}
}

int main(){
	State curState = {0,0};
	Node* node = BFSAlgorithm(curState);
//	printState(node->state);
	printWaysToGetGoal(node);
	return 0;
}
