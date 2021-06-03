#include "philo_two.h"

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
			sem_post(philo->eat_sem);
			sem_post(philo->all->status);
			return (NULL);
		}
		sem_post(philo->eat_sem);
		fix_sleep(10, philo->all);
	}
	return (NULL);
}
