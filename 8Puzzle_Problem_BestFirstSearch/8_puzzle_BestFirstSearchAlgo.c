// Program to solving 8 puzzle problem by using heuristic searching
#include <stdio.h>
#include <stdlib.h>
#define ROWS 3
#define COLS 3
#define EMPTY_CELL 0
#define MAX_OPERATOR 4
#define MAX_LENGTH 500

const char* actions[] = {"First state", "Move cell EMPTY to UP", "Move cell EMPTY to DOWN", 
						"Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};

// declare state structure for 8 puzzle problem
typedef struct {
	int eightPuzzle[ROWS][COLS];
	int emptyRow, emptyCol;
} State;

void printState(State state){
	int row, col;
	printf("\n--------\n");
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			printf("%d  ", state.eightPuzzle[row][col]);
		}
		printf("\n");
	}
	printf("--------\n");
}

State readDataFromFile(const char *fileName){
	FILE *file = fopen(fileName, "r");
	if(file == NULL){
		printf("Error! Could not open file");
		exit(1);  // program exits if file pointer returns NULL -> need include stdlib.h
	}
	State state;
	int row, col;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++)
			fscanf(file, "%d", &state.eightPuzzle[row][col]);
	}
	fscanf(file, "%d%d", &state.emptyRow, &state.emptyCol);
	fclose(file);
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
			printf("Cannot call operator!");
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
	struct Node* parent;
	int noFunction;
	int heuristic;
};
typedef struct Node* Node;

// implement List
typedef struct{
	Node element[MAX_LENGTH];
	int size;
} List;

void makeNullList(List *L){
	L->size = 0;
}

int emptyList(List L){
	return L.size == 0;
}

int fullList(List L){
	return L.size == MAX_LENGTH;
}

// get the element at position p
Node elementAt(List L, int p){
	return L.element[p-1];
}

// add new element at position p
void pushList(List *L, Node element, int p){
	if(!fullList(*L)){
		int q;
		for(q = L->size; q >= p; q--){
			L->element[q] = L->element[q-1];
		}
		L->element[p-1] = element;
		L->size++;
	}
	else 
		printf("List is full!\n");
}

// delete the element at position p
void deleteList(List *L, int p){
	if(emptyList(*L)){
		printf("List is empty!\n");
	}
	else if(p < 1 || p > L->size){
		printf("Position is not possible to delete!\n");
	}
	else {
		int q;
		for(q = p-1; q < L->size; q++){
			L->element[q] = L->element[q+1];
		}
		L->size--;
	}
}

// finding a state is whether exist in OPEN/CLOSE list or not
// store the position of the state in pointer p
Node findState(State state, List L, int* p){
	int i;
	for(i = 1; i <= L.size; i++){
		if(compareState(state, elementAt(L,i)->state)){
			*p = i;
			return elementAt(L,i);
		}
	}
	return NULL;
}

// sorting the list by heuristic ascending 
void sort(List *L){
	int i, j;
	for(i = 0; i < L->size-1; i++){
		for(j = i+1; j < L->size; j++)
			if(L->element[i]->heuristic > L->element[j]->heuristic){
				Node temp = L->element[i];
				L->element[i] = L->element[j];
				L->element[j] = temp;
			}
	}
}

// Best First Search algorithm using heuristic1 function
Node bestFirstSearch1(State state, State goal){
	Node root = (Node)malloc(sizeof(struct Node));
	root->state = state;
	root->parent = NULL;
	root->noFunction = 0;
	root->heuristic = heuristic1(state, goal);
	
	List openBFS, closeBFS;
	makeNullList(&openBFS);
	makeNullList(&closeBFS);
	
	pushList(&openBFS, root, openBFS.size+1);
	
	while(!emptyList(openBFS)){
		Node node = elementAt(openBFS, 1);
		deleteList(&openBFS, 1);
		
		if(checkGoal(node->state, goal))
			return node;
		pushList(&closeBFS, node, closeBFS.size+1);
		int opt;
		for(opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				Node newNode = (Node)malloc(sizeof(struct Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				newNode->heuristic = heuristic1(newState, goal);
				
				int posOpen, posClose;
				Node nodeFoundOpen = findState(newState, openBFS, &posOpen);
				Node nodeFoundClose = findState(newState, closeBFS, &posClose);
			
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					pushList(&openBFS, newNode, openBFS.size+1);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					deleteList(&openBFS, posOpen);
					pushList(&openBFS, newNode, posOpen);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					deleteList(&closeBFS, posClose);
					pushList(&openBFS, newNode, openBFS.size+1);
				}
				
				sort(&openBFS);
			}	
		}
	}
	return NULL;
}

// Best First Search algorithm using heuristic2 function
Node bestFirstSearch2(State startState, State goalState){
	Node root = (Node)malloc(sizeof(struct Node));
	root->state = startState;
	root->parent = NULL;
	root->noFunction = 0;
	root->heuristic = heuristic2(startState, goalState);
	
	List openBFS, closeBFS;
	makeNullList(&openBFS);
	makeNullList(&closeBFS);
	
	pushList(&openBFS, root, openBFS.size+1);
	
	while(!emptyList(openBFS)){
		Node node = elementAt(openBFS, 1);
		deleteList(&openBFS, 1);
		pushList(&closeBFS, node, closeBFS.size+1);
		
		if(checkGoal(node->state, goalState))
			return node;
		
		int opt;
		State newState;
		for(opt = 1; opt <= MAX_OPERATOR; opt++){
			if(callOperators(node->state, &newState, opt)){
				Node newNode = (Node)malloc(sizeof(struct Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				newNode->heuristic = heuristic2(newState, goalState);
				
				int posOpen, posClose;
				Node nodeFoundOpen = findState(newState, openBFS, &posOpen);
				Node nodeFoundClose = findState(newState, closeBFS, &posClose);
				
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					pushList(&openBFS, newNode, openBFS.size+1);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					deleteList(&openBFS, posOpen);
					pushList(&openBFS, newNode, openBFS.size+1);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					deleteList(&closeBFS, posClose);
					pushList(&openBFS, newNode, openBFS.size+1);
				}
				
				sort(&openBFS);
			}
		}
	}
	return NULL;
}

List tracingPath(Node node){
	List list;
	makeNullList(&list);
	while(node->parent != NULL){
		pushList(&list, node, list.size+1);
		node = node->parent;
	}
	pushList(&list, node, list.size+1);
	return list;
}

void printWaysToGetGoal(List list){
	int i, noAction = 0;
	for(i = list.size; i > 0; i--){
		Node node = elementAt(list, i);
		printf("\nAction %d: %s", noAction, actions[node->noFunction]);
		printState(node->state);
		noAction++;
	}
}

int main(){
	State startState = readDataFromFile("start_state.txt");
	State goalState = readDataFromFile("goal_state.txt");
//	Node node = bestFirstSearch1(startState, goalState);
	Node node = bestFirstSearch2(startState, goalState);
//	printState(node->state);
	List list = tracingPath(node);
	printWaysToGetGoal(list);
	return 0;
}
