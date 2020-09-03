package PouringWater;

import java.util.Stack;

public class Main {
	static int MAX_OPERATOR = 6;
	static final String[] actions = {"First state", "pour Water Full X", "pour Water Full Y", 
			"pour Water Empty X", "pour Water Empty Y", "pour Water X to Y", "pour Water Y to X"};
	
	public static boolean findNode(Node node, Stack<Node> stack) {
		while(!stack.empty()) {
			Node curNode = stack.pop();
			if((node.state.getX() == curNode.state.getX()) 
					&& (node.state.getY() == curNode.state.getY()))
				return true;
		}
		return false;
	}
	
	public static Node dfs(State state) {
		Stack<Node> openDFS = new Stack<>();
		Stack<Node> closeDFS = new Stack<>();
		
		Node root = new Node(state, null, 0);
		openDFS.push(root);
		
		while(!openDFS.empty()) {
			Node node = openDFS.pop();
			
			closeDFS.push(node);
			
			if(node.state.getX() == 6 || node.state.getY() == 6) {
				System.out.println("something wrong here");
				return node;	
			}
			for(int opt = 1; opt <= 6; opt++) {
				State newState = new State();
				boolean isValidState = node.state.callOperator(newState, opt);
				
				if(isValidState) {
					Node newNode = new Node(newState, node, opt);
					
					if(findNode(newNode, openDFS) || findNode(newNode, closeDFS))
						continue;
					
					openDFS.push(newNode);
				}
			}
		}
		
		return null;
	}
	
	public static void main(String[] args) {
		/* Test 
		State state = new State(5, 4, 9, 4, 6);
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
		} */ 
		
//		State state1 = new State(0, 0, 9, 4, 6);
//		State state2 = new State(9, 0, 9, 4, 6);
//		State state3 = new State(0, 4, 9, 4, 6);
//		Node node1 = new Node(state1, null, 0);
//		Node node2 = new Node(state2, node1, 1);
//		Node node3 = new Node(state3, node2, 2);
//		
//		Stack stack = new Stack();
//		stack.push(node1);
//		stack.push(node2);
//		stack.push(node3);
//		
//		while(!stack.isEmpty()) {
//			Node node = stack.pop();
//			if((node.state.getX() == node2.state.getX()) && (node.state.getY() == node2.state.getY()))
//				System.out.println("node and node1 are equal");
//		}
//		System.out.println(stack.findNode(node2));
		
		State state = new State(0, 0, 9, 4, 6);		
		Node node = dfs(state);
		if(node != null)
			node.state.printState();
		else System.out.println("No success");
		
//		State newState = new State();
//		boolean isValidOperator = state.callOperator(newState, 1);
//		
//		if(isValidOperator) {
//			System.out.println("Success");
//			newState.printState();
//		}
		
		/* Test 1 for actions which no return (using void) */
//		State state1 = new State(state);
//		state.pouringWaterFullX(state1);
//		
//		state1.printState();
//		
//		State state2 = new State(state1);
//		state1.pouringWaterFullY(state2);
//		
//		state2.printState();
		
		
		/* Test 2 for actions which return a state */
//		State state1 = state.pouringWaterFullX();
//		state1.printState();
//		
//		State state2 = state1.pouringWaterXToY();
//		state2.printState();
//		
//		State state3 = state2.pouringWaterEmptyX();
//		state3.printState();
//		
//		State state4 = state3.pouringWaterYToX();
//		state4.printState();
//		
//		State state5 = state4.pouringWaterFullY();
//		state5.printState();
//		
//		State state6 = state5.pouringWaterEmptyY();
//		state6.printState();	
	}
}
