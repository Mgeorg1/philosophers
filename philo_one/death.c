#include "philo_one.h"

void	*death(void *arg)
{
	t_philo		*philo;
	long long	t;

	philo = (t_philo *)arg;
	while (!philo->all->done)
	{
		pthread_mutex_lock(&philo->eat_mutex);
		t = gettime() - philo->cur_ttodie;
		if (t > philo->all->ttodie && !philo->eating)
		{
			pthread_mutex_lock(&philo->all->status);
			display_message(philo, "is died");
			philo->all->done = 1;
			pthread_mutex_unlock(&philo->eat_mutex);
			pthread_mutex_unlock(&philo->all->status);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->eat_mutex);
		fix_sleep(10, philo->all);
	}
	return (NULL);
}
