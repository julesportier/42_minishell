# Lexing

- [wikipedia: *Lexical Analysis*](https://en.wikipedia.org/wiki/Lexical_analysis)
Extract tokens from the input string.
Some tokens names:
- identifier
- keyword (if, 
- they are merged in the parsing for minishell
    - separator (&&, ||, ;)
    - operator (|, &&, ||, ()...)
- literal (1, true, "text"...)
- comment
- whitespace

``` c
// For parsing -> tokenization & expansion
# define OP = 0x1;
# define SEP = 0x2;
# define CMD = 0x4;
# define S_QUOTES = 0x8;
# define D_QUOTES = 0xF;

typedef enum e_token_type
{
    litteral,
    variable,
    wildcard,
    or,
    and,
    pipe,
    left_parenthesis,
    right_parenthesis,
    redir_input,
    redir_output,
    append_output,
    heredoc
}   t_token_type;

typedef struct  s_word
{
    // 0 or 1, to know if the word is separated by spaces
    // (eg `echo $HOME"text"1` is four tokens but expands to two words `echo /home/juportietext1`)
    int cat_prev;
    t_token_type    type; // a macro. pour le lexing pour savoir ou splitter et éviter de redéfinir le rôle des tokens.
    char    *word;
    t_word  *next;
} t_word;

// After expansions set redirections in t_block
// transform t_word into string array (char **) and put it into words member

typedef enum e_operator
{
    nil,
    pipe,
    and,
    or
}   t_operator;

// Structures to give to exec.
typedef struct  s_bin_tree
{
    t_operator  operator;
    t_bin_tree *parent_node;
    t_bin_tree *node_left;
    t_bin_tree *node_right;
    t_node_content *content
} t_bin_tree;

typedef struct  s_node_content
{
    //int is_subshell; // start from zero (no subshell) to N nested subshells
    // 01b heredoc, 00b standard file
    // 00b no expansions on input (if delimiter inside quotes or double quotes), 10b expands. Delimiter is never expanded.
    int heredoc_flags;
    char    *input; // NULL if no redirection
    char    *output; // NULL if no redirection
    t_word  *words_lst; // Command followed by args (or operator)
} t_node_content;
```
OLD VERSION
``` c
// For parsing -> tokenization & expansion
# define OP = 0x1;
# define SEP = 0x2;
# define CMD = 0x4;
# define S_QUOTES = 0x8;
# define D_QUOTES = 0xF;
// For binary tree node's operator number
# define PIPE = 0x1;
# define OR = 0x2;
# define AND = 0x4;

typedef struct  s_word
{
    int type; // a macro. pour le parsing pour savoir ou splitter et éviter de redéfinir le rôle des tokens.
    char    *word;
    t_word  *next;
} t_word;

// After expansions set redirections in t_block
// transform t_word into string array (char **) and put it into words member

// Structures to give to exec.
typedef struct  s_bin_tree
{
    int    operator; // a macro
    t_node *parent_node;
    t_node *node_left;
    t_node *node_right;
} t_bin_tree;

typedef struct  s_node
{
    //int is_subshell; // start from zero (no subshell) to N nested subshells
    char    *input; // NULL if no redirection
    char    *output; // NULL if no redirection
    t_word  *words_lst; // Command followed by args (or operator)
    t_node  *prev; //?? REMOVE THIS TWO POINTERS AND JUST STORE THE LIST IN WORD TYPE. NEED TO PERFORM REDIRECTIONS WHEN CREATING TREE ???
    t_node  *next;
} t_node;
```
