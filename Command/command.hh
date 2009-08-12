#ifndef __MASTER_COMMAND_H__
#define __MASTER_COMMAND_H__

#include <network/network.hh>
#include <os/os.hh>
#include <optimization/Messages/messages.hh>
#include "Job/job.hh"

namespace master
{
	class Application;

	class Command : public network::TcpServer
	{
		struct Data : public base::Object::PrivateData
		{
			network::Client signalClient;

			Application &application;

			void onNewConnection(network::Client &client);
			void onData(os::FileDescriptor::DataArgs &args, network::Client client);
			
			void response(network::Client &client, optimization::messages::command::CommandType type, bool status, std::string const &data);
			
			void handleAddJob(network::Client &client, optimization::messages::command::AddJobCommand const &command);
			void handleRemoveJob(network::Client &client, optimization::messages::command::RemoveJobCommand const &command);
			void handleListJobs(network::Client &client, optimization::messages::command::ListJobsCommand const &command);
			void handleJobProgress(network::Client &client, optimization::messages::command::JobProgressCommand const &command);
			void handleJobInfo(network::Client &client, optimization::messages::command::JobInfoCommand const &command);
			
			void setJobInfo(Job &job, optimization::messages::command::Job &message);
			void setIteration(db::Row &row, optimization::messages::command::Iteration &iteration);
			
			optimization::messages::command::Response createResponse(optimization::messages::command::CommandType type, bool status, std::string const &data) const;
			
			Data(Application &application);
		};
		
		Data *d_data;
		
		public:
			/* Constructor/destructor */
			Command(Application &application);
			
			void jobAdded(Job &job);
			void jobRemoved(Job &job);
			void newIteration(Job &job);
		private:
			/* Private functions */
	};
}

#endif /* __MASTER_COMMAND_H__ */
