#include "./philo_two.h"

long long	gettime(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)(1000 * tp.tv_sec + tp.tv_usec / 1000));
}

int	print_error(char *s)
{
	printf("ERROR\n%s\n", s);
	return (1);
}

int	is_digit_str(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	fix_sleep(long long n)
{
	long long	t;
	t = gettime();
	while (gettime() - t < n)
	{
		usleep(1);
	}
}

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
	if (all->phl_num < 2 || all->ttodie < 1 || all->ttoeat < 1 || all->ttosleep < 1)
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

void	display_message(t_philo *philo, char *s)
{
	long long t;
	t = gettime() - philo->all->t_start;
	if (philo->all->done)
		return ;
	sem_wait(philo->all->message);
	if (!philo->all->done && !philo->full)
		printf("%-8lli philo %-4i %s\n", t, philo->id + 1, s);
	sem_post(philo->all->message);
}

void	eat(t_philo *philo)
{
	philo->eating = 1;
	philo->cur_ttodie = gettime();
	sem_wait(philo->eat_sem);
	display_message(philo, "is eating");
	fix_sleep(philo->all->ttoeat);
	philo->eat++;
	sem_post(philo->eat_sem);
	philo->eating = 0;
}

void	philo_sleep(t_philo *philo)
{
	display_message(philo, "is sleeping");
	fix_sleep(philo->all->ttosleep);
}

void	*death(void *arg)
{
	t_philo		*philo;
	long long	t;

	philo = (t_philo*)arg;
	while (!philo->all->done)
	{
		sem_wait(philo->eat_sem);
		t = gettime() - philo->cur_ttodie;
		if (t > philo->all->ttodie && !philo->eating)
		{
			sem_wait(philo->all->status);
			display_message(philo, "is died");
			sem_post(philo->all->die);
			philo->all->done = 1;
			sem_post(philo->eat_sem);
			exit(0);
		}
		sem_post(philo->eat_sem);
	}
	return (NULL);
}

void	get_ticket(t_philo *philo)
{
	sem_wait(philo->all->ticket_sem);
}

void	return_ticket(t_philo *philo)
{
	sem_post(philo->all->ticket_sem);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->eating = 0;
	philo->cur_ttodie = philo->all->t_start;
	pthread_create(&philo->death, NULL, death, (void*)philo);
	pthread_detach(philo->death);
	while (!philo->all->done && !philo->full)
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

void	*is_die(void *arg)
{
	t_all	*all;
	int		i;

	all = arg;
	sem_wait(all->die);
	i = -1;
	while (++i < all->phl_num)
	{
		kill(all->philo[i].pid, SIGTERM);
	}
	return (0);
}

int	philo_init(t_all *all)
{
	int	i;

	i = -1;
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
	all->done = 0;
	i = -1;
	sem_wait(all->die);
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
	pthread_create(&all->is_die, NULL, is_die, all);
	pthread_detach(all->is_die);
	i = -1;
	while (++i < all->phl_num)
	{
		waitpid(all->philo[i].pid, 0, 0);
	}
	return (0);
}

void	clear_all(t_all *all)
{
	int i;

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

int	main(int argc, char **argv)
{
	int		i;
	t_all	all;


	if (argc > 6 || argc < 5)
		return (print_error("incorrect num of arguments"));
	i = 1;
	while (argv[i])
	{
		if (!is_digit_str(argv[i]))
			return (print_error("Argument is not digit"));
		i++;
	}
	if (init_args(&all, argc, argv))
		return (1);
	philo_init(&all);
	// pthread_mutex_lock(&all.death);
	// pthread_mutex_unlock(&all.death);
	//	fix_sleep(1000000);
	clear_all(&all);
	return(0);
}