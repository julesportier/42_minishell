#include "../unity/unity.h"
#include "../../src/parsing/parsing.h"

void	setUp(void) {
}

void	tearDown(void) {
}

void	wrap_test_scan_line_firstTokenShouldPass(
		enum e_token_type type,
		char *lines[],
		char *assert_str)
{
	t_dlst	*tokens_list = NULL;
	t_error	error = success;
	int	i = 0;
	char message[100];
	while (lines[i] != NULL)
	{
		sprintf(message, "  Input%d: '%s'", i, lines[i]);
		tokens_list = scan_line(lines[i], &error);
		TEST_ASSERT_EQUAL_INT(success, error);
		TEST_ASSERT_EQUAL_INT_MESSAGE(type, ((t_word *)(tokens_list->content))->type, message);
		TEST_ASSERT_EQUAL_STRING_MESSAGE(
			assert_str, ((t_word *)(tokens_list->content))->str, message);
		tokens_list = NULL;
		error = success;
		++i;
	}
}

void	wrap_test_scan_line_firstTokenShouldFail(
		char *lines[],
		t_error error)
{
	t_dlst	*tokens_list = NULL;
	int	i = 0;
	char message[100];
	while (lines[i] != NULL)
	{
		sprintf(message, "  Input%d: '%s'", i, lines[i]);
		tokens_list = scan_line(lines[i], &error);
		TEST_ASSERT_EQUAL_INT(recoverable, error);
		free(tokens_list);
		tokens_list = NULL;
		error = success;
		++i;
	}
}

void	test_scan_line_firstStringLiteralShouldPass(void)
{
	char *line[] = {
		"test",
		"test  ",
		"  test",
		"   test  ",
		"test hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(literal, line, "test");
}

void	test_scan_line_firstSingleQuotesLiteralShouldPass(void)
{
	char *line[] = {
		"'test'",
		"'test'  ",
		"  'test'",
		"   'test'  ",
		"'test' hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(literal, line, "test");
}

void	test_scan_line_firstSingleQuotesLiteralShouldFail(void)
{
	char *line[] = {
		"'test",
		"'test  ",
		"  'test",
		"   'test  ",
		"'test hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldFail(line, recoverable);
}

void	test_scan_line_firstDollarLiteralShouldPass(void)
{
	char *line[] = {
		"$",
		"$$",
		"$  ",
		"  $",
		"   $  ",
		"$ hello",
		"$ $",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(literal, line, "$");
}

void	test_scan_line_firstDoubleQuotesShouldPass(void)
{
	char *line[] = {
		"\"test\"",
		"\"test\"  ",
		"  \"test\"",
		"   \"test\"  ",
		"\"test\" \"hello\"",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(double_quotes, line, "test");
}

void	test_scan_line_firstDoubleQuotesShouldFail(void)
{
	char *line[] = {
		"\"test",
		"\"test  ",
		"  \"test",
		"   \"test  ",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldFail(line, recoverable);
}

void	test_scan_line_firstDoubleQuotesSpecialCharsShouldPass(void)
{
	char *line[] = {
		"\"$test'&&|;||()><>><<*\"",
		"\"$test'&&|;||()><>><<*\"  ",
		"  \"$test'&&|;||()><>><<*\"",
		"   \"$test'&&|;||()><>><<*\"  ",
		"\"$test'&&|;||()><>><<*\" \"hello\"",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(double_quotes, line, "$test'&&|;||()><>><<*");
}

void	test_scan_line_firstVariableShouldPass(void)
{
	char *line[] = {
		"$test",
		"$test  ",
		"  $test",
		"   $test  ",
		"$test hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(variable, line, "test");
}

void	test_scan_line_firstWildcardShouldPass(void)
{
	char *line[] = {
		"*",
		"*  ",
		"  *",
		"   *  ",
		"* hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(wildcard, line, NULL);
}

void	test_scan_line_firstOrShouldPass(void)
{
	char *line[] = {
		"||",
		"||  ",
		"  ||",
		"   ||  ",
		"|| hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(or, line, NULL);
}

void	test_scan_line_firstAndShouldPass(void)
{
	char *line[] = {
		"&&",
		"&&  ",
		"  &&",
		"   &&  ",
		"&& hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(and, line, NULL);
}

void	test_scan_line_firstPipelineShouldPass(void)
{
	char *line[] = {
		"|",
		"|  ",
		"  |",
		"   |  ",
		"| hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(pipeline, line, NULL);
}

void	test_scan_line_firstLeftparenthesisShouldPass(void)
{
	char *line[] = {
		"(",
		"(  ",
		"  (",
		"   (  ",
		"( hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(left_parenthesis, line, NULL);
}

void	test_scan_line_firstRightparenthesisShouldPass(void)
{
	char *line[] = {
		")",
		")  ",
		"  )",
		"   )  ",
		") hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(right_parenthesis, line, NULL);
}

void	test_scan_line_firstRediroutputShouldPass(void)
{
	char *line[] = {
		">",
		">  ",
		"  >",
		"   >  ",
		"> hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(redir_output, line, NULL);
}

void	test_scan_line_firstAppendoutputShouldPass(void)
{
	char *line[] = {
		">>",
		">>  ",
		"  >>",
		"   >>  ",
		">> hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(append_output, line, NULL);
}

void	test_scan_line_firstRedirinputShouldPass(void)
{
	char *line[] = {
		"<",
		"<  ",
		"  <",
		"   <  ",
		"< hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(redir_input, line, NULL);
}

void	test_scan_line_firstHeredocShouldPass(void)
{
	char *line[] = {
		"<<",
		"<<  ",
		"  <<",
		"   <<  ",
		"<< hello",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(heredoc, line, NULL);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_scan_line_firstStringLiteralShouldPass);
	RUN_TEST(test_scan_line_firstSingleQuotesLiteralShouldPass);
	RUN_TEST(test_scan_line_firstSingleQuotesLiteralShouldFail);
	RUN_TEST(test_scan_line_firstDollarLiteralShouldPass);
	RUN_TEST(test_scan_line_firstDoubleQuotesShouldPass);
	RUN_TEST(test_scan_line_firstDoubleQuotesShouldFail);
	RUN_TEST(test_scan_line_firstDoubleQuotesSpecialCharsShouldPass);
	RUN_TEST(test_scan_line_firstVariableShouldPass);
	RUN_TEST(test_scan_line_firstWildcardShouldPass);
	RUN_TEST(test_scan_line_firstOrShouldPass);
	RUN_TEST(test_scan_line_firstAndShouldPass);
	RUN_TEST(test_scan_line_firstPipelineShouldPass);
	RUN_TEST(test_scan_line_firstLeftparenthesisShouldPass);
	RUN_TEST(test_scan_line_firstRightparenthesisShouldPass);
	RUN_TEST(test_scan_line_firstRediroutputShouldPass);
	RUN_TEST(test_scan_line_firstAppendoutputShouldPass);
	RUN_TEST(test_scan_line_firstRedirinputShouldPass);
	RUN_TEST(test_scan_line_firstHeredocShouldPass);
	return UNITY_END();
}
