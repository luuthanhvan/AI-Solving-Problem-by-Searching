#include <iostream>
#include <stdlib.h>
#include <stack>
#include <queue>
#define OVERTURN 1 // lat ngua
#define UPSIDEDOWN 0 // lat up
#define MAX_OPERATOR 4
using namespace std;

/* 
Start state: 101010
Goal state: 111111
*/

const char* actions[] = {"First state", "Change glass 1, 2, 3", "Change glass 2, 3, 4",
						"Change glass 3, 4, 5", "Change glass 4, 5, 6"};

typedef struct{
	int glass[6];
} State;

State startState = {1, 0, 1, 0, 1, 0};

int checkGoal(State state){
	for(int i = 0; i < 6; i++)
		if(state.glass[i] != OVERTURN)
			return 0;
	return 1;
}

void printState(State state){
	cout << endl << "-------------------------" << endl;
	for(int i = 0; i < 6; i++)
		cout << "| " << state.glass[i] << " ";
	cout << "|" << endl << "-------------------------";
	cout << endl << endl << endl;
}

int compareState(State state1, State state2){
	for(int i = 0; i < 6; i++)
		if(state1.glass[i] != state2.glass[i])
			return 0;
	return 1;
}

void changeGlass123(State state, State *result){
	*result = state;
	result->glass[0] = !state.glass[0];
	result->glass[1] = !state.glass[1];
	result->glass[2] = !state.glass[2];
}

void changeGlass234(State state, State *result){
	*result = state;
	result->glass[1] = !state.glass[1];
	result->glass[2] = !state.glass[2];
	result->glass[3] = !state.glass[3];
}

void changeGlass345(State state, State *result){
	*result = state;
	result->glass[2] = !state.glass[2];
	result->glass[3] = !state.glass[3];
	result->glass[4] = !state.glass[4];
}

void changeGlass456(State state, State *result){
	*result = state;
	result->glass[3] = !state.glass[3];
	result->glass[4] = !state.glass[4];
	result->glass[5] = !state.glass[5];
}

void callOperator(State state, State *result, int opt){
	switch(opt){
		case 1:
			changeGlass123(state, result);
			break;
		case 2:
			changeGlass234(state, result);
			break;
		case 3:
			changeGlass345(state, result);
			break;
		case 4:
			changeGlass456(state, result);
			break;
		default:
			cout << "Cannot call operator" << endl;
			break;
	}
}

struct Node{
	State state;
	Node* parent;
	int no_function;
};

int findState(State state, stack<Node*> list){
	while(!list.empty()){
		Node *curNode = list.top();
		if(compareState(state, curNode->state))
			return 1;
		list.pop();
	}
	return 0;
}

int findState(State state, queue<Node*> list){
	while(!list.empty()){
		Node *curNode = list.front();
		if(compareState(state, curNode->state))
			return 1;
		list.pop();
	}
	return 0;
}

Node* DFSAlgorithm(State startState){
	Node *root = (Node*)malloc(sizeof(Node));
	root->state = startState;
	root->parent = NULL;
	root->no_function = 0;
	
	stack<Node*> openDFS;
	stack<Node*> closeDFS;
	
	openDFS.push(root);
	
//	printState(root->state);
	
	while(!openDFS.empty()){
		Node *node = openDFS.top();
		openDFS.pop();
		
		closeDFS.push(node);
		node->no_function;
		
//		printState(node->state);
		
		if(checkGoal(node->state))
			return node;
		
		for(int opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			callOperator(node->state, &newState, opt);
			
//			printState(newState);
		
			if(findState(newState, openDFS) || findState(newState, closeDFS))
				continue;
			
			Node *newNode = (Node*)malloc(sizeof(Node));
			newNode->state = newState;
			newNode->parent = node;
			newNode->no_function = opt;
				
			openDFS.push(newNode);
		}
	}
	
	return  NULL;
}

Node* BFSAlgorithm(State startState){
	Node *root = (Node*)malloc(sizeof(Node));
	root->state = startState;
	root->parent = NULL;
	root->no_function = 0;
	
	queue<Node*> openBFS;
	queue<Node*> closeBFS;
	
	openBFS.push(root);
	
//	printState(root->state);
	
	while(!openBFS.empty()){
		Node *node = openBFS.front();
		openBFS.pop();
		
		closeBFS.push(node);
		
//		printState(node->state);
		
		if(checkGoal(node->state))
			return node;
		
		for(int opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			callOperator(node->state, &newState, opt);
			
//			printState(newState);
		
			if(findState(newState, openBFS) || findState(newState, closeBFS))
				continue;
			
			Node *newNode = (Node*)malloc(sizeof(Node));
			newNode->state = newState;
			newNode->parent = node;
			newNode->no_function = opt;
				
			openBFS.push(newNode);
		}
	}
	
	return  NULL;
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
		cout << "Action " << noAction++ << ": " << actions[node->no_function];
		printState(node->state);
		listNodes.pop();
	}
}

int main(){
	Node *node = DFSAlgorithm(startState);
//	printState(node->state);
	stack<Node*> listNodes = tracingPath(node);
	printPathFrom(listNodes);
	
	/*
	State result;
	cout << "First state: " << endl;
	printState(startState);
	for(int opt = 1; opt <= MAX_OPERATOR; opt++){
		callOperator(startState, &result, opt);
		cout << actions[opt] << endl;
		printState(result);
	} */
	return 0;
}

