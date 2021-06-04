#include "philo_three.h"

void	*death(void *arg)
{
	t_philo		*philo;
	long long	t;

	philo = (t_philo *)arg;
	while (!philo->all->done)
	{
		sem_wait(philo->eat_sem);
		t = gettime() - philo->cur_ttodie;
		if (t > philo->all->ttodie && !philo->eating)
		{
			sem_wait(philo->all->status);
			display_message(philo, "is died");
			philo->all->done = 1;
			sem_post(philo->all->die);
			sem_post(philo->eat_sem);
			exit(0);
		}
		sem_post(philo->eat_sem);
	}
	return (NULL);
}

void	*is_die(void *arg)
{
	t_all	*all;
	int		i;

	all = arg;
	i = -1;
	sem_wait(all->die);
	while (++i < all->phl_num)
	{
		kill(all->philo[i].pid, SIGTERM);
	}
	return (0);
}
