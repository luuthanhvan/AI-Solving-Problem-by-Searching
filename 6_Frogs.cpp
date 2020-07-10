#include <iostream>
#include <stack>
#include <stdlib.h>
#define YELLOW_FROG 1
#define BROWN_FROG 2
#define EMPTY_SLATE 0
#define MAX_OPERATOR 4
using namespace std;

const char *actions[] = {"First state", "Jump forward from left to right", "Jump forward from right to left",
"Jump over from left to right", "Jump over from right to left"};

typedef struct{
	int slate[7]; // mang cac phien da
	int posEmpSlate; // vi tri phien da trong
} State;

void initState(State *state){
	for(int i = 0; i < 3; i++){
		state->slate[i] = YELLOW_FROG; // khoi tao mang o vi tri 0, 1, 2 co gia tri la 1
		for(int j = 4; j < 7; j++){
			state->slate[j] = BROWN_FROG; // khoi tao mang o vi tri 4, 5, 6 co gia tri la 2
		}
	}
	state->slate[3] = EMPTY_SLATE;
	state->posEmpSlate = 3; // vi tri phien da trong la o vi tri 3
}

void printState(State state){
	cout << "------------------------------------------------" << endl;
	for(int i = 0; i < 7; i++){
		cout << " | " << state.slate[i] << " | ";
	}
	cout << endl;
	cout << "------------------------------------------------" << endl;
}

bool compareStates(State state1, State state2){
	for(int i = 0; i < 7; i++){
		if(state1.slate[i] != state2.slate[i])
			return false;
	}
	return true;
}

bool checkGoal(State state){
	for(int i = 0; i < 3; i++){
		for(int j = 4; j < 7; j++){
			if(state.slate[i] != BROWN_FROG || state.slate[j] != YELLOW_FROG || state.posEmpSlate != 3)
				return false;
		}
	}
	return true;
}

void swap(int *num1, int *num2){
	int temp;
	temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

// a frog jump forward from left to right
bool jumpForwardLR(State state, State *result){
	*result = state;
	for(int i = 0; i < 7; i++){
		if(result->slate[i] == YELLOW_FROG){
			if((result->posEmpSlate - i) == 1){
				swap(result->slate[i], result->slate[result->posEmpSlate]);
				result->posEmpSlate = i;
				return true;
			}
		}
	}
	return false;
}

// a frog jump forward from right to left
bool jumpForwardRL(State state, State *result){
	*result = state;
	for(int i = 0; i < 7; i++){
		if(result->slate[i] == BROWN_FROG){
			if((i - result->posEmpSlate) == 1){
				swap(result->slate[i], result->slate[result->posEmpSlate]);
				result->posEmpSlate = i;
				return true;
			}
		}
	}
	return false;
}

// frog jump over from left to right
bool jumpOverLR(State state, State *result){
	*result = state;
	for(int i = 0; i < 7; i++){
		if(result->slate[i] == YELLOW_FROG){
			if((result->posEmpSlate - i) == 2 && (result->slate[i] != result->slate[i+1])){
				swap(result->slate[i], result->slate[result->posEmpSlate]);
				result->posEmpSlate = i;
				return true;
			}
		}
	}
	return false;
}

// frog jump over from right to left
bool jumpOverRL(State state, State *result){
	*result = state;
	for(int i = 0; i < 7; i++){
		if(result->slate[i] == BROWN_FROG){
			if((i - result->posEmpSlate) == 2 && result->slate[i] != result->slate[i-1]){
				swap(result->slate[i], result->slate[result->posEmpSlate]);
				result->posEmpSlate = i;
				return true;
			}
		}
	}
	return false;
}

bool callOperator(State state, State *result, int opt){
	switch(opt){
		case 1:
			return jumpForwardLR(state, result);
		case 2:
			return jumpForwardRL(state, result);
		case 3:
			return jumpOverLR(state, result);
		case 4:
			return jumpOverRL(state, result);
		default:
			cout << "Cannot call operator!" << endl;
			return false;
	}
}

struct Node{
	State state;
	Node* parent;
	int noFunction;
};

bool findState(State state, stack<Node*> S){
	while(!S.empty()){
		Node* curNode = S.top();
		if(compareStates(state, curNode->state))
			return true;
		S.pop();
	}
	return false;
}

Node* dfsAlgorithm(State startState){
	Node *root = (Node*)malloc(sizeof(struct Node));
	root->state = startState;
	root->parent = NULL;
	root->noFunction = 0;
	
	stack<Node*> openDFS;
	stack<Node*> closeDFS;
	
	openDFS.push(root);
	
//	printState(root->state);

	while(!openDFS.empty()){
		Node *node = openDFS.top();
		openDFS.pop();
		
		closeDFS.push(node);
		
		if(checkGoal(node->state))
			return node;
//		printState(node->state);
		
		for(int opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			if(callOperator(node->state, &newState, opt)){
				
//				printState(newState);
				
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

void printWaysToGetGoal(Node *node){
	stack<Node*> stackPrint;
	while(node->parent != NULL){
		stackPrint.push(node);
		node = node->parent;
	}
	stackPrint.push(node); // root node
	int noAction = 0;
	while(!stackPrint.empty()){
		Node* curNode = stackPrint.top();
		stackPrint.pop();
		cout << endl << endl << "Action " << noAction++ << ": " << actions[curNode->noFunction] << endl; 		
		printState(curNode->state);
	}
}

int main(){
	State startState;
	initState(&startState);
	
//	cout << "Start state: " << endl;
//	printState(startState);
//	cout << "Goal state: " << endl;
	Node* result = dfsAlgorithm(startState);
//	printState(result->state);
	printWaysToGetGoal(result);
	
	/*
	State state1;
	jumpForwardLR(startState, &state1);
	cout << "State 1: " << endl;
	printState(state1);
	
	State state2;
	jumpForwardRL(state1, &state2);
	cout << "State 2: " << endl;
	printState(state2);
	
	State state3;
	jumpOverRL(state2, &state3);
	cout << "State 3: " << endl;
	printState(state3);
	
	State state4;
	jumpOverLR(state3, &state4);
	cout << "State 4: " << endl;
	printState(state4); */
	return 0;
}
