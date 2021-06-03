#include "philo_one.h"

void	grab_fork(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->all->fork[i]);
	display_message(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->all->fork[i]);
}

void	get_ticket(t_philo *philo)
{
	int	done;

	done = 0;
	while (!done)
	{
		pthread_mutex_lock(&philo->all->ticket_mutex);
		if (philo->all->ticket > 0)
		{
			philo->all->ticket--;
			done = 1;
		}
		else
			done = 0;
		pthread_mutex_unlock(&philo->all->ticket_mutex);
	}
}

void	return_ticket(t_philo *philo)
{
	pthread_mutex_lock(&philo->all->ticket_mutex);
	philo->all->ticket++;
	pthread_mutex_unlock(&philo->all->ticket_mutex);
}

void	down_forks(t_philo *philo, int l_fork, int r_fork)
{
	pthread_mutex_unlock(&philo->all->fork[l_fork]);
	pthread_mutex_unlock(&philo->all->fork[r_fork]);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		r_fork;
	int		l_fork;

	philo = (t_philo *)arg;
	r_fork = philo->id;
	l_fork = philo->id + 1;
	if (l_fork == philo->all->phl_num)
		l_fork = 0;
	pthread_create(&philo->death, NULL, death, (void *)philo);
	pthread_detach(philo->death);
	while (!philo->all->done)
	{
		get_ticket(philo);
		grab_fork(philo, r_fork);
		grab_fork(philo, l_fork);
		eat(philo);
		down_forks(philo, l_fork, r_fork);
		return_ticket(philo);
		if (philo->eat >= philo->all->eat_num && philo->all->eat_num)
			break ;
		philo_sleep(philo);
		display_message(philo, "is thinking");
	}
	return (NULL);
}
