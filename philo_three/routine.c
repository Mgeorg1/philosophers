#include "philo_three.h"

void	get_ticket(t_philo *philo)
{
	sem_wait(philo->all->ticket_sem);
}

void	return_ticket(t_philo *philo)
{
	sem_post(philo->all->ticket_sem);
}

void	life_cycle(t_philo *philo)
{
	get_ticket(philo);
	sem_wait(philo->all->fork);
	display_message(philo, "has taken a fork");
	sem_wait(philo->all->fork);
	display_message(philo, "has taken a fork");
	eat(philo);
	sem_post(philo->all->fork);
	sem_post(philo->all->fork);
	return_ticket(philo);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->eating = 0;
	philo->cur_ttodie = philo->all->t_start;
	pthread_create(&philo->death, NULL, death, (void *)philo);
	pthread_detach(philo->death);
	while (!philo->all->done && !philo->full)
	{
		life_cycle(philo);
		if (philo->eat >= philo->all->eat_num && philo->all->eat_num)
		{
			philo->full = 1;
			break ;
		}
		philo_sleep(philo);
		display_message(philo, "is thinking");
	}
	return (NULL);
}
