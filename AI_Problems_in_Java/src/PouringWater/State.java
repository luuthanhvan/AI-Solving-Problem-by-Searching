package PouringWater;

public class State {
	/* define some private attributes */
	private int x;
	private int y;
	private int TANK_CAPACITY_X = 9;
	private int TANK_CAPACITY_Y = 4;
	private int GOAL = 6;
	private int EMPTY = 0;
	
	/* define public methods */
	// default constructor
	public State() {
		this.x = 0;
		this.y = 0;
	}
	
	// constructor with 2 parameters
	public State(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	// copy constructor
	public State(State state) {
		this.x = state.x;
		this.y = state.y;
	}
	
	public int getX() {
		return this.x;
	}
	
	public int getY() {
		return this.y;
	}
	
	// display a state
	public void printState() {
		System.out.println("(" + this.x + ", " + this.y + ")");
	}
	
	// check a state is goal state
	public boolean checkGoal() {
		return (this.x == this.GOAL || this.y == this.GOAL);
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
}	
