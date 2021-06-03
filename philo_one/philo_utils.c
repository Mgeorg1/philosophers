#include "philo_one.h"

long long	gettime(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)(1000 * tp.tv_sec + tp.tv_usec / 1000));
}

int	print_error(char *s)
{
	printf("ERROR\n%s\n", s);
	return (1);
}

int	is_digit_str(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	fix_sleep(long long n, t_all *all)
{
	long long	t;

	t = gettime();
	while (gettime() - t < n && !all->done)
	{
		usleep(1);
	}
}

void	display_message(t_philo *philo, char *s)
{
	long long	t;

	t = gettime() - philo->all->t_start;
	if (philo->all->done)
		return ;
	pthread_mutex_lock(&philo->all->message);
	if (!philo->all->done)
		printf("%-8lli philo %-4i %s\n", t, philo->id + 1, s);
	pthread_mutex_unlock(&philo->all->message);
}
