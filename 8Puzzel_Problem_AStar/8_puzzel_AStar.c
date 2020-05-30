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
	int eightPuzzel[ROWS][COLS];
	int emptyRow, emptyCol;
} State;

void printState(State state){
	int row, col;
	printf("\n--------\n");
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			printf("%d  ", state.eightPuzzel[row][col]);
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
			fscanf(file, "%d", &state.eightPuzzel[row][col]);
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
			if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
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
		result->eightPuzzel[emptyRowCurrent - 1][emptyColCurrent] = EMPTY_CELL;
		// update the value of old empty cell
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent - 1][emptyColCurrent];
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
		result->eightPuzzel[emptyRowCurrent + 1][emptyColCurrent] = EMPTY_CELL;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent + 1][emptyColCurrent];
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
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent - 1] = EMPTY_CELL;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent][emptyColCurrent - 1];
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
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent + 1] = EMPTY_CELL;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent][emptyColCurrent + 1];
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
			if(state.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
				count++;
	return count;
}

// way 2: counting the number of moves from cell position uncorrect to cell position correct
int heuristic2(State state, State goal){
	int row, col, count = 0;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(state.eightPuzzel[row][col] != EMPTY_CELL){
				int row_g, col_g;
				for(row_g = 0; row_g < ROWS; row_g++){
					for(col_g = 0; col_g < COLS; col_g++){
						if(state.eightPuzzel[row][col] == goal.eightPuzzel[row_g][col_g]){
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
	int h;
	int g;
	int f; // f = g + h
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
			if(L->element[i]->f > L->element[j]->f){
				Node temp = L->element[i];
				L->element[i] = L->element[j];
				L->element[j] = temp;
			}
	}
}

void insertionSort(List *L)  {  
    int i, j;
    Node key;
    for (i = 1; i < L->size; i++){  
        key = L->element[i];  
        j = i - 1;  
        while (j >= 0 && L->element[j]->f > key->f) {  
            L->element[j + 1] = L->element[j];  
            j = j - 1;  
        }  
        L->element[j + 1] = key;  
    }  
}

void printOpenList(List L){
	int i;
	for(i = 0; i < L.size; i++){
		printState(L.element[i]->state);
		printf("h = %d\n", L.element[i]->h);
		printf("f = %d\n", L.element[i]->f);
	}
}

// Best First Search algorithm using heuristic1 function
Node AStar(State state, State goal){
	Node root = (Node)malloc(sizeof(struct Node));
	root->state = state;
	root->parent = NULL;
	root->noFunction = 0;
	root->g = 0;
	root->h = heuristic1(state, goal);
	root->f = root->g + root->h;
	
	List openAStar, closeAStar;
	makeNullList(&openAStar);
	makeNullList(&closeAStar);
	
	pushList(&openAStar, root, openAStar.size+1);
	
	// int count = 0;
	while(!emptyList(openAStar)){
		// count++;
		Node node = elementAt(openAStar, 1);
		deleteList(&openAStar, 1);
		
		if(checkGoal(node->state, goal))
			return node;
		pushList(&closeAStar, node, closeAStar.size+1);
		
		// if(count == 5) printOpenList(openAStar);
		
		int opt;
		for(opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				Node newNode = (Node)malloc(sizeof(struct Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->noFunction = opt;
				newNode->g = node->g + 1;
				newNode->h = heuristic1(newState, goal);
				newNode->f = newNode->g + newNode->h;
				
				int posOpen, posClose;
				Node nodeFoundOpen = findState(newState, openAStar, &posOpen);
				Node nodeFoundClose = findState(newState, closeAStar, &posClose);
			
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					pushList(&openAStar, newNode, openAStar.size+1);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g){
					deleteList(&openAStar, posOpen);
					pushList(&openAStar, newNode, posOpen);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->g > newNode->g){
					deleteList(&closeAStar, posClose);
					pushList(&openAStar, newNode, openAStar.size+1);
				}
				sort(&openAStar);
				// insertionSort(&openAStar);
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
	Node node = AStar(startState, goalState);
//	printState(node->state);
	List list = tracingPath(node);
	printWaysToGetGoal(list);
	return 0;
}
