#include "./philo_two.h"

static int	ft_isspace(const char *str)
{
	if (*str == ' ' || *str == '\t' || *str == '\r'
		|| *str == '\n' || *str == '\v' || *str == '\f')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long long	i;
	int			k;
	long long	b;

	b = 214748364;
	k = 1;
	i = 0;
	while (ft_isspace(str) == 1)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			k = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if ((i > b || (i == b && (*str - '0') > 7)) && k == 1)
			return (-1);
		if ((i > b || (i == b && (*str - '0') > 8)) && k == -1)
			return (0);
		i = i * 10 + *str - 48;
		str++;
	}
	return (k * i);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
