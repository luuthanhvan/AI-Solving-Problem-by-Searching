package pouring_water;

public class Node {
	/* some private attributes */
	State state;
	Node parent;
	int noFunction;
	
	public Node(State state, Node parent, int noFunction) {
		this.state = state;
		this.parent = parent;
		this.noFunction = noFunction;
	}
}
