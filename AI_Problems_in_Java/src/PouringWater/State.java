package PouringWater;

public class State {
	/* some private attributes */
	private int x;
	private int y;
	private int TANK_CAPACITY_X;
	private int TANK_CAPACITY_Y;
	private int GOAL;
	// a constant
	final int EMPTY = 0;
	
	/* public methods */
	// default constructor
	public State() {
		this.x = 0;
		this.y = 0;
	}
	
	// constructor with 5 parameters
	public State(int x, int y, int tankCapacityX, int tankCapacityY, int goal) {
		this.x = x;
		this.y = y;
		this.TANK_CAPACITY_X = tankCapacityX;
		this.TANK_CAPACITY_Y = tankCapacityY;
		this.GOAL = goal;
	}
	
	// copy constructor
	public State(State state) {
		this.x = state.x;
		this.y = state.y;
		this.TANK_CAPACITY_X = state.TANK_CAPACITY_X;
		this.TANK_CAPACITY_Y = state.TANK_CAPACITY_Y;
		this.GOAL = state.GOAL;
	}
	
	public int getX() {
		return this.x;
	}
	
	public int getY() {
		return this.y;
	}
	
	// display a state
	public void printState() {
		System.out.println(this.x + ", " + this.y);
	}
	
	// check a state is goal state
	public boolean checkGoal() {
//		System.out.println("Goal: " + this.GOAL);
		return (this.x == GOAL || this.y == GOAL);
	}
	
	// compare 2 states are equal or not
	public boolean compareState(State state) {
		return (this.x == state.x && this.y == state.y);
	}
	
	/* Define 6 actions: 
		- pouring water full X
		- pouring water full Y
		- pouring water empty X
		- pouring water empty Y
		- pouring water X to Y
		- pouring water Y to X
	*/
	
	// pouring water full X jar
	public boolean pouringWaterFullX(State newState){
		if(this.x < this.TANK_CAPACITY_X){
			newState.x = this.TANK_CAPACITY_X;
			newState.y = this.y;
			return true;
		}
		return false;
	}
	
	// pouring water full Y jar
	public boolean pouringWaterFullY(State newState){
		if(this.y < this.TANK_CAPACITY_Y){
			newState.x = this.x;
			newState.y = this.TANK_CAPACITY_Y;
			return true;
		}
		return false;
	}
	
	// pouring water empty X jar
	public boolean pouringWaterEmptyX(State newState){
		if(this.x > this.EMPTY){
			newState.x = this.EMPTY;
			newState.y = this.y;
			return true;
		}
		return false;
	}
	
	// pouring water empty Y jar
	public boolean pouringWaterEmptyY(State newState){
		if(this.y > this.EMPTY){
			newState.x = this.x;
			newState.y = this.EMPTY;
			return true;
		}
		return false;
	}
	
	// pouring water X to Y
	public boolean pouringWaterXToY(State newState) {
		if(this.x > this.EMPTY && this.y < this.TANK_CAPACITY_Y) {
			newState.x = Math.max(0, this.x-(this.TANK_CAPACITY_Y-this.y));
			newState.y = Math.min(this.TANK_CAPACITY_Y, this.x+this.y);
			return true;
		}
		return false;
	}
	
	// pouring water Y to X
	public boolean pouringWaterYToX(State newState) {
		if(this.y > this.EMPTY && this.x < this.TANK_CAPACITY_X) {
			newState.x = Math.min(this.TANK_CAPACITY_X, this.x+this.y);
			newState.y = Math.max(0, this.y-(this.TANK_CAPACITY_X-this.x));
			return true;
		}
		return false;
	}
	
	public boolean callOperator(State newState, int opt) {
		switch(opt) {
			case 1: return this.pouringWaterFullX(newState);
			case 2: return this.pouringWaterFullY(newState);
			case 3: return this.pouringWaterEmptyX(newState);
			case 4: return this.pouringWaterEmptyY(newState);
			case 5: return this.pouringWaterXToY(newState);
			case 6: return this.pouringWaterYToX(newState);
			default:
				System.out.println("Cannot call operator!");
				return false;
		}
	}
	
//	public boolean findState(Stack<Node> stack) {
//		while(!stack.empty()) {
//			Node node = stack.pop();
//			if(this.compareState(node.state))
//				return true;
//		}
//		return false;
//	}
	
	/*
	// pouring water full X jar
	public State pouringWaterFullX(){
		State newState = new State(this);
		if(this.x < this.TANK_CAPACITY_X){
			newState.x = this.TANK_CAPACITY_X;
			return newState;
		}
		return null;
	}	
	
	// pouring water full Y jar
	public State pouringWaterFullY(){
		State newState = new State(this);
		if(this.y < this.TANK_CAPACITY_Y){
			newState.y = this.TANK_CAPACITY_Y;
			return newState;
		}
		return null;
	}
	
	// pouring water empty X jar
	public State pouringWaterEmptyX(){
		State newState = new State(this);
		if(this.x > this.EMPTY) {
			newState.x = this.EMPTY;
			return newState;
		}
		return null;
	}
	
	// pouring water empty Y jar
	public State pouringWaterEmptyY(){
		State newState = new State(this);
		if(this.y > this.EMPTY) {
			newState.y = this.EMPTY;
			return newState;
		}
		return null;
	}
	
	// pouring water X to Y
	public State pouringWaterXToY(){
		State newState = new State(this);
		if(this.x > this.EMPTY && this.y < this.TANK_CAPACITY_Y) {
			newState.x = Math.max(0, this.x-(this.TANK_CAPACITY_Y-this.y));
			newState.y = Math.min(this.TANK_CAPACITY_Y, this.x+this.y);
			return newState;
		}
		return null;
	}
	
	public State pouringWaterYToX(){
		State newState = new State(this);
		if(this.y > this.EMPTY && this.x < this.TANK_CAPACITY_X) {
			newState.x = Math.min(this.TANK_CAPACITY_X, this.x+this.y);
			newState.y = Math.max(0, this.y-(this.TANK_CAPACITY_X-this.x));
			return newState;
		}
		return null;
	}
	
	public State callOperator(int opt) {
		switch(opt) {
			case 1:
				return this.pouringWaterFullX();
			case 2:
				return this.pouringWaterFullY();
			case 3:
				return this.pouringWaterEmptyX();
			case 4:
				return this.pouringWaterEmptyY();
			case 5:
				return this.pouringWaterXToY();
			case 6:
				return this.pouringWaterYToX();
			default:
				System.out.println("Error! Cannot call operator.");
				return null;
		}
	} */
}	
