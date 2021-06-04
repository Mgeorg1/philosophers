#include "philo_two.h"

static int	diglen(int n)
{
	int	l;
	int	n1;

	l = 0;
	n1 = n;
	if (n == 0)
		return (1);
	while (n1 != 0)
	{
		l++;
		n1 = n1 / 10;
	}
	return (l);
}

static int	is_negative(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

char	*ft_itoa(int n)
{
	int			len;
	long long	n1;
	char		*a;
	int			i;

	n1 = (long long)n;
	i = 0;
	len = diglen(n) + is_negative(n);
	if (n < 0)
		n1 *= -1;
	a = (char *)malloc((len + 1) * sizeof(char));
	if (a == NULL)
		return (NULL);
	if (n1 == 0)
		a[0] = '0';
	while (n1 != 0)
	{
		a[len - 1 - i] = n1 % 10 + '0';
		n1 = n1 / 10;
		i++;
	}
	a[len] = 0;
	if (n < 0)
		a[0] = '-';
	return (a);
}
