#include "philo_two.h"

void	get_ticket(t_philo *philo)
{
	sem_wait(philo->all->ticket_sem);
}

void	return_ticket(t_philo *philo)
{
	sem_post(philo->all->ticket_sem);
}

void	is_full(t_philo *philo)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (++i < philo->all->phl_num)
	{
		if (philo->all->philo[i].eat == philo->all->eat_num)
			count++;
	}
	if (count == philo->all->phl_num)
	{
		philo->all->done = 1;
	}
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
	is_full(philo);
	return (NULL);
}
