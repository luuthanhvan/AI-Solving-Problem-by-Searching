package pouring_water;

public class Stack {
	/* define some private attributes */
	private Node[] nodes;
	private int topIdx;
	private int capacity; // save the size of Stack
	
	public Stack(int size) {
		this.nodes = new Node[size];
		this.topIdx = -1;
		this.capacity = size;
	}
	
	public void push(Node newNode) {
		if(this.isFull()) {
			System.out.println("Error! Stack is full.");
			System.exit(1); // terminates the program
		}
		else {
			this.nodes[++this.topIdx] = newNode;
		}
	}
	
	public Node pop() {
		if(this.isEmpty()) {
			System.out.println("Error! Stack is empty.");
			System.exit(1);
		}
		// return this node and remove it
		return this.nodes[this.topIdx--];	
	}
	
	public int getSize() {
		return this.topIdx+1;
	}
	
	public boolean isEmpty() {
		return this.topIdx == -1;
	}
	
	public boolean isFull() {
		return this.topIdx == this.capacity-1;
	}
	
	public boolean findNode(Node node) {
		for(int i = 0; i <= this.topIdx; i++) {
			Node curNode = this.nodes[i];
			if(curNode.state.compareState(node.state))
				return true;
		}
		return false;
	}
	
	public void printStack() {
		for(int i = 0; i <= this.topIdx; i++) {
			Node curNode = this.nodes[i];
			curNode.state.printState();
		}
	}
}
