#include "../../unity/src/unity.h"
#include "../../src/parsing/lexer.h"

void	setUp(void) {
}

void	tearDown(void) {
}

//void	test_scan_line_firstStringLiteralShouldPass(void)
//{
//	char *lines[] = {
//		"test",
//		"test  ",
//		"  test",
//		"   test  ",
//		"test hello",
//		NULL
//	};
//	char *assert_str = "test";
//	t_dlst	*tokens_list = NULL;
//	t_error	error = success;
//	int	i = 0;
//	while (lines[i] != NULL)
//	{
//		tokens_list = scan_line(lines[i], &error);
//		TEST_ASSERT_EQUAL_INT(success, error);
//		TEST_ASSERT_EQUAL_INT(literal, ((t_word *)(tokens_list->content))->type);
//		TEST_ASSERT_EQUAL_STRING_MESSAGE(
//			assert_str, ((t_word *)(tokens_list->content))->str, ft_itoa(i));
//		tokens_list = NULL;
//		error = success;
//		++i;
//	}
//}
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
		"\"test\"",
		"\"test\"  ",
		"  \"test\"",
		"   \"test\"  ",
		"\"test\" \"hello\"",
		NULL
	};
	wrap_test_scan_line_firstTokenShouldPass(double_quotes, line, "test");
}

int	main(void)
{
	//printf("%s\n", extract_token("test", (int *){0}, false, (t_error *){0})->str);
	UNITY_BEGIN();
	RUN_TEST(test_scan_line_firstDollarLiteralShouldPass);
	RUN_TEST(test_scan_line_firstStringLiteralShouldPass);
	RUN_TEST(test_scan_line_firstDoubleQuotesShouldPass);
	RUN_TEST(test_scan_line_firstDoubleQuotesSpecialCharsShouldPass);
	return UNITY_END();
}
