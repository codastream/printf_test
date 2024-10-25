/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:15:34 by fpetit            #+#    #+#             */
/*   Updated: 2024/10/25 15:07:10 by fpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define WIDTH 80
#define COLWIDTH 12
#define	BASE10 "0123456789"
#define BASEHU "0123456789ABCDEF"
#define BASEHL "0123456789abcdef"

void yellow()
{
	printf("\033[1;33m");
}

void green()
{
	printf("\033[1;32m");
}

void red()
{
	printf("\033[1;31m");
}

void reset()
{
	printf("\033[0m");
}

//// UTILS
void center_text(char *text, int fieldWidth, int len_txt) {
	int padleft;
	int padright;

	if ((fieldWidth - len_txt) % 2 == 0)    
		padleft = padright = (fieldWidth - strlen(text)) / 2;
	else
	{
		padleft = (fieldWidth - strlen(text)) / 2 + 1;
		padright = (fieldWidth - strlen(text)) / 2;
	}
	printf("%*s%s%*s", padleft, "", text, padright, "");
} 

void	print_res(bool is_passed)
{
	if (is_passed)
	{
		green();
		center_text("\U00002705", COLWIDTH, 4);
		reset();
	}
	else
	{
		red();
		center_text("\U00002718", COLWIDTH, 4);
		reset();
	}

}

static bool	check_nb_printed_2(int rf, int rft, bool is_silent)
{
	bool	is_passed;

	if(rf == rft)
	{
		is_passed = true;
		green();
		if (rf < 0)
			is_silent ? 0 : printf("---> both returned %d", rf);
		else	
			is_silent ? 0 : printf("---> both printed %d chars", rf);
	}
	else if (rf == -1)
	{
		is_passed = false;
		red();
		is_silent ? 0 : printf("---> ft_printf should have returned %d  (instead was %d)", rf, rft);
	}
	else
	{
		is_passed = false;
		red();
		is_silent ? 0 : printf("---> ft_printf should have printed %d chars (instead printed %d)", rf, rft);
	}
	reset();
	return (is_passed);
}

int	do_print(int (*f)(const char *, ...), char *template, char *types, char **args, int
		ac, int is_short)
{
	int	res;
	if (is_short)
	{
		FILE *output = freopen("output.txt", "w", stdout);
		if (output == NULL)
		{
			perror("freopen");
			return 1;
		}
	}
	res = -1;

	if (ac == 0)
		res = f(template);
	else if (ac == 1)
	{
		if (!strcmp(types, "c"))
			res = f(template, args[ac - 1][0]);
		else if (!strcmp(types, "s"))
			res = f(template, args[ac - 1]);
		else if (!strcmp(types, "i") || !strcmp(types, "d"))
			res = f(template, ft_atoi(args[ac - 1]));
		else if (!strcmp(types, "x") || !strcmp(types, "X"))
			res = f(template, ft_atol(args[ac - 1]));
		else if (!strcmp(types, "p"))
			res = f(template, ft_atoull(args[ac - 1]));
	}
	else if (ac == 3)
	{
		if (!strcmp(types, "did"))
			res = f(template, ft_atoi(args[ac - 3]), ft_atoi(args[ac - 2]), \
					ft_atoi(args[ac - 1]));
		else if (!strcmp(types, "uuu"))
			res = f(template, ft_atoull(args[ac - 3]), \
					ft_atoull(args[ac - 2]), ft_atoull(args[ac - 1]));
		else if (!strcmp(types, "Xxp"))
			res = f(template, ft_atoull(args[ac - 3]), \
					ft_atoull(args[ac - 2]), ft_atoull(args[ac - 1]));
	}
	else if (ac == 4)
			if (!strcmp(types, "csiu"))
				res = f(template, args[ac - 4][0], args[ac - 3], \
						ft_atol(args[ac - 2]), ft_atoull(args[ac - 1]));
	if (is_short)
	{
		freopen("/dev/tty", "w", stdout);
		
	}
	return (res);
}

bool	test_generic(char *desc, char *template, char *types, char *args[5], int ac, bool
		silent)
{
	int		rf;
	int		rft;
	char	*test_name;
	char	*test;
	int		result;
	int		i;

	test_name = ft_calloc(150, sizeof(char));	
	ft_strlcat(test_name, "\n\n=== TEST ", 150);
	ft_strlcat(test_name, desc, 150);
	silent ? 0 : printf("%s", test_name);
	silent ? 0 : printf("\n");
	test = ft_calloc(150, sizeof(char));
	if (!template)
		ft_strcat(test, "NULL");
	else
		ft_strlcpy(test, template, ft_strlen(template) + 1);
	if (ac > 0)
		ft_strcat(test, " + '");
	i = 0;
	while (i < ac)
	{
		if (!args[i])
			ft_strcat(test, "NULL");
		else
		{
			ft_strcat(test, args[i]);
			ft_strcat(test, "/");
		}
		i++;
	}
	if (ac > 0)
		ft_strcat(test, "'");
	yellow();
	silent ? 0 : printf("%*s\n", WIDTH, test);
	reset();
	// printf
	silent ? 0 : printf("= original\n");
	rf = do_print(printf, template, types, args, ac, silent);
	silent ? 0 : printf("\n");
	// libft
	silent ? 0 : printf("= libft\n");
	rft = do_print(ft_printf, template, types, args, ac, silent);
	silent ? 0 : printf("\n");
	result = check_nb_printed_2(rf, rft, silent);
	silent ? 0 : printf("\n. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
	return (result);
}

// CHARS
bool	test_char_empty(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "";
	is_passed = test_generic("char / empty", "good jo%c", "c", args, 1, is_silent);
	return (is_passed);
}

bool	test_char_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "b";
	is_passed = test_generic("char / nominal", "good jo%c", "c", args, 1, is_silent);
	return (is_passed);
}

bool	test_char_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = NULL;
	is_passed = test_generic("char / NULL", "good jo%c", "c", args, 1, is_silent);
	return (is_passed);
}
// STRINGS
bool	test_str_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "can";
	is_passed = test_generic("str / nominal", "yes we %s!", "s", args, 1, is_silent);
	return (is_passed);
}

bool	test_str_empty(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "";
	is_passed = test_generic("str / empty", "yes we %s!", "s", args, 1, is_silent);
	return (is_passed);
}

bool	test_str_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *s = 0;

	args[0] = s;
	is_passed = test_generic("str / NULL", "yes we %s!", "s", args, 1, is_silent);
	return (is_passed);
}

// INTEGER
bool	test_int_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "1";
	args[1] = "2";
	args[2] = "3";
	is_passed = test_generic("int / nominal", "%d, %i, %d, viva l'Algérie!", "did", args,
			3, is_silent);
	return (is_passed);
}

bool	test_int_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "1";
	args[1] = NULL;
	args[2] = "3";
	is_passed = test_generic("int / NULL", "%d, %i, %d, viva l'Algérie!", "did", args, 3, is_silent);
	return (is_passed);
}

bool	test_int_minout(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "1";
	args[1] = "-2147483649";
	args[2] = "3";
	is_passed = test_generic("int / min - 1", "%d, %i, %d, viva l'Algérie!", "did", args,
			3, is_silent);
	return (is_passed);
}

// UNSIGNED
bool	test_unsint_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "1";
	args[1] = "2";
	args[2] = "3";
	is_passed = test_generic("int / nominal", "%u, %u, %u, viva l'Algérie!", "uuu", args,
			3, is_silent);
	return (is_passed);
}

bool	test_unsint_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "1";
	args[1] = NULL;
	args[2] = "3";
	is_passed = test_generic("int / NULL", "%u, %u, %u, viva l'Algérie!", "uuu", args, 3, is_silent);
	return (is_passed);
}

bool	test_unsint_maxout(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "1";
	args[1] = "4294967296";
	args[2] = "3";
	is_passed = test_generic("int / min - 1", "%d, %i, %d, viva l'Algérie!", "i", args,
			3, is_silent);
	return (is_passed);
}

// HEXA lower
bool	test_hexl_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "15";
	is_passed = test_generic("hexl / nominal", "%x = 15", "x", args,
			1, is_silent);
	return (is_passed);
}

bool	test_hexl_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = NULL;
	is_passed = test_generic("hexl / NULL", "%x = 15", "x", args, 1, is_silent);
	return (is_passed);
}

bool	test_hexl_empty(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "";
	is_passed = test_generic("hexl / empty", "%x = 15", "x", args,
			1, is_silent);
	return (is_passed);
}

// HEXA UPPER
bool	test_hexU_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "15";
	is_passed = test_generic("hexU / nominal", "%X = 15", "X", args,
			1, is_silent);
	return (is_passed);
}

bool	test_hexU_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = NULL;
	is_passed = test_generic("hexU / NULL", "%X = 15", "X", args, 1, is_silent);
	return (is_passed);
}

bool	test_hexU_empty(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;

	args[0] = "";
	is_passed = test_generic("hexU / empty", "%X = 15", "X", args,
			1, is_silent);
	return (is_passed);
}

// ADDRESS
bool	test_address_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *template = "42 address is %p";
	int i = 42;

	args[0] = ft_lutoa_base((unsigned long long) &i, BASE10);
	is_passed = test_generic("address / nominal", template, "p", args,
			1, is_silent);
	return (is_passed);
}

bool	test_address_null(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *template = "42 address is %p";

	args[0] = "0";
	is_passed = test_generic("address / NULL", template, "p", args, 1, is_silent);
	return (is_passed);
}

bool	test_address_string(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *template = "42 address is %p";
	char *add = "besson-bey";

	args[0] = ft_lutoa_base((unsigned long long) &add, BASE10);
	is_passed = test_generic("address / string", template, "p", args, 1, is_silent);
	return (is_passed);
}
// PERCENT
bool	test_percent_nominal(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *template = "Give your 100%%";

	is_passed = test_generic("% / nominal", template, "%", args, 0, is_silent);
	return (is_passed);
}

bool	test_percent_unknownspec(bool is_silent)
{
	char	*args[5] = {0};
	bool	is_passed;
	char	*template = "you are the %king";

	is_passed = test_generic("% / %k", template, "%", args, 0, is_silent);
	return (is_passed);
}

bool	test_percent_final(bool is_silent)
{
	char	*args[5] = {0};
	bool	is_passed;
	char	*template = "check here%";

	is_passed = test_generic("% / final", template, "%", args, 0, is_silent);
	return (is_passed);
}

// MISC
bool	test_misc_mixed1(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *template = "a char %c, a string %s, an int %i, an unsigned int %u";
	args[0] = "c";
	args[1] = "youpi";
	args[2] = "-42";
	args[3] = "-42";
	is_passed = test_generic("misc / mixed 1", template, "csiu", args,
			4, is_silent);
	return (is_passed);
}

bool	test_misc_mixed2(bool is_silent)
{
	char *args[5] = {0};
	bool	is_passed;
	char *template = "an hexa upp %X and low %x, an address %p";
	int i = 42;
	int *ptr = &i;

	args[0] = "42";
	args[1] = "42";
	args[2] = ft_lutoa_base((unsigned long long) &ptr, BASE10);
	is_passed = test_generic("misc / mixed 2", template, "Xxp", args,
			3, is_silent);
	return (is_passed);
}

bool	test_misc_no_arg_null(bool is_silent)
{
	char 	*args[5] = {0};
	bool	is_passed;
	char	*template = NULL;

	is_passed = test_generic("NULL template", template, "", args, 0, is_silent);
	return (is_passed);
}


///////  TESTS LAUNCHERS BY SPECIFIER

void 	test_char(int is_short)
{
	printf("\n=== CHARACTERS\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("empty", COLWIDTH, strlen("empty"));
		printf("\t|");
		center_text("", COLWIDTH, strlen(""));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_char_nominal(is_short));
		printf("\t|");
		print_res(test_char_empty(is_short));
		printf("\t|");
//		print_res(test_char_null(is_short));
		printf("\t\t|\n");
	}
	else
	{
		test_char_nominal(is_short);
		test_char_empty(is_short);
//		test_char_null(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_str(int is_short)
{
	printf("\n=== STRINGS\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("empty", COLWIDTH, strlen("empty"));
		printf("\t|");
		center_text("NULL", COLWIDTH, strlen("NULL"));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_str_nominal(is_short));
		printf("\t|");
		print_res(test_str_empty(is_short));
		printf("\t|");
		print_res(test_str_null(is_short));
		printf("\t|\n");
	}
	else
	{
		test_str_nominal(is_short);
		test_str_empty(is_short);
		test_str_null(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_int(int is_short)
{
	printf("\n=== INTEGER\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("", COLWIDTH, strlen(""));
		printf("\t|");
		center_text("", COLWIDTH, strlen(""));
		printf("\t|");
		center_text("MIN - 1", COLWIDTH, strlen("MIN - 1"));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_int_nominal(is_short));
		printf("\t|");
		printf("\t\t|");
//		print_res(test_int_null(is_short));
		printf("\t\t|");
		print_res(test_int_minout(is_short));
		printf("\t|\n");
	}
	else
	{
		test_int_nominal(is_short);
//		test_int_null(is_short);
		test_int_minout(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_unsint(int is_short)
{
	printf("\n=== UNSIGNED\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("", COLWIDTH, strlen("empty"));
		printf("\t|");
		center_text("", COLWIDTH, strlen(""));
		printf("\t|");
		center_text("MAX + 1", COLWIDTH, strlen("MAX + 1"));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_unsint_nominal(is_short));
		printf("\t|");
		printf("\t\t|");
//		print_res(test_unsint_null(is_short));
		printf("\t\t|");
		print_res(test_unsint_maxout(is_short));
		printf("\t|\n");
	}
	else
	{
		test_unsint_nominal(is_short);
//		test_unsint_null(is_short);
		test_unsint_maxout(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_hexU(int is_short)
{
	printf("\n=== HEXA UPPER\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("empty", COLWIDTH, strlen("empty"));
		printf("\t|");
		center_text("", COLWIDTH, strlen(""));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_hexU_nominal(is_short));
		printf("\t|");
		print_res(test_hexU_empty(is_short));
		printf("\t|");
//		print_res(test_hexU_null(is_short));
		printf("\t\t|\n");
	}
	else
	{
		test_hexU_nominal(is_short);
		test_hexU_empty(is_short);
//		test_hexU_null(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_hexl(int is_short)
{
	printf("\n=== HEXA lower\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("empty", COLWIDTH, strlen("empty"));
		printf("\t|");
		center_text("", COLWIDTH, strlen(""));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_hexl_nominal(is_short));
		printf("\t|");
		print_res(test_hexl_empty(is_short));
		printf("\t|");
//		print_res(test_hexl_null(is_short));
		printf("\t\t|\n");
	}
	else
	{
		test_hexl_nominal(is_short);
		test_hexl_empty(is_short);
//		test_hexl_null(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_address(int is_short)
{
	printf("\n=== ADDRESS\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		center_text("nom string", COLWIDTH, strlen("nom string"));
		printf("\t|");
		center_text("NULL", COLWIDTH, strlen("NULL"));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_address_nominal(is_short));
		printf("\t|");
		print_res(test_address_string(is_short));
		printf("\t|");
		print_res(test_address_null(is_short));
		printf("\t|\n");
	}
	else
	{
		test_address_nominal(is_short);
		test_address_string(is_short);
		test_address_null(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_percent(int is_short)
{
	printf("\n=== PERCENT\t");
	if (is_short)
	{
		center_text("nominal", COLWIDTH, strlen("nominal"));
		printf("\t|");
		printf("\t\t|");
		center_text("%k", COLWIDTH, strlen("%k"));
		printf("\t|");
		center_text("final", COLWIDTH, strlen("%k"));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_percent_nominal(is_short));
		printf("\t|");
		printf("\t\t|");
		print_res(test_percent_unknownspec(is_short));
		printf("\t|");
		print_res(test_percent_final(is_short));
		printf("\t|\n");
	}
	else
	{
		test_percent_nominal(is_short);
		test_percent_unknownspec(is_short);
		test_percent_final(is_short);
		printf("\n.............................................................\n");
	}
}

void 	test_misc(int is_short)
{
	printf("\n=== MISC.\t");
	if (is_short)
	{
		center_text("mixed 1", COLWIDTH, strlen("mixed 1"));
		printf("\t|");
		center_text("mixed 2", COLWIDTH, strlen("mixed 2"));
		printf("\t|");
		center_text("NULL", COLWIDTH, strlen("NULL"));
		printf("\t|\n");

		printf("\t\t");	
		print_res(test_misc_mixed1(is_short));
		printf("\t|");
		print_res(test_misc_mixed2(is_short));
		printf("\t|");
		print_res(test_misc_no_arg_null(is_short));
		printf("\t|\n");
	}
	else{
		test_misc_mixed1(is_short);
		test_misc_mixed2(is_short);
		test_misc_no_arg_null(is_short);
		printf("\n.............................................................\n");
	}
}

void	show_tests(char *types, char *mode)
{
	int	display_mode;

	if (!strcmp(mode, "detail"))
		display_mode = 0;
	else if (!strcmp(mode, "short"))
		display_mode = 1;

	if (ft_strchr(types, 'c'))
		test_char(display_mode);
	if (ft_strchr(types, 's'))
		test_str(display_mode);
	if (ft_strchr(types, 'd') || ft_strchr(types, 'i'))
		test_int(display_mode);
	if (ft_strchr(types, 'u'))
		test_unsint(display_mode);
	if (ft_strchr(types, 'x'))
		test_hexl(display_mode);
	if (ft_strchr(types, 'X'))
		test_hexU(display_mode);
	if (ft_strchr(types, 'p'))
		test_address(display_mode);
	if (ft_strchr(types, '%'))
		test_percent(display_mode);
	if (ft_strchr(types, 'm'))
		test_misc(display_mode);
}

int	main(int ac, char **av)
{
	(void) ac;
	if (ac != 3)
	{
		printf("\t\t\t\tusage : 2 args <specifiers> <mode>\n\
				*specifiers can be composed of following characters:\n\
				\tcsdiup : to test chars, strings, ...\n\
				\t%%\n\
				\tm for other tests\n\
				*mode : all/short/detail\n");
		return (0);
	}
	if (!strcmp(av[2], "all"))
	{
		show_tests(av[1], "short");	
		show_tests(av[1], "detail");	
	}
	else if (!strcmp(av[2], "short"))
		show_tests(av[1], "short");	
	else if (!strcmp(av[2], "detail"))
		show_tests(av[1], "detail");	
}
