#include <iostream>
#include <queue>
#include <stack>
#include <stdlib.h>
#define MAX_MONKS 3
#define MAX_DEMONS 3
#define MAX_OPERATOR 5
using namespace std;

const char* actions[] = {"First state", "Moving 0 Monk and 1 Demon", "Moving 0 Monk and 2 Demons",
"Moving 1 Monk and 0 Demon", "Moving 1 Monk and 1 Demon", "Moving 2 Monks and 0 Demon"};

typedef struct{
	int noMonks;
	int noDemons;
	char posBoat;
} State;

void initState(State *S){
	S->noMonks = MAX_MONKS;
	S->noDemons = MAX_DEMONS;
	S->posBoat = 'A';
}

void printState(State S){
	cout << endl << "----------------------------------------------------------------------";
	cout << endl << "Riverside A					Riverside B";
	cout << endl << "Number of Monks: " << S.noMonks << "				" << "Number of Monks: " << (MAX_MONKS - S.noMonks);
	cout << endl << "Number of Demons: " << S.noDemons << "				" << "Number of Demons: " << (MAX_DEMONS - S.noDemons);
	cout << endl << "Boat position: " << S.posBoat;
	cout << endl << "----------------------------------------------------------------------";
}

int compareStates(State S1, State S2){
	return (S1.noMonks == S2.noMonks && S1.noDemons == S2.noDemons && S1.posBoat == S2.posBoat);
}

int checkGoal(State S){
	return (S.noMonks == 0 && S.noDemons == 0 && S.posBoat == 'B');
}

int move0Monk1Demon(State state, State *result){
	*result = state;
	if(state.posBoat == 'A' && state.noDemons > 0){
		result->noDemons = state.noDemons - 1;
		result->posBoat = 'B';
	}
	else if(state.posBoat == 'B' && (MAX_DEMONS - state.noDemons) > 0 ){
		result->noDemons = state.noDemons + 1;
		result->posBoat = 'A';
	}
	
	if(((result->noDemons < result->noMonks && result->noMonks == MAX_MONKS) ||
		(result->noDemons == result->noMonks) ||
		(result->noDemons > result->noMonks && result->noMonks == 0)) 
		&& !compareStates(state, *result))
		return 1;
	return 0;
}

int move0Monk2Demons(State state, State *result){
	*result = state;
	if(state.posBoat == 'A' && state.noDemons > 1){
		result->noDemons = state.noDemons - 2;
		result->posBoat = 'B';
	}
	else if(state.posBoat == 'B' && (MAX_DEMONS - state.noDemons) > 1){
		result->noDemons = state.noDemons + 2;
		result->posBoat = 'A';
	}
	
	if(((result->noDemons < result->noMonks && result->noMonks == MAX_MONKS) ||
		(result->noDemons == result->noMonks) ||
		(result->noDemons > result->noMonks && result->noMonks == 0)) 
		&& !compareStates(state, *result))
		return 1;
	return 0;
}

int move1Monk0Demon(State state, State *result){
	*result = state;
	if(state.posBoat == 'A' && state.noMonks > 0){
		result->noMonks = state.noMonks - 1;
		result->posBoat = 'B';
	}
	else if(state.posBoat == 'B' && (MAX_MONKS - state.noMonks) > 0){
		result->noMonks = state.noMonks + 1;
		result->posBoat = 'A';
	}
	
	if(((result->noDemons < result->noMonks && result->noMonks == MAX_MONKS) ||
		(result->noDemons == result->noMonks) ||
		(result->noDemons > result->noMonks && result->noMonks == 0)) 
		&& !compareStates(state, *result))
		return 1;
	return 0;
}

int move1Monk1Demon(State state, State *result){
	*result = state;
	if(state.posBoat == 'A' && state.noMonks > 0 && state.noDemons > 0){
		result->noMonks = state.noMonks - 1;
		result->noDemons = state.noDemons - 1;
		result->posBoat = 'B';
	}
	else if(state.posBoat == 'B' && (MAX_MONKS - state.noMonks) > 0 && (MAX_DEMONS - state.noDemons) > 0){
		result->noMonks = state.noMonks + 1;
		result->noDemons = state.noDemons + 1;
		result->posBoat = 'A';
	}
	
	if(((result->noDemons < result->noMonks && result->noMonks == MAX_MONKS) ||
		(result->noDemons == result->noMonks) ||
		(result->noDemons > result->noMonks && result->noMonks == 0)) 
		&& !compareStates(state, *result))
		return 1;
	return 0;
}

int move2Monks0Demon(State state, State *result){
	*result = state;
	if(state.posBoat == 'A' && state.noMonks > 1){
		result->noMonks = state.noMonks - 2;
		result->posBoat = 'B';
	}
	else if(state.posBoat == 'B' && (MAX_MONKS - state.noMonks) > 1){
		result->noMonks = state.noMonks + 2;
		result->posBoat = 'A';
	}
	
	if(((result->noDemons < result->noMonks && result->noMonks == MAX_MONKS) ||
		(result->noDemons == result->noMonks) ||
		(result->noDemons > result->noMonks && result->noMonks == 0)) 
		&& !compareStates(state, *result))
		return 1;
	return 0;
}

int callOperator(State state, State *result, int opt){
	switch(opt){
		case 1:
			return move0Monk1Demon(state, result);
		case 2:
			return move0Monk2Demons(state, result);
		case 3:
			return move1Monk0Demon(state, result);
		case 4:
			return move1Monk1Demon(state, result);
		case 5:
			return move2Monks0Demon(state, result);
		default: 
			cout << "Cannot call operator" << endl;
			return 0;
	}
}

typedef struct Node{
	State state;
	struct Node* parent;
	int noFunction;
} Node;

int findState(State state, queue<Node*> Q){
	while(!Q.empty()){
		Node* curNode = Q.front();
		if(compareStates(state, curNode->state))
			return 1;
		Q.pop();
	}
	return 0;
}

int findState(State state, stack<Node*> S){
	while(!S.empty()){
		Node* curNode = S.top();
		if(compareStates(state, curNode->state))
			return 1;
		S.pop();
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
		if(checkGoal(node->state))
			return node;
		int opt;
		for(opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
//			printState(newState);
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

Node* DFSAlgorithm(State startState){
	Node *root = (Node*)malloc(sizeof(Node));
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
//		noAction++;
	}
}

int main(){
	State startState, result;
	initState(&startState);
	
	Node *node = DFSAlgorithm(startState);
//	printState(node->state);
	printWaysToGetGoal(node);
	
	/*
	cout << "Start state: " << endl;
	printState(startState);
	int opt;
	for(opt = 1; opt <= 5; opt++){
		if(callOperator(startState, &result, opt)){
			if(!compareStates(startState,result)){
				cout << "Action " << actions[opt] << " execute successfully" << endl;
				printState(result);
			}
		}
		else{
			cout << "Action " << actions[opt] << " does not execute successfully" << endl << endl;
		}
	} */
}
