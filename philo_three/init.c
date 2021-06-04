#include "philo_three.h"

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
	all->t_start = gettime();
	i = -1;
	while (++i < all->phl_num)
		all->philo[i].all = all;
	all->philo->all = all;
	return (0);
}

int	sems_init(t_all *all)
{
	sem_unlink("fork");
	sem_unlink("message");
	sem_unlink("eat");
	sem_unlink("ticket");
	sem_unlink("die");
	sem_unlink("status");
	all->fork = sem_open("fork", O_CREAT, 0644, all->phl_num);
	if (all->fork == SEM_FAILED)
		return (print_error("FATAL"));
	all->die = sem_open("die", O_CREAT, 0644, 1);
	if (all->die == SEM_FAILED)
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
	sem_wait(all->die);
	return (0);
}

int	forking(t_all *all)
{
	int	i;

	i = -1;
	while (++i < all->phl_num)
	{	
		all->philo[i].id = i;
		all->philo[i].eat = 0;
		all->philo[i].full = 0;
		sem_unlink(ft_itoa(i));
		all->philo[i].eat_sem = sem_open(ft_itoa(i), O_CREAT, 0644, 1);
		if (all->philo[i].eat_sem == SEM_FAILED)
			return (print_error("FATAL"));
		all->philo[i].pid = fork();
		if (all->philo[i].pid < 0)
			return (print_error("FATAL: can not creat proccess"));
		if (all->philo[i].pid == 0)
		{
			routine(&all->philo[i]);
			exit(EXIT_SUCCESS);
		}
	}
	return (0);
}

int	philo_init(t_all *all)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (sems_init(all))
		return (1);
	all->done = 0;
	if (forking(all) < 0)
		return (1);
	if (pthread_create(&all->is_die, NULL, is_die, all) < 0)
		return (print_error("FATAL:can't create thread"));
	if (pthread_detach(all->is_die) < 0)
		return (print_error("FATAL"));
	i = -1;
	while (++i < all->phl_num)
		waitpid(all->philo[i].pid, 0, 0);
	sem_post(all->die);
	return (0);
}

void	clear_all(t_all *all)
{
	int	i;

	i = -1;
	sem_unlink("eat");
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
	sem_unlink("die");
	sem_close(all->die);
	sem_unlink("status");
	sem_close(all->status);
}
