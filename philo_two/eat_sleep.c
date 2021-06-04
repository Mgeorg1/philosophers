#include "philo_two.h"

void	eat(t_philo *philo)
{
	philo->eating = 1;
	sem_wait(philo->eat_sem);
	display_message(philo, "is eating");
	philo->cur_ttodie = gettime();
	fix_sleep(philo->all->ttoeat, philo->all);
	philo->eat++;
	sem_post(philo->eat_sem);
	philo->eating = 0;
}

void	philo_sleep(t_philo *philo)
{
	display_message(philo, "is sleeping");
	fix_sleep(philo->all->ttosleep, philo->all);
}
