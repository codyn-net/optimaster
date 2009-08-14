#ifndef __MASTER_WORKER_H__
#define __MASTER_WORKER_H__

#include <network/network.hh>
#include <optimization/optimization.hh>

#include "Job/job.hh"

namespace optimaster
{
	class Worker : public network::Client
	{
		public:
			struct SuccessArgs
			{
				Job job;
				optimization::Solution solution;
				optimization::messages::worker::Response response;

				std::map<std::string, double> fitness;
			};
			
			/* Constructor/destructor */
			Worker();
			Worker(network::AddressInfo &info);
		
			/* Public functions */
			void sendJob(Job &job, optimization::Solution &solution);
			
			Job &job();
			optimization::Solution &solution();
			
			void failed(optimization::messages::worker::Response::Failure::Type type, std::string const &message = "");
			
			base::signals::Signal<optimization::messages::worker::Response::Failure> &onFailed();
			base::signals::Signal<SuccessArgs> &onSuccess();
			base::signals::Signal<std::string> &onChallenge();
		private:
			/* Private functions */
			struct Data : public base::Object::PrivateData
			{
				/* Instance data */
				base::signals::Signal<optimization::messages::worker::Response::Failure> onFailed;
				base::signals::Signal<SuccessArgs> onSuccess;
				base::signals::Signal<std::string> onChallenge;

				sigc::connection timeout;
			
				SuccessArgs args;
				
				bool onData(os::FileDescriptor::DataArgs &args);
				bool onTimeout();
			};

			Data *d_data;		
	};
	
	inline Job &Worker::job()
	{
		return d_data->args.job;
	}
	
	inline optimization::Solution &Worker::solution()
	{
		return d_data->args.solution;
	}
	
	inline base::signals::Signal<optimization::messages::worker::Response::Failure> &Worker::onFailed()
	{
		return d_data->onFailed;
	}
	
	inline base::signals::Signal<Worker::SuccessArgs> &Worker::onSuccess()
	{
		return d_data->onSuccess;
	}
	
	inline base::signals::Signal<std::string> &Worker::onChallenge()
	{
		return d_data->onChallenge;
	}
}

#endif /* __MASTER_WORKER_H__ */
