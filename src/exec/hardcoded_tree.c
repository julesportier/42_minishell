
// #include "../../libft/src/libft.h"
// #include <unistd.h>
// #include <stdio.h>
// #include <sys/wait.h>
// #include "../general_utils/utils.h"
// #include "../error_handling/errors.h"
// #include "../cleaning_utils/cleaning.h"
// #include "exec.h"
// #include "../minishell.h"

// void init_node(t_node *node, char *cmd, t_node *left, t_node *right, int op, char *name)
// {
// 	char	**cmd_words;

// 	if (cmd != NULL)
// 	{
// 		cmd_words = ft_split(cmd, ' ');
// 		node->cmd = cmd_words;
// 	}
// 	else
// 		node->cmd = NULL;
// 	node->name = name;
// 	node->op = op;
// 	node->left = left;
// 	node->right = right;
// 	if(left)
// 		left->prev = node;
// 	if(right)
// 		right->prev = node;
// }

// void print_array(char **array)
// {
// 	int	i;

// 	i = 0;
// 	while (array[i])
// 	{
// 		printf("%s ", array[i]);
// 		i++;
// 	}
// }

// void	print_tree_node(t_node *node)
// {
// 	printf("NODE %s -> CMD = %p| LEFT = %p | RIGHT = %p | PREV = %p | OP = %d \n", node->name, node->cmd, node->left, node->right, node->prev, node->op);
// }

// t_node *hardcoded_tree(void)
// {
// 	t_node	*nodeA;
// 	t_node	*nodeA1;
// 	t_node	*nodeA2;
// 	t_node	*nodeB;
// 	t_node	*nodeB1;
// 	t_node	*nodeB2;
// 	t_node	*nodeC;
// 	t_node	*nodeC1;
// 	t_node	*nodeC2;
// 	t_node	*nodeD;
// 	t_node	*nodeD1;
// 	t_node	*nodeD2;
// 	t_node	*nodeE;
// 	t_node	*nodeE1;
// 	t_node	*nodeE2;

// 	nodeA = malloc(sizeof (t_node));
// 	nodeA1 = malloc(sizeof (t_node));
// 	nodeA2 = malloc(sizeof (t_node));
// 	nodeB = malloc(sizeof (t_node));
// 	nodeB1 = malloc(sizeof (t_node));
// 	nodeB2 = malloc(sizeof (t_node));
// 	nodeC = malloc(sizeof (t_node));
// 	nodeC1 = malloc(sizeof (t_node));
// 	nodeC2 = malloc(sizeof (t_node));
// 	nodeD = malloc(sizeof (t_node));
// 	nodeD1 = malloc(sizeof (t_node));
// 	nodeD2 = malloc(sizeof (t_node));
// 	nodeE = malloc(sizeof (t_node));
// 	nodeE1 = malloc(sizeof (t_node));
// 	nodeE2 = malloc(sizeof (t_node));

// 	nodeA->prev = NULL;

// 																																																		init_node(nodeA, NULL, nodeA1, nodeA2, PIPE, "A");
// 																																																																																																																										;
// 																							init_node(nodeA1, NULL, nodeB, nodeC, AND, "A1");																																																	/*init_node(nodeA2, NULL, nodeD, nodeE, PIPE, "A2");*/
// 																																																																																																																															;
// 							init_node(nodeB, "/bin/echo 1", NULL, NULL, 0, "B"); 															init_node(nodeC, "/bin/echo sleep", NULL, NULL, 0, "C");																		init_node(nodeA2, "/bin/cat", nodeD1, nodeD2, PIPE, "D");														/* init_node(nodeE, "bin/grep", nodeE1, nodeE2, PIPE, "E");*/
// 																																																													init_node(nodeD1, "/bin/cat", NULL, NULL, 0, "D1"); 	init_node(nodeD2, "/bin/cat", NULL, NULL, 0, "D2");																																																		;
// 	init_node(nodeB1, "/bin/echo 1", NULL, NULL, 0, "B1"); 						init_node(nodeB2, NULL, nodeC1, nodeC2, OR, "B2");		/*	init_node(nodeC1, "/bin/cat", NULL, NULL, 0, "C1"); init_node(nodeC2, "/bin/cat", NULL, NULL, 0, "C2");		*/

// 										init_node(nodeC1, "/bin/grep", NULL, NULL, 0, "E1"); 												init_node(nodeC2, NULL, nodeD, nodeE, OR, "E2");

// 																											init_node(nodeD, "/bin/cat", nodeD1, nodeD2, PIPE, "E1");						 init_node(nodeE, "/bin/echo 5", NULL, NULL, 0, "E2");







// 	// init_node(nodeE2, "/bin/echo E2", NULL, NULL, 0);

// 	// print_tree_node(nodeA);
// 	// print_tree_node(nodeA1);
// 	// print_tree_node(nodeA2);
// 	// print_tree_node(nodeB);
// 	// // print_tree_node(nodeB1);
// 	// print_tree_node(nodeC);
// 	// // print_tree_node(nodeC1);
// 	// // print_tree_node(nodeC2);
// 	// print_tree_node(nodeD);
// 	// // print_tree_node(nodeD1);
// 	// // print_tree_node(nodeD2);
// 	// print_tree_node(nodeE);
// 	// // print_tree_node(nodeE1);
// 	// // print_tree_node(nodeE2);

// 	return (nodeA);
// }

// // int	main(int ac, char **av, char **env)
// // {
// // 	t_node	*tree;
// // 	t_shell_vars vars;

// // 	vars.env = env;

// // 	tree = hardcoded_tree();
// // 	return (exec_command_tree(tree, &vars));

// // }
