#include "vm.h"

static void add_champ(t_args *args, int idx, const char *champ_name)
{
	ft_assert(idx >= 0 && idx < 4, "bad player number #%d", idx);
	ft_assert(!args->champs[idx], "player #%d already exists", idx);
	args->champs[idx] = champ_name;
}

char        *get_error(t_uint mode)
{
    if (mode == MODE_VERBOSE)
        return ("-v N      : Verbosity levels, can be added together to enable several\n"
                "            - 16 : Show PC movements (Except for jumps)");
    return("USGE: ./corewar [-i | -v | -p | -d N] [[-n N] CHAMP] ...");
}

static void parse_arg(t_args *args, int ac, char **av, int *i)
{
	int idx;

	if (!ft_strcmp("-i", av[*i]))
		args->mode = MODE_VIS;
	if(!ft_strcmp("-v", av[*i]))
    {
		args->mode = MODE_VERBOSE;
        ft_assert(++(*i) < ac, get_error(args->mode));
        args->dump = ft_atoi(av[*i]);
    }
	else if (!ft_strcmp("-p", av[*i]))
		args->mode = MODE_PRINT;
	else if (!ft_strcmp("-d", av[*i]))
	{
		args->mode = MODE_DUMP;
		ft_assert(++(*i) < ac, VM_USAGE);
		args->dump = ft_atoi(av[*i]);
	}
	else if (!ft_strcmp("-n", av[*i]))
	{
		ft_assert(++(*i) < ac, VM_USAGE);
		idx = ft_atoi(av[*i]) - 1;
		ft_assert(++(*i) < ac, VM_USAGE);
		add_champ(args, idx, av[*i]);
	}
	else
		add_champ(args, ft_len((void **)&args->champs[0]), av[*i]);
}

void parse_args(t_args *args, int ac, char **av)
{
	int i;

	ft_assert(ac > 1, VM_USAGE);
	i = 1;
	ft_bzero(args, sizeof(t_args));
	args->mode = MODE_DEFAULT;
	while (i < ac)
	{
		parse_arg(args, ac, av, &i);
		++i;
	}
}
