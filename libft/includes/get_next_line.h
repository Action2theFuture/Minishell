/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:28:16 by junsan            #+#    #+#             */
/*   Updated: 2024/05/12 19:12:25 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

char	*gnl_strchr(const char *s, size_t len);
char	*gnl_strjoin(char *s1, char *s2, size_t len, size_t rd);
char	*get_next_line(int fd);

#endif /* GET_NEXT_LINE_H */
