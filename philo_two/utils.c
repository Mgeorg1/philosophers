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

	b = 922337203685477580;
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

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			i;

	a = (unsigned char *)s1;
	b = (unsigned char *)s2;
	i = 0;
	while ((*a || *b) && i < n)
	{
		if (*a != *b)
		{
			if (*a > *b)
				return (1);
			else if (*a < *b)
				return (-1);
			else
				return (0);
		}
		a++;
		b++;
		i++;
	}
	return (0);
}

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
