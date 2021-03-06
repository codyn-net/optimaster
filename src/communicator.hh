/*
 * communicator.hh
 * This file is part of optimaster
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
 *
 * optimaster is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * optimaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with optimaster; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef __OPTIMASTER_COMMUNICATOR_H__
#define __OPTIMASTER_COMMUNICATOR_H__

#include <jessevdk/base/base.hh>
#include <optimization/messages.hh>
#include <jessevdk/network/network.hh>

namespace optimaster
{
	class Communicator : public jessevdk::base::Object
	{
		public:
			struct CommunicationArgs
			{
				/** Communicator source object */
				Communicator &Source;
				
				/** Communication message object */
				optimization::messages::task::Communication &Communication;

				CommunicationArgs(Communicator &source, optimization::messages::task::Communication &communication);
			};

			Communicator();

			template <typename T>
			bool Send(T const &message);

			size_t Id() const;

			bool operator==(size_t id) const;
			bool operator==(Communicator const &other) const;

			jessevdk::network::Client &Client();
			jessevdk::network::Client const &Client() const;

			jessevdk::base::signals::Signal<CommunicationArgs> &OnCommunication();
			jessevdk::base::signals::Signal<Communicator> &OnClosed();
		protected:
			struct Data : public jessevdk::base::Object::PrivateData
			{
				jessevdk::network::Client client;

				size_t id;

				jessevdk::base::signals::Signal<CommunicationArgs> onCommunication;
				jessevdk::base::signals::Signal<Communicator> onClosed;
				
				void OnData(jessevdk::os::FileDescriptor::DataArgs &args);
				void OnClosed(int fd);
				
				~Data();
			};
			
			void Set(Data *data, jessevdk::network::Client const &client);
			Communicator(Data *data);
		private:
			Data *d_data;
	};

	/**
 * @brief Send message to the communicator end point
	 * @param message the message to send
	 *
	 * Send message to the communicator end point
	 *
	 * @return true if the message was send successfully, false otherwise
	 */
	template <typename T>
	bool Communicator::Send(T const &message)
	{
		std::string serialized;
		
		if (optimization::Messages::Create(message, serialized))
		{
			d_data->client.Write(serialized);
			return true;
		}
		
		return false;
	}
}

#endif /* __OPTIMASTER_COMMUNICATOR_H__ */
