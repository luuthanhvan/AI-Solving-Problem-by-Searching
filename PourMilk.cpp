#include <iostream>
#include <stdlib.h>
#include <queue>
#include <stack>
#define TANK_CAPACITY_X 10
#define TANK_CAPACITY_Y 5
#define TANK_CAPACITY_Z 6
#define EMPTY 0
#define GOAL 8
#define MAX_OPERATOR 6
using namespace std;

/* 
Start state: (10, 0, 0)
Goal state: (8, y, z)
*/

const char* actions[] = {"First state", "Pouring milk from Tank X to Tank Y", "Pouring milk from Tank X to Tank Z",
						"Pouring milk from Tank Y to Tank X", "Pouring milk from Tank Y to Tank Z", 
						"Pouring milk from Tank Z to Tank X", "Pouring milk from Tank Z to Tank Y"};

typedef struct{
	int x, y, z;
} State;

void initState(State *state){
	state->x = 10;
	state->y = 0;
	state->z = 0;
}

void printState(State state){
	cout << endl << "(" << state.x << ", " << state.y << ", " << state.z << ")" << endl << endl;
}

int checkGoal(State state){
	return (state.x == GOAL || state.y == GOAL || state.z == GOAL);
}

int compareStates(State state1, State state2){
	return (state1.x == state2.x && state1.y == state2.y && state1.z == state2.z);
}

/* 
Actions:
	pourMilkXY(state, result)
	pourMilkYX(state, result)
	pourMilkXZ(state, result)
	pourMilkZX(state, result)
	pourMilkYZ(state, result)
	pourMilkZY(state, result)
*/

int min(int num1, int num2){
	return num1 < num2 ? num1 : num2;
}

int max(int num1, int num2){
	return num1 > num2 ? num1 : num2;
}

int pourMilkXY(State state, State *result){
	if(state.x > 0 && state.y < TANK_CAPACITY_Y){
		result->x = max(0, (state.x - (TANK_CAPACITY_Y - state.y)));
		result->y = min(TANK_CAPACITY_Y, (state.x + state.y));
		result->z = state.z;
		return 1;
	}
	return 0;
}

int pourMilkYX(State state, State *result){
	if(state.y > 0 && state.x < TANK_CAPACITY_X){
		result->x = min(TANK_CAPACITY_X, (state.x + state.y));
		result->y = max(0, (state.y - (TANK_CAPACITY_X - state.x)));
		result->z = state.z;
		return 1;
	}
	return 0;
}

int pourMilkXZ(State state, State *result){
	if(state.x > 0 && state.z < TANK_CAPACITY_Z){
		result->x = max(0, (state.x - (TANK_CAPACITY_Z - state.z)));
		result->y = state.y;
		result->z = min(TANK_CAPACITY_Z, (state.x + state.z));
		return 1;
	}
	return 0;
}

int pourMilkZX(State state, State *result){
	if(state.z > 0 && state.x < TANK_CAPACITY_Z){
		result->x = min(TANK_CAPACITY_X, (state.x + state.z));
		result->y = state.y;
		result->z = max(0, (state.z - (TANK_CAPACITY_X - state.x)));
		return 1;
	}
	return 0;
}

int pourMilkYZ(State state, State *result){
	if(state.y > 0 && state.z < TANK_CAPACITY_Z){
		result->x = state.x;
		result->y = max(0, (state.y - (TANK_CAPACITY_Z - state.z)));
		result->z = min(TANK_CAPACITY_Z, (state.y + state.z));
		return 1;
	}
	return 0;
}

int pourMilkZY(State state, State *result){
	if(state.z > 0 && state.y < TANK_CAPACITY_Y){
		result->x = state.x;
		result->y = min(TANK_CAPACITY_Y, (state.y + state.z));
		result->z = max(0, (state.z - (TANK_CAPACITY_Y - state.y)));
		return 1;
	}
	return 0;
}

int callOperator(State state, State *result, int opt){
	switch(opt){
		case 1:
			return pourMilkXY(state, result);
		case 2:
			return pourMilkXZ(state, result);
		case 3:
			return pourMilkYX(state, result);
		case 4:
			return pourMilkYZ(state, result);
		case 5:
			return pourMilkZX(state, result);
		case 6:
			return pourMilkZY(state, result);
		default:
			cout << "Cannot call operator" << endl;
			return 0;
	}
}

struct Node {
	State state;
	Node* parent;
	int noFunction;
};

int findState(State state, stack<Node*> S){
	while(!S.empty()){
		Node *curNode = S.top();
		if(compareStates(curNode->state, state))
			return 1;
		S.pop();
	}
	return 0;
}

int findState(State state, queue<Node*> Q){
	while(!Q.empty()){
		Node *curNode = Q.front();
		if(compareStates(curNode->state, state))
			return 1;
		Q.pop();
	}
	return 0;
}

Node* DFSAlgorithm(State startState){
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = startState;
	root->parent = NULL;
	root->noFunction = 0;
	
	stack<Node*> openDFS;
	stack<Node*> closeDFS;
	
	openDFS.push(root);
	
	while(!openDFS.empty()){
		Node *node = openDFS.top();
		openDFS.pop();
		
		closeDFS.push(node);
		
		if(checkGoal(node->state))
			return node;
		
		for(int opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			initState(&newState);
			
			if(callOperator(node->state, &newState, opt)){
				if(findState(newState, openDFS) || findState(newState, closeDFS))
					continue;
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				
				openDFS.push(newNode);
			}
		}
	}
	return NULL;
}

Node* BFSAlgorithm(State startState){
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = startState;
	root->parent = NULL;
	root->noFunction = 0;
	
	queue<Node*> openBFS;
	queue<Node*> closeBFS;
	
	openBFS.push(root);
	
	while(!openBFS.empty()){
		Node *node = openBFS.front();
		openBFS.pop();
		
		closeBFS.push(node);
		
		if(checkGoal(node->state))
			return node;
		
		for(int opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			initState(&newState);
			
			if(callOperator(node->state, &newState, opt)){
				if(findState(newState, openBFS) || findState(newState, closeBFS))
					continue;
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				
				openBFS.push(newNode);
			}
		}
	}
	return NULL;
}

stack<Node*> tracingPath(Node *node){
	stack<Node*> listNodes;
	while(node->parent != NULL){
		listNodes.push(node);
		node = node->parent;
	}
	listNodes.push(node); // root node
	return listNodes;
}

void printPathFrom(stack<Node*> listNodes){
	int noAction = 0;
	while(!listNodes.empty()){
		Node* node = listNodes.top();
		cout << "----------------------------------------------" << endl;
		cout << endl << "Action " << noAction++ << ": " << actions[node->noFunction] << endl;
		printState(node->state);
//		cout << "----------------------------------------------" << endl;
		listNodes.pop();
	}
}

int main(){
	State startState;
	initState(&startState);
	Node *node = BFSAlgorithm(startState);
//	printState(node->state);
	stack<Node*> listNodes = tracingPath(node);
	printPathFrom(listNodes);
	/*
	State state1;
	pourMilkXY(startState, &state1);
	printState(state1);
	
	State state2;
	pourMilkYX(state1, &state2);
	printState(state2);
	
	State state3;
	pourMilkXZ(state2, &state3);
	printState(state3);
	
	State state4;
	pourMilkZX(state3, &state4);
	printState(state4);
	
	State state5;
	pourMilkZY(state3, &state5);
	printState(state5);
	
	State state6;
	pourMilkYZ(state5, &state6);
	printState(state6); */
	return 0;
}
