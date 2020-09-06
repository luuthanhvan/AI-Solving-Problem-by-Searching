package pouring_water;

public class Main {
	static int MAX_OPERATOR = 6;
	static final String[] actions = {"First state", "pour Water Full X", "pour Water Full Y", 
			"pour Water Empty X", "pour Water Empty Y", "pour Water X to Y", "pour Water Y to X"};
	
	public static Node dfs(State state) {
		Stack openDFS = new Stack(100);
		Stack closeDFS = new Stack(100);
		
		Node root = new Node(state, null, 0);
		
		openDFS.push(root);
		
		while(!openDFS.isEmpty()) {
			Node node = openDFS.pop();
			
			closeDFS.push(node);
			
			if(node.state.checkGoal())
				return node;
			
			for(int opt = 1; opt <= 6; opt++) {
				State newState = new State();
				boolean isValidState = node.state.callOperator(newState, opt);
				
				if(isValidState) {
					Node newNode = new Node(newState, node, opt);
					
					if(openDFS.findNode(newNode) || closeDFS.findNode(newNode))
						continue;
					
					openDFS.push(newNode);
				}
			}
		}
		
		return null;
	}
	
	public static void main(String[] args) {
		State state = new State(0, 0);
		Node node = dfs(state);
		if(node != null) {
			System.out.println("Goal state: ");
			node.state.printState();
		}
		else {
			System.out.println("Cannot reach the goal state!");
		}
		/*
		// Test 
		State state = new State(5, 4);
		System.out.println("Start state: ");
		state.printState();
		
		for(int opt = 1; opt <= 6; opt++) {
			State newState = new State();
			if(state.callOperator(newState, opt)) {
				System.out.println("Hanh dong " + actions[opt] + " thuc hien thanh cong!");
				newState.printState();
			}
			else {
				System.out.println("Hanh dong " + actions[opt] + " thuc hien KHONG thanh cong!");
			}
		}*/
		
//		State state1 = new State(5, 4);
//		State state2 = new State(4, 5);
//		State state3 = new State(9, 0);
//		State state4 = new State(4, 0);
//		
//		Node node1 = new Node(state1, null, 0);
//		Node node2 = new Node(state2, node1, 1);
//		Node node3 = new Node(state3, node2, 2);
//		Node node4 = new Node(state4, node3, 3);
//		
//		Stack stack = new Stack(10);
//		stack.push(node1);
//		stack.push(node2);
//		stack.push(node3);
//		
//		stack.printStack();
//		System.out.println(stack.findNode(node3));
		
//		State newState = new State();
//		boolean isValidOperator = state.callOperator(newState, 1);
//		
//		if(isValidOperator) {
//			System.out.println("Success");
//			newState.printState();
//		}
	}
}
