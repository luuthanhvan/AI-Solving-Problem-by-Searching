package pouring_water;

public class Queue {
	/* define some private attributes */
	private Node[] nodes;
	private int front;
	private int rear;
	private int capacity; // save the size of Queue
	
	public Queue(int size) {
		nodes = new Node[size];
		this.front = 0;
		this.rear = -1;
		this.capacity = size;
	}
		
	// insert a new node to the queue
	public void enQueue(Node newNode) {
		if(this.isFull()) {
			System.out.println("Error! Queue is full.");
			System.exit(1); // terminate the program
		}
		else {
			this.nodes[++this.rear] = newNode;
		}
	}
	
	// get the node at the top of Queue and remove it
	public Node deQueue() {
		if(this.isEmpty()) {
			System.out.println("Error! Queue is empty.");
			return null;
		}
		return this.nodes[this.front++];
	}
	
	public boolean isFull() {
		return this.rear == capacity-1;
	}
	
	public boolean isEmpty() {
		return this.rear == -1;
	}
	
	public int getSize() {
		return (this.rear-this.front)+1;
	}
	
	public boolean findNode(Node node) {
		for(int i = 0; i < this.getSize(); i++) {
			Node curNode = this.nodes[i];
			if(curNode.state.compareState(node.state))
				return true;
		}
		return false;
	}
	
	public void printQueue() {
		for(int i = 0; i < this.getSize(); i++) {
			Node curNode = this.nodes[i];
			curNode.state.printState();
		}
	}
}
