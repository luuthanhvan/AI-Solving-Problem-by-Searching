// Program to solving 8 puzzle problem by using heuristic searching
#include <iostream>
#include <fstream> // ifstream
#include <stdlib.h> // exit()
#include <algorithm> // std::sort
#include <vector> // std::vector
#define ROWS 3
#define COLS 3
#define EMPTY_CELL 0
#define MAX_OPERATOR 4
#define MAX_LENGTH 500
using namespace std;

const char* actions[] = {"First state", "Move cell EMPTY to UP", "Move cell EMPTY to DOWN", 
						"Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};

// declare state structure for 8 puzzle problem
typedef struct {
	int eightPuzzle[ROWS][COLS];
	int emptyRow, emptyCol;
} State;

void printState(State state){
	cout << endl << "--------" << endl;
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			cout << state.eightPuzzle[row][col] << "  ";
		}
		cout << endl;
	}
	cout << "--------" << endl;
}

State readDataFromFile(const char* fileName){
	ifstream file;
	file.open(fileName);
	// fileName can have string type and change the line 35 to file.open(fileName.c_str()) if using C++ 11
	if(!file){
		cout << "Error! Could not open file";
		exit(1);  // terminate with error
	}
	State state;
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++)
			file >> state.eightPuzzle[row][col];
	}
	file >> state.emptyRow >> state.emptyCol;
	file.close();
	return state;
}

int compareState(State state1, State state2){
	int row, col;
	if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
		return 0;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++)
			if(state1.eightPuzzle[row][col] != state2.eightPuzzle[row][col])
				return 0;
	}
	return 1;
}

int checkGoal(State state, State goal){
	return compareState(state, goal);
}

// contruct 4 operators: move the empty cell UP/LEFT/RIGHT/DOWN
int upOperator(State state, State* result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;
	
	if(emptyRowCurrent > 0){
		result->emptyCol = emptyColCurrent; // position of column does not change
		result->emptyRow = emptyRowCurrent - 1;
		// update the value of new empty cell
		result->eightPuzzle[emptyRowCurrent - 1][emptyColCurrent] = EMPTY_CELL;
		// update the value of old empty cell
		result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent - 1][emptyColCurrent];
		return 1;
	}
	return 0;
}

int downOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;
	
	if(emptyRowCurrent < 2){
		result->emptyCol = emptyColCurrent;
		result->emptyRow = emptyRowCurrent + 1;
		result->eightPuzzle[emptyRowCurrent + 1][emptyColCurrent] = EMPTY_CELL;
		result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent + 1][emptyColCurrent];
		return 1;
	}
	return 0;
}

int leftOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;
	
	if(emptyColCurrent > 0){
		result->emptyRow = emptyRowCurrent; // position of row does not change
		result->emptyCol = emptyColCurrent - 1;
		result->eightPuzzle[emptyRowCurrent][emptyColCurrent - 1] = EMPTY_CELL;
		result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent - 1];
		return 1;
	}
	return 0;
}

int rightOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;
	
	if(emptyColCurrent < 2){
		result->emptyRow = emptyRowCurrent;
		result->emptyCol = emptyColCurrent + 1;
		result->eightPuzzle[emptyRowCurrent][emptyColCurrent + 1] = EMPTY_CELL;
		result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent + 1];
		return 1;
	}
	return 0;
}

int callOperators(State state, State *result, int noOperator){
	switch(noOperator){
		case 1:
			return upOperator(state, result);
		case 2:
			return downOperator(state, result);
		case 3:
			return leftOperator(state, result);
		case 4:
			return rightOperator(state, result);
		default:
			cout << "Cannot call operator!";
			return 0;
	}
}

int abs(int x){
	if(x >= 0)
		return x;
	return -x;
}

// calculating heuristic
// way 1: counting the number of cells that are different from the goal state
int heuristic1(State state, State goal){
	int row, col, count = 0;
	for(row = 0; row < ROWS; row++)
		for(col = 0; col < COLS; col++)
			if(state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
				count++;
	return count;
}

// way 2: counting the number of moves from cell position uncorrect to cell position correct
int heuristic2(State state, State goal){
	int row, col, count = 0;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(state.eightPuzzle[row][col] != EMPTY_CELL){
				int row_g, col_g;
				for(row_g = 0; row_g < ROWS; row_g++){
					for(col_g = 0; col_g < COLS; col_g++){
						if(state.eightPuzzle[row][col] == goal.eightPuzzle[row_g][col_g]){
							count += abs(row - row_g) + abs(col - col_g);
							row_g = ROWS; // break loop row_g
							col_g = COLS; // break loop col_g
						}		
					}
				}
			}	
		}
	}
	return count;
}

// Node structure to contruct Tree search
struct Node{
	State state;
	Node* parent;
	int noFunction;
	int heuristic;
};

// finding a state is whether exist in OPEN/CLOSE list or not
// store the position of the state in pointer p
Node* findState(State state, vector<Node*> list, vector<Node*>::iterator *p){
	vector<Node*>::iterator it = list.begin();
	
	if(list.size() == 0)
		return NULL;
		
	while(it != list.end()){
		if(compareState(state, (*it)->state)){
			*p = it;
			return *it;
		}
		it = list.erase(it);
	}
	return NULL;
}

bool compareHeuristic(Node *node1, Node* node2){
	return node1->heuristic > node2->heuristic;
}

// Best First Search algorithm using heuristic1 function
Node* bestFirstSearch1(State state, State goal){
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->noFunction = 0;
	root->heuristic = heuristic1(state, goal);
	
	vector<Node*> openBFS(MAX_LENGTH);
	openBFS.clear();
	vector<Node*> closeBFS(MAX_LENGTH);
	closeBFS.clear();
	
	openBFS.push_back(root);
	
	while(!openBFS.empty()){
		Node* node = openBFS.back();
		openBFS.pop_back();
		closeBFS.push_back(node);
		
		if(checkGoal(node->state, goal))
			return node;
		
		int opt;
		for(opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				newNode->heuristic = heuristic1(newState, goal);
				
				vector<Node*>::iterator posOpen, posClose;
				Node *nodeFoundOpen = findState(newState, openBFS, &posOpen);
				Node *nodeFoundClose = findState(newState, closeBFS, &posClose);
			
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					openBFS.push_back(newNode);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					openBFS.erase(posOpen);
					openBFS.push_back(newNode);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					closeBFS.erase(posClose);
					openBFS.push_back(newNode);
				}
				sort(openBFS.begin(), openBFS.end(), compareHeuristic);
			}	
		}
	}
	return NULL;
}

// Best First Search algorithm using heuristic2 function
Node* bestFirstSearch2(State startState, State goalState){
	Node *root = (Node*)malloc(sizeof(Node));
	root->state = startState;
	root->parent = NULL;
	root->noFunction = 0;
	root->heuristic = heuristic2(startState, goalState);
	
	vector<Node*> openBFS;
	openBFS.clear();
	vector<Node*> closeBFS;
	closeBFS.clear();
	
	openBFS.push_back(root);
	
	while(!openBFS.empty()){
		Node *node = openBFS.back();
		openBFS.pop_back();
		closeBFS.push_back(node);
		
		if(checkGoal(node->state, goalState))
			return node;
		
		int opt;
		State newState;
		for(opt = 1; opt <= MAX_OPERATOR; opt++){
			if(callOperators(node->state, &newState, opt)){
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				newNode->heuristic = heuristic2(newState, goalState);
				
				vector<Node*>::iterator posOpen, posClose;
				Node *nodeFoundOpen = findState(newState, openBFS, &posOpen);
				Node *nodeFoundClose = findState(newState, closeBFS, &posClose);
				
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					openBFS.push_back(newNode);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					openBFS.erase(posOpen);
					openBFS.push_back(newNode);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					openBFS.erase(posClose);
					openBFS.push_back(newNode);
				}
				
				sort(openBFS.begin(), openBFS.end(), compareHeuristic);
			}
		}
	}
	return NULL;
}

vector<Node*> tracingPath(Node* node){
	vector<Node*> list;
	while(node->parent != NULL){
		list.push_back(node);
		node = node->parent;
	}
	list.push_back(node);
	return list;
}

void printWaysToGetGoal(vector<Node*> list){
	int i, noAction = 0;
	for(i = list.size()-1; i >= 0; i--){
		Node *node = list.at(i);
		cout << endl << "Action " << noAction << ": " <<  actions[node->noFunction];
		printState(node->state);
		noAction++;
	}
}

int main(){
	State startState = readDataFromFile("start_state.txt");
	State goalState = readDataFromFile("goal_state.txt");
//	Node *node = bestFirstSearch1(startState, goalState);
	Node *node = bestFirstSearch2(startState, goalState);
	printState(node->state);
	vector<Node*> list = tracingPath(node);
	printWaysToGetGoal(list);
	return 0;
}
