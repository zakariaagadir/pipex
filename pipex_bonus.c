
#include "pipex_bonus.h"
#include "get_next_line_bonus.h"

void	exec(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	if(!s_cmd)
		exit(-1);
	path = get_path(s_cmd[0], env);
	if (!path)
	{
		ft_free_tab(s_cmd);
		exit(1);
	}
	if (execve(path, s_cmd, env) == -1)
	{
		perror ("pipex: command not found: ");
		perror (s_cmd[0]);
		ft_free_tab(s_cmd);
		free(path); //
		exit(-1);
	}
		free(path); //

}

void	here_doc_put_in(char **av, int *p_fd)
{
	char	*ret;

	close(p_fd[0]);
	while (1)
	{
		ret = get_next_line(0);
		if (!ret) {
    		ft_putstr_fd("pipex: error reading from stdin\n", 2);
    		exit(1);
		}
		if (ft_strncmp(ret, av[2], ft_strlen(av[2])) == 0) {
    		free(ret);
    		exit(0);
		}
		ft_putstr_fd(ret, p_fd[1]);
		free(ret);

	}
}

void	here_doc(char **av)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (!pid)
		here_doc_put_in(av, p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], 0);
		wait(NULL);
	}
}

void	do_pipe(char *cmd, char **env)
{
	pid_t	pid;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (!pid)
	{
		close(p_fd[0]);
		dup2(p_fd[1], 1);
		exec(cmd, env);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], 0);
	}
}

int	main(int ac, char **av, char **env)
{
	int		i;
	int		fd_in;
	int		fd_out;

	if (ac < 5)
		exit_handler(1);
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		if (ac < 6)
			exit_handler(1);
		i = 3;
		fd_out = open_file(av[ac - 1], 2);
		here_doc(av);
	}
	else
	{
		i = 2;
		fd_in = open_file(av[1], 0);
		fd_out = open_file(av[ac - 1], 1);
		dup2(fd_in, 0);
	}
	while (i < (ac - 2))
		do_pipe(av[i++], env);
	dup2(fd_out, 1);
	exec(av[ac - 2], env);
}