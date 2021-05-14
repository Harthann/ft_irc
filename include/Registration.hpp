/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stbaleba <stbaleba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/25 18:04:50 by stbaleba          #+#    #+#             */
/*   Updated: 2021/04/25 18:07:47 by stbaleba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGISTRATION_HPP
# define REGISTRATION_HPP
# include "Socket.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Commands.hpp"
# include <vector>
# include <iostream>

void	add_user(Socket *client, std::vector<User> &temp_user, Commands &cmd);
int		already_register(Socket *client, Server &server);
void	update_server_user(std::vector<User> &temp_user, Server &server);

#endif
