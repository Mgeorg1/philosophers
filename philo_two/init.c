#include "philo_two.h"

int	init_args(t_all *all, int argc, char **argv)
{
	int	i;

	if (argc == 6)
		all->eat_num = ft_atoi(argv[5]);
	else
		all->eat_num = 0;
	all->phl_num = ft_atoi(argv[1]);
	all->ttodie = ft_atoi(argv[2]);
	all->ttoeat = ft_atoi(argv[3]);
	all->ttosleep = ft_atoi(argv[4]);
	if (all->phl_num < 2 || all->ttodie < 1
		|| all->ttoeat < 1 || all->ttosleep < 1)
		return (print_error("wrong argument"));
	all->philo = malloc(sizeof(t_philo) * (all->phl_num));
	if (!all->philo)
		return (print_error("malloc error"));
	if (!all->fork)
		return (print_error("malloc error"));
	all->t_start = gettime();
	i = -1;
	while (++i < all->phl_num)
		all->philo[i].all = all;
	all->philo->all = all;
	return (0);
}

void	sems_unlink(void)
{
	sem_unlink("fork");
	sem_unlink("message");
	sem_unlink("eat");
	sem_unlink("ticket");
	sem_unlink("status");
}

int	init_sem(t_all *all)
{
	int	i;

	sems_unlink();
	all->fork = sem_open("fork", O_CREAT, 0644, all->phl_num);
	if (all->fork == SEM_FAILED)
		return (print_error("FATAL"));
	all->message = sem_open("message", O_CREAT, 0644, 1);
	if (all->message == SEM_FAILED)
		return (print_error("FATAL"));
	all->ticket_sem = sem_open("ticket", O_CREAT, 0644, all->phl_num - 1);
	if (all->ticket_sem == SEM_FAILED)
		return (print_error("FATAL"));
	all->status = sem_open("status", O_CREAT, 0644, 1);
	if (all->status == SEM_FAILED)
		return (print_error("FATAL"));
	i = -1;
	while (++i < all->phl_num)
	{
		sem_unlink(ft_itoa(i));
		all->philo[i].eat_sem = sem_open(ft_itoa(i), O_CREAT, 0644, 1);
		if (all->philo[i].eat_sem == SEM_FAILED)
			return (print_error("FATAL"));
	}
	return (0);
}

int	philo_init(t_all *all)
{
	int	i;

	if (init_sem(all))
		return (1);
	all->done = 0;
	i = -1;
	while (++i < all->phl_num)
	{	
		all->philo[i].id = i;
		all->philo[i].eat = 0;
		all->philo[i].full = 0;
		if (pthread_create(&all->philo[i].m_philo, NULL,
				&routine, (void *)&all->philo[i]))
			return (print_error("FATAL: can't create thread"));
		fix_sleep(1, all);
	}
	i = -1;
	while (++i < all->phl_num)
	{
		if (pthread_join(all->philo[i].m_philo, NULL))
			return (print_error("FATAL: can't join thread"));
	}
	all->done = 1;
	return (0);
}

void	clear_all(t_all *all)
{
	int	i;

	i = -1;
	while (++i < all->phl_num)
	{
		sem_unlink(ft_itoa(i));
		sem_close(all->philo[i].eat_sem);
	}
	free(all->philo);
	sem_unlink("message");
	sem_close(all->message);
	sem_unlink("fork");
	sem_close(all->fork);
	sem_unlink("ticket");
	sem_close(all->ticket_sem);
}
