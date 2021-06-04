#include "./philo_one.h"

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
			return (print_error("Argument is not natural number"));
		i++;
	}
	if (init_args(&all, argc, argv))
		return (1);
	if (philo_init(&all) < 0)
		return (1);
	clear_all(&all);
	return (0);
}
