#include "philo_one.h"

void	eat(t_philo *philo)
{
	philo->eating = 1;
	pthread_mutex_lock(&philo->eat_mutex);
	philo->cur_ttodie = gettime();
	display_message(philo, "is eating");
	fix_sleep(philo->all->ttoeat, philo->all);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_mutex);
	philo->eating = 0;
}

void	philo_sleep(t_philo *philo)
{
	display_message(philo, "is sleeping");
	fix_sleep(philo->all->ttosleep, philo->all);
}
