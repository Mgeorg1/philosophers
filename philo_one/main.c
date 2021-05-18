#include "philo_one.h"

long int	gettime(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return (1000 * tp.tv_sec + tp.tv_usec / 1000);
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

int	init_args(t_all *all, int argc, char **argv)
{
	int i;

	if (argc == 6)
		all->eat_num = ft_atoi(argv[5]);
	else
		all->eat_num = 1;
	all->phl_num = ft_atoi(argv[1]);
	all->ttodie = ft_atoi(argv[2]);
	all->ttoeat = ft_atoi(argv[3]);
	all->ttosleep = ft_atoi(argv[4]);
	all->philo = malloc(sizeof(t_philo) * (all->phl_num));
	if (!all->philo)
		return (1);
	all->fork = malloc(sizeof(pthread_mutex_t) * (all->phl_num));
	if (!all->fork)
		return (1);
	all->t_start = gettime();
	i = -1;
	while (++i < all->phl_num)
		all->philo[i].all = all;
	all->philo->all = all;
	return (0);
}

void	display_message(t_all *all, int philo, char *s, long int tstmp)
{
	tstmp = tstmp - all->t_start;
	pthread_mutex_lock(&all->message);
	printf("%-8ld philo %i %s\n", tstmp, philo, s);
	pthread_mutex_unlock(&all->message);
}


void grab_fork(t_philo *philo, int i)
{
	long int	tmsp;

	pthread_mutex_lock(&philo->all->fork[i]);
	tmsp = gettime();
	display_message(philo->all, philo->id + 1, "has taken fork", tmsp); // получить время 
}

void	eat(t_philo *philo, int l, int r)
{
	long int	tmsp;

	tmsp = gettime();
	display_message(philo->all, philo->id, "is eating", tmsp);
	usleep(philo->all->ttoeat * 1000);
	philo->eat++;
	pthread_mutex_unlock(&philo->all->fork[r]);
	pthread_mutex_unlock(&philo->all->fork[l]);
}

void	ph_sleep(t_philo *philo)
{
	long int tmsp;

	tmsp = gettime();
	display_message(philo->all, philo->id + 1, "is sleeping", tmsp);
	usleep(philo->all->ttosleep * 1000);
}

void	*death(void *arg)
{
	t_philo	*philo;
	long int t;

	philo = arg;
	pthread_mutex_init(&philo->all->death, NULL);
	while (1)
	{
		t = gettime();
		t -= philo->all->t_start;
		if (t > philo->all->ttodie)
		{
			display_message(philo->all, philo->id + 1, "died", gettime());
			exit(0);
		}
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	philo = arg;
	philo->r_fork = philo->id;
	philo->l_fork = philo->id + 1;
	pthread_create(&philo->death, NULL, death, arg);
	if (philo->l_fork == philo->all->phl_num)
		philo->l_fork = 0;
	while (1)
	{
		while (philo->eat < philo->all->eat_num)
		{
			grab_fork(philo, philo->r_fork);
			grab_fork(philo, philo->l_fork);
			eat(philo, philo->l_fork, philo->r_fork);
			ph_sleep(philo);
			display_message(philo->all, philo->id + 1, "is thinking", gettime());
		}
	}
	return (NULL);
}

int	init_philos(t_all *all)
{
	int	i;

	i = -1;
	pthread_mutex_init(&all->message, NULL);
	while (++i < all->phl_num)
		pthread_mutex_init(&all->fork[i], NULL);
	i = -1;
	while (++i < all->phl_num)
	{	
		all->philo[i].id = i;
		all->philo[i].eat = 0;
		pthread_create(&all->philo[i].m_philo, NULL, &routine, (void *)&all->philo[i]);
		usleep(100);
	}
	i = -1;
	while (++i < all->phl_num)
		pthread_join(all->philo[i].m_philo, NULL);
	return (0);
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
	init_args(&all, argc, argv);
	init_philos(&all);
}