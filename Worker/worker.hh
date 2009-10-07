#ifndef __MASTER_WORKER_H__
#define __MASTER_WORKER_H__

#include <network/network.hh>
#include <optimization/optimization.hh>

#include "TaskQueue/taskqueue.hh"

namespace optimaster
{
	class Worker : public network::Client
	{
		public:
			/* Constructor/destructor */
			Worker();
			Worker(network::AddressInfo &info);
		
			/* Public functions */
			void send(taskqueue::Task &task);
			
			taskqueue::Task const &task() const;
			taskqueue::Task &task();
			
			bool working() const;

			base::signals::Signal<optimization::messages::task::Response> &onResponse();
			base::signals::Signal<std::string> &onChallenge();
		private:
			/* Private functions */
			struct Data : public base::Object::PrivateData
			{
				/* Instance data */
				base::signals::Signal<optimization::messages::task::Response> onResponse;
				base::signals::Signal<std::string> onChallenge;
			
				bool working;
				optimization::messages::task::Response response;
				taskqueue::Task task;
				
				bool onData(os::FileDescriptor::DataArgs &args);
				void setFailure(optimization::messages::task::Response::Failure::Type type, std::string const &message = "");
			};

			Data *d_data;		
	};
	
	inline taskqueue::Task &Worker::task()
	{
		return d_data->task;
	}
	
	inline taskqueue::Task const &Worker::task() const
	{
		return d_data->task;
	}
	
	inline base::signals::Signal<optimization::messages::task::Response> &Worker::onResponse()
	{
		return d_data->onResponse;
	}
	
	inline base::signals::Signal<std::string> &Worker::onChallenge()
	{
		return d_data->onChallenge;
	}
	
	inline bool Worker::working() const
	{
		return d_data->working;
	}
}

#endif /* __MASTER_WORKER_H__ */
