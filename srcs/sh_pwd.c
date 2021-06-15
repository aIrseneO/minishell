/******************************************************************************/
/*     Copyright (c) 2020 Arsene Temfack                                      */
/*                                                                            */
/*     SPDX-License-Identifier: MIT                                           */
/******************************************************************************/

#include "minishell.h"

int	sh_pwd(t_data *data, int i)
{
	(void)i;
	ft_putendl_fd(data->pwd, STDOUT_FILENO);
	return (0);
}
