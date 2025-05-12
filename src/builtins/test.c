#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int	main(int ac, char *av[])
{
	char		*str;
	int			ret;

	if (getcwd(NULL, 0) == NULL && errno == ENOENT)
	{
		perror("getcwd");
	}
	ret = chdir(av[1]);
	if (ret)
		perror("chdir");
	else
	{
		str = getcwd(NULL, 0);
		printf("%s\n", str);
	}
}