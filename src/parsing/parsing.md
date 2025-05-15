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

# context free grammar (for parsing)

expression -> binary

binary -> pipeline* (("||" | "&&") expression)* ;

pipeline -> redirection ("|" redirection)* ;

redirection -> primary? ((">" | ">>" | "<" | "<<") primary* ;

primary -> (literal | double_quotes | variable | wildcards | subshell)* ;





expression -> literal
            | expanding
            | binary
            | grouping
            | quotes

binary -> expression ("||" | "&&") expression ;
        | pipeline

pipeline -> expression "|" expression ;
        | redirection

redirection -> expression? (">" | ">>" | "<" | "<<") (literal | variable) ;
        | grouping

grouping -> "(" expression ")" ;
        | (expanding | quotes | literal)

expanding -> variable | "*" ;
quotes -> ("'" | '"') expression ("'" | '"') ;
literal ;
