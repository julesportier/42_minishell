
#include "minishell.h"

void init_node(t_node *node, char *cmd, t_node *left, t_node *right, int op)
{
	char	**cmd_words;

	if (cmd != NULL)
	{
		cmd_words = ft_split(cmd, ' ');
		node->cmd = cmd_words;
	}
	else
		node->cmd = NULL;
	node->op = op;
	node->left = left;
	node->right = right;
	if(left)
		left->prev = node;
	if(right)
		right->prev = node;
}

void print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("%s ", array[i]);
		i++;
	}
}

void	print_tree_node(t_node *node)
{
	printf("NODE -> CMD = %p| LEFT = %p | RIGHT = %p | PREV = %p | OP = %d \n", node->cmd, node->left, node->right, node->prev, node->op);
}

t_node *hardcoded_tree(void)
{
	t_node	*nodeA;
	t_node	*nodeA1;
	t_node	*nodeA2;
	t_node	*nodeB;
	t_node	*nodeB1;
	t_node	*nodeB2;
	t_node	*nodeC;
	t_node	*nodeC1;
	t_node	*nodeC2;
	t_node	*nodeD;
	t_node	*nodeD1;
	t_node	*nodeD2;
	t_node	*nodeE;
	t_node	*nodeE1;
	t_node	*nodeE2;

	nodeA = malloc(sizeof (t_node));
	nodeA1 = malloc(sizeof (t_node));
	nodeA2 = malloc(sizeof (t_node));
	nodeB = malloc(sizeof (t_node));
	nodeB1 = malloc(sizeof (t_node));
	nodeB2 = malloc(sizeof (t_node));
	nodeC = malloc(sizeof (t_node));
	nodeC1 = malloc(sizeof (t_node));
	nodeC2 = malloc(sizeof (t_node));
	nodeD = malloc(sizeof (t_node));
	nodeD1 = malloc(sizeof (t_node));
	nodeD2 = malloc(sizeof (t_node));
	nodeE = malloc(sizeof (t_node));
	nodeE1 = malloc(sizeof (t_node));
	nodeE2 = malloc(sizeof (t_node));
	
	nodeA->prev = NULL;

	init_node(nodeA, NULL, nodeA1, nodeA2, AND);

	init_node(nodeA1, NULL, nodeB, nodeC, OR);
	init_node(nodeB, "/bin/echo B", NULL, NULL, 0);
	// init_node(nodeB1, "/bin/echo B1", NULL, NULL, 0);
	// init_node(nodeB2, "/bin/cat", NULL, NULL, 0);
	init_node(nodeC, "/bin/echo C", NULL, NULL, 0);
	// init_node(nodeC1, "/bin/echo C1", NULL, NULL, 0);
	// init_node(nodeC2, "/bin/echo C2", NULL, NULL, 0);

	init_node(nodeA2, "/bin/echo A2", NULL, NULL, 0);
	// init_node(nodeD, NULL, nodeD1, nodeD2, AND);
	// init_node(nodeD1, "/bin/echo D1", NULL, NULL, 0);
	// init_node(nodeD2, "/bin/echo D2", NULL, NULL, 0);
	// init_node(nodeE, NULL, nodeE1, nodeE2, AND);
	// init_node(nodeE1, "/bin/echo E1", NULL, NULL, 0);
	// init_node(nodeE2, "/bin/echo E2", NULL, NULL, 0);

	// print_tree_node(nodeA);
	// print_tree_node(nodeA1);
	// print_tree_node(nodeA2);
	// print_tree_node(nodeB);
	// print_tree_node(nodeB1);
	// print_tree_node(nodeC);
	// print_tree_node(nodeC1);
	// print_tree_node(nodeC2);
	// print_tree_node(nodeD);
	// print_tree_node(nodeD1);
	// print_tree_node(nodeD2);
	// print_tree_node(nodeE);
	// print_tree_node(nodeE1);
	// print_tree_node(nodeE2);

	return (nodeA);
}

int	main(int ac, char **av, char **env)
{
	t_node	*tree;
	t_var_data vars;

	vars.env = env;

	tree = hardcoded_tree();
	return (recursive_exec(tree, &vars));

}