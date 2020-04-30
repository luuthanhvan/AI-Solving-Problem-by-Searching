#include <stdio.h>
#include <malloc.h>
#define MAX_ROW 3
#define MAX_COL 3
#define EMPTY 0
#define MAXLENGTH 50

typedef struct{
	int eightPuzzles[MAX_ROW][MAX_COL];
	int emptyRow, emptyCol;
} State;

State goalState = { 1, 2, 3,
					8, 0, 4,
					7, 6, 5,
					1, 1 };

State startState = { 2, 3, 4,
					1, 8, 0,
					7, 6, 5,
					1, 2 };

void makeNullState(State *state){
	int i, j;
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			state->eightPuzzles[i][j] = 0;
	state->emptyRow = -1;
	state->emptyCol = -1;
}

int compareState(State state1, State state2){
	int i, j;
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			if(state1.eightPuzzles[i][j] != state2.eightPuzzles[i][j])
				return 0;
	return 1;
}

int checkGoal(State state){
	int i, j;
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			if(state.eightPuzzles[i][j] != goalState.eightPuzzles[i][j])
				return 0;
	return 1;
}

void printState(State state){
	int i, j;
	for(i = 0; i < MAX_ROW; i++){
		for(j = 0; j < MAX_COL; j++){
			printf("%d ", state.eightPuzzles[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void upOperator(State state, State *result){
	int currentEmpRow = state.emptyRow;
	int currentEmpCol = state.emptyCol;
	
	if(state.emptyRow > 0){
		*result = state;
		result->emptyRow = state.emptyRow-1;
		result->emptyCol = state.emptyCol;
		
		result->eightPuzzles[currentEmpRow][currentEmpCol] = state.eightPuzzles[currentEmpRow-1][currentEmpCol];
		result->eightPuzzles[currentEmpRow-1][currentEmpCol] = EMPTY;
	}
}

void downOperator(State state, State *result){
	int currentEmpRow = state.emptyRow;
	int currentEmpCol = state.emptyCol;
	
	if(state.emptyRow < 2){
		*result = state;
		result->emptyRow = state.emptyRow+1;
		result->emptyCol = state.emptyCol;
		
		result->eightPuzzles[currentEmpRow][currentEmpCol] = state.eightPuzzles[currentEmpRow+1][currentEmpCol];
		result->eightPuzzles[currentEmpRow+1][currentEmpCol] = EMPTY;
	}
}

void leftOperator(State state, State *result){
	int currentEmpRow = state.emptyRow;
	int currentEmpCol = state.emptyCol;
	
	if(state.emptyCol > 0){
		*result = state;
		result->emptyRow = state.emptyRow;
		result->emptyCol = state.emptyCol-1;
		
		result->eightPuzzles[currentEmpRow][currentEmpCol] = state.eightPuzzles[currentEmpRow][currentEmpCol-1];
		result->eightPuzzles[currentEmpRow][currentEmpCol-1] = EMPTY;
	}
}

void rightOperator(State state, State *result){
	int currentEmpRow = state.emptyRow;
	int currentEmpCol = state.emptyCol;
	
	if(state.emptyCol < 2){
		*result = state;
		result->emptyRow = state.emptyRow;
		result->emptyCol = state.emptyCol+1;
		
		result->eightPuzzles[currentEmpRow][currentEmpCol] = state.eightPuzzles[currentEmpRow][currentEmpCol+1];
		result->eightPuzzles[currentEmpRow][currentEmpCol+1] = EMPTY;
	}
}

struct Node{
	State state;
	struct Node* parent;
	int no_function;
};
typedef struct Node* Node;

void initNode(Node* node){
	(*node) = (Node)malloc(sizeof(struct Node));
	makeNullState(&(*node)->state);
	(*node)->parent = NULL;
	(*node)->no_function = 0;
}

typedef struct{
	Node element[MAXLENGTH];
	int length;
} List;

void makeNullList(List *L){
	L->length = 0;
}

void add(List *L, Node node){
	L->element[L->length++] = node;
}

int inCloseList(List L, Node node){
	int i;
	for(i = 0; i < L.length; i++){
		if(compareState(L.element[i]->state, node->state) == 1)
			return 1;
	}
	return 0;
}

typedef struct{
	Node data[MAXLENGTH];
	int size;
} Stack;

void makeNullStack(Stack *S){
	S->size = 0;
}

void push(Stack *S, Node node){
	S->data[S->size++] = node;
}

Node pop(Stack *S){
	return S->data[--S->size];
}

int emptyStack(Stack S){
	return S.size == 0;
}

void performFunctions(Node node, Node result, int noFunction){
	switch(noFunction){
		case 1:{
			upOperator(node->state, &result->state);	
			break;
		}
		case 2:{
			downOperator(node->state, &result->state);
			break;
		}
		case 3:{
			leftOperator(node->state, &result->state);
			break;
		}
		case 4:{
			rightOperator(node->state, &result->state);
			break;
		}
		default:{
			printf("No operator!");
			break;
		}
	}
}

typedef struct{
	Node items[MAXLENGTH];
	int front, rear;
} Queue;

void makeNullQueue(Queue *Q){
	Q->front = 0;
	Q->rear = 0;
}

void enQueue(Queue *Q, Node node){
	Q->items[Q->rear++] = node;
}

Node deQueue(Queue *Q){
	return Q->items[Q->front++];
}

int emptyQueue(Queue Q){
	return Q.front > Q.rear;
}

Node bfs(Node startNode){
	int act;
	Queue open;
	makeNullQueue(&open);
	List close;
	makeNullList(&close);
	
	enQueue(&open, startNode);	
	while(!emptyQueue(open)){
		Node node = deQueue(&open);
		add(&close, node);
		for(act = 1; act <= 4; act++){
			Node childNode;
			initNode(&childNode);
			performFunctions(node, childNode, act);
			childNode->parent = node;
			childNode->no_function = act;
			if(!inCloseList(close, childNode)){
				if(checkGoal(childNode->state))
					return childNode;
				enQueue(&open, childNode);
			}
		}
	}
}

Stack tracingPath(Node node){
	Stack S;
	makeNullStack(&S);
	while(node->parent != NULL){
		push(&S, node);
		node = node->parent;
	}
	return S;
}

void printPathFrom(Stack S){
	int step = 1;
	while(!emptyStack(S)){
		Node node = pop(&S);
		printf("Step %d: ", step++);
		switch(node->no_function){
			case 1:{
				printf("UP\n");
				break;
			}
			case 2:{
				printf("DOWN\n");
				break;
			}
			case 3:{
				printf("LEFT\n");
				break;
			}
			case 4:{
				printf("RIGHT\n");
				break;
			}
		}
		printState(node->state);
	}
}

int main(){
	int i, j;
	Node startNode;
	initNode(&startNode);
	
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			startNode->state.eightPuzzles[i][j] = startState.eightPuzzles[i][j];
	startNode->state.emptyRow = startState.emptyRow;
	startNode->state.emptyCol = startState.emptyCol;
	
	printf("Start state:\n");
	printState(startState);
//	printState(startNode->state);
	Node node = bfs(startNode);
//	printState(node->state);
	Stack stack = tracingPath(node);
	printPathFrom(stack);
	return 0;
}
