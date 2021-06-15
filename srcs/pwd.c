/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include <unistd.h>
#include "libft.h"

int	main(int ac, char **av)
{
	char	pwd[4096];

	(void)ac;
	if (!getcwd(pwd, 4096))
	{
		ft_putstr_fd(av[0], 1);
		ft_putendl_fd(
			": couldn't find directory entry in ‘..’ with matching i-node", 1);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	return (0);
}
