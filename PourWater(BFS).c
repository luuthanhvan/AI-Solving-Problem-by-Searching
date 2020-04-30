#include <stdio.h>
#include <malloc.h>
#define TANKCAPACITY1 9
#define TANKCAPACITY2 4
#define GOAL 6
#define EMPTY 0
#define MAXLENGTH 50

const char* action[] = {"First State", "pour Water Full X", "pour Water Full Y", "pour Water Empty X",
"pour Water Empty Y", "pour Water X to Y", "pour Water Y to X"};


typedef struct{
	int x, y;
} State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
}

int checkGoal(State S){
	return ((S.x == GOAL) || (S.y) == GOAL);
}

void printState(State state){
	printf("(%d, %d)\n", state.x, state.y);
}

void pourWaterFullX(State state, State *result){
	if(state.x < TANKCAPACITY1){
		result->x = TANKCAPACITY1;
		result->y = state.y;
	}
}

void pourWaterFullY(State state, State *result){
	if(state.y < TANKCAPACITY2){
		result->x = state.x;
		result->y = TANKCAPACITY2;
	}
}

void pourWaterEmptyX(State state, State* result){
	if(state.x > EMPTY){
		result->x = EMPTY;
		result->y = state.y;
	}
}

void pourWaterEmptyY(State state, State* result){
	if(state.y > EMPTY){
		result->x = state.x;
		result->y = EMPTY;
	}
}

int min(int num1, int num2){
	return num1 < num2 ? num1:num2;
}

int max(int num1, int num2){
	return num1 > num2 ? num1:num2;
}

void pourWaterXY(State state, State* result){
	if(state.x > 0 && state.y < TANKCAPACITY2){
		result->x = max(0, state.x-(TANKCAPACITY2-state.y));
		result->y = min(TANKCAPACITY2, state.x+state.y);
	}
}

void pourWaterYX(State state, State* result){
	if(state.y > 0 && state.x < TANKCAPACITY1){
		result->x = min(TANKCAPACITY1, state.x+state.y);
		result->y = max(0, state.y-(TANKCAPACITY1-state.x));
	}
}

struct Node{
	State state;
	struct Node* parent;
	int no_function;
};
typedef struct Node* Node;

void initNode(Node *node){
	(*node) = (struct Node*)malloc(sizeof(struct Node));
	makeNullState(&(*node)->state);
	(*node)->parent = NULL;
	(*node)->no_function = 0;
}

typedef struct{
	Node element[MAXLENGTH];
	int size;
} List;

/* Initialization an empty list */
void makeNullList(List *L){
	L->size = 0;
}

/* Adding new node to the end of the list */
void add(List *L, Node newNode){
	L->element[L->size++] = newNode;
}

/* Checking a state is in close list or not*/
int closeList(List L, Node node){
	int i;
	for(i = 0; i < L.size; i++){
		if((L.element[i]->state.x == node->state.x) && (L.element[i]->state.y == node->state.y))
			return 1;
	}
	return 0;
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

typedef struct{
	Node data[MAXLENGTH];
	int top;
} Stack;

void makeNullStack(Stack *S){
	S->top = 0;
}

/* Adding new node to the end of the Stack */
void push(Stack *S, Node newNode){
	S->data[S->top++] = newNode;
}

/* Removing the node at the end of the Stack and returns this node */
Node pop(Stack *S){
	return S->data[--S->top];
}

int emptyStack(Stack S){
	return S.top == 0;
}

void performFunctions(Node node, Node newNode, int noFunction){
	switch(noFunction){
		case 1: {
			pourWaterFullX(node->state, &newNode->state);	
			break;
		}
		case 2:{
			pourWaterFullY(node->state, &newNode->state);
			break;
		}
		case 3:{
			pourWaterEmptyX(node->state, &newNode->state);
			break;
		}
		case 4:{
			pourWaterEmptyY(node->state, &newNode->state);
			break;
		}
		case 5:{
			pourWaterXY(node->state, &newNode->state);
			break;
		}
		case 6:{
			pourWaterYX(node->state, &newNode->state);
			break;
		}
		default:{
			printf("No operator!");
			break;
		}
	}
}

Node bfs(Node startNode){
	int act; // action	
	Queue open;
	makeNullQueue(&open);
	
	List close;
	makeNullList(&close);

	enQueue(&open, startNode);
	
//	printState(startNode->state);
	
	while(!emptyQueue(open)){
		Node node = deQueue(&open);

		add(&close, node);
		
		for(act = 1; act <= 6; act++){
			Node newNode;
			initNode(&newNode);
			performFunctions(node, newNode, act);
			newNode->parent = node;
			newNode->no_function = act;
			
			if(!closeList(close, newNode)){
				if(checkGoal(newNode->state))
					return newNode;
				enQueue(&open, newNode);
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
	push(&S, node); // start node
	return S;
}

/* Printing path from leaf to root in the stack S */
void printPathFrom(Stack S){
	int step = 1;
	while(!emptyStack(S)){
		Node node = pop(&S);
		printf("Step %d: %s\n", step, action[node->no_function]);
		printState(node->state);
		step++;
		printf("===============\n");
	}
}

int main(){
	Node startNode;
	initNode(&startNode);
	startNode->state.x = 0;
	startNode->state.y = 0;
	
	Node node = bfs(startNode);
//	printState(node->state);
	Stack stack = tracingPath(node);
	printPathFrom(stack);
	return 0;
}
