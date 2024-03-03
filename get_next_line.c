/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbongout <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:04:21 by dbongout          #+#    #+#             */
/*   Updated: 2024/02/25 16:05:15 by dbongout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*increase_buffer_size(char *buffer, int *capacity, int total_read)
{
	char	*temp;

	*capacity *= 2;
	temp = (char *)malloc((*capacity + 1) * sizeof(char));
	if (!temp)
	{
		free(buffer);
		return (NULL);
	}
	ft_strncpy(temp, buffer, total_read);
	free(buffer);
	return (temp);
}

static char	*read_and_store(int fd, char *buffer, int *total_read, int *capacity)
{
	int	bytes_read;

	bytes_read = read(fd, buffer + *total_read, 1);
	while (bytes_read > 0)
	{
		*total_read += bytes_read;
		if (*total_read >= *capacity)
		{
			buffer = increase_buffer_size(buffer, capacity, *total_read);
			if (!buffer)
				return (NULL);
		}
		if (buffer[*total_read - 1] == '\n')
			break ;
		bytes_read = read(fd, buffer + *total_read, 1);
	}
	if (bytes_read < 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[*total_read] = '\0';
	return (buffer);
}

static char	*store_line(int fd, char **storage, int *buffer_size)
{
	int		total_read;
	int		capacity;
	char	*buffer;

	total_read = 0;
	capacity = *buffer_size;
	buffer = (char *)malloc((capacity + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	buffer = read_and_store(fd, buffer, &total_read, &capacity);
	if (!buffer || total_read == 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[total_read] = '\0';
	*storage = buffer;
	*buffer_size = capacity;
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*storage = NULL;
	static int	buffer_size = BUFFER_SIZE;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		free(storage);
		storage = NULL;
		return (NULL);
	}
	if (!storage)
	{
		storage = ft_strdup("");
		if (!storage)
			return (NULL);
	}
	storage = store_line(fd, &storage, &buffer_size);
	return (storage);
}
