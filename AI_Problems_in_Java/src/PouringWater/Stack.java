package PouringWater;

public class Stack {
	/* define some private attributes */
	private Node[] nodes;
	private int topIdx;
	
	public Stack() {
		this.nodes = new Node[100];
		this.topIdx = 0;
	}
	
	public boolean isEmpty() {
		return this.topIdx == 0;
	}
	
	public void push(Node newNode) {
		this.nodes[this.topIdx++] = newNode;
	}
	
	public Node pop() {
		return this.nodes[--this.topIdx];
	}
	
	public boolean findNode(Node node) {
		while(!this.isEmpty()) {
			Node curNode = this.pop();
			if(node.state.compareState(curNode.state))
				return true;
		}
		return false;
	}
	
	public void display() {
		while(!this.isEmpty()) {
			Node curNode = this.pop();
			curNode.state.printState();
		}
	}
}
