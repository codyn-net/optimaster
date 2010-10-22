#ifndef __OPTIMASTER_COMMAND_H__
#define __OPTIMASTER_COMMAND_H__

#include <jessevdk/network/network.hh>
#include <jessevdk/os/os.hh>
#include <optimization/messages.hh>

#include "job.hh"

namespace optimaster
{
	class Application;

	class Command : public jessevdk::network::TcpServer
	{
		struct Data : public jessevdk::network::TcpServer::Data
		{
			struct ClientData
			{
				std::string challenge;
				bool authenticated;
			};

			Data(Application &application);

			protected:
				virtual jessevdk::network::Client Accept();
			private:
				Application &application;
				std::map<int, ClientData> clientData;

				bool CheckAuthentication(jessevdk::network::Client                    &client,
				                         optimization::messages::command::CommandType  type);

				void OnClientData(jessevdk::os::FileDescriptor::DataArgs &args, jessevdk::network::Client client);
				void OnClientClosed(int fd, jessevdk::network::Client client);

				void GenerateChallenge(jessevdk::network::Client &client, optimization::messages::command::AuthenticateCommand const &command);
				void ValidateChallenge(jessevdk::network::Client &client, optimization::messages::command::AuthenticateCommand const &command);

				void Respond(jessevdk::network::Client &client, optimization::messages::command::CommandType type, bool status, std::string const &message);
				void Respond(jessevdk::network::Client &client, optimization::messages::command::Response &response);

				void FillJob(Job const                            &job,
				             optimization::messages::command::Job &cjob);

				void HandleList(jessevdk::network::Client                          &client,
				                optimization::messages::command::ListCommand const &command);

				void HandleInfo(jessevdk::network::Client                          &client,
				                optimization::messages::command::InfoCommand const &command);

				void HandleKill(jessevdk::network::Client                          &client,
				                optimization::messages::command::KillCommand const &command);

				void HandleSetPriority(jessevdk::network::Client                                 &client,
				                       optimization::messages::command::SetPriorityCommand const &command);

				void HandleAuthenticate(jessevdk::network::Client                                  &client,
				                        optimization::messages::command::AuthenticateCommand const &command);

				void HandleIdle(jessevdk::network::Client                          &client,
				                optimization::messages::command::IdleCommand const &command);

				optimization::messages::command::Response CreateResponse(optimization::messages::command::CommandType  type,
				                                                         bool                                          status,
				                                                         std::string const                            &message = "") const;
		};

		Data *d_data;

		public:
			/* Constructor/destructor */
			Command(Application &application);
		private:
			/* Private functions */
	};
}

#endif /* __OPTIMASTER_COMMAND_H__ */
