#include "pipex.h"
#include "libft.h"

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

void	child(char **av, int *p_fd, char **env)
{
	int		fd;

	close(p_fd[0]);
	fd = open_file(av[1], 0);
	if (fd == -1) {
		close(p_fd[1]);
		exit(0);
	}
	dup2(fd, 0);
	close(fd); // close
	dup2(p_fd[1], 1);
	close(p_fd[1]);
	exec(av[2], env);
}


void	parent_process(char **av, int *p_fd, char **env)
{
	int		fd;

	close(p_fd[1]);
	fd = open_file(av[4], 1);
	dup2(fd, 1);
	close(fd); // close
	dup2(p_fd[0], 0);
	close(p_fd[0]);
	exec(av[3], env);
}

int	main(int ac, char **av, char **env)
{
	int		p_fd[2];
	int		status;
	__pid_t	pid1;
	__pid_t	pid2;

	if (ac != 5)
		exit_handler(1);
	if (pipe(p_fd) == -1)
		exit(-1);
	pid1 = fork();
	if (pid1 == -1)
		exit(-1);
	if (!pid1)
		child(av, p_fd, env);
	pid2 = fork();
	 if (pid2 == -1)
        exit(-1);
	if (!pid2)
		parent_process(av, p_fd, env);
	close (p_fd[0]);
	close (p_fd[1]);
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		ft_putstr_fd("pipex: First child exited with an error\n", 2);
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		ft_putstr_fd("pipex: Second child exited with an error\n", 2);
	return (0);
}