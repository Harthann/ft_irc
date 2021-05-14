/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_Registration.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nieyraud <nieyraud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/25 18:08:42 by stbaleba          #+#    #+#             */
/*   Updated: 2021/05/14 12:01:40 by nieyraud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_REGISTRATION_HPP
# define CHANNEL_REGISTRATION_HPP

# include "Socket.hpp"
# include "User.hpp"
# include "Server.hpp"
# include "Commands.hpp"
# include <vector>
# include <iostream>

User	 *check_user(std::vector<User *>server_users, Socket *client);
int		channel_exist(std::string name, Server &server);
void	add_member(User *user, Server &server, std::string name);
void	add_to_channel(Commands cmd, Socket *client, Server &server);
void	part_from_channel(Commands cmd, Socket *client, Server &server);


#endif
