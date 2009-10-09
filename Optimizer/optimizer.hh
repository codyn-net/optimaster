#ifndef __MASTER_OPTIMIZER_H__
#define __MASTER_OPTIMIZER_H__

#include <network/network.hh>
#include <optimization/Messages/messages.hh>

namespace optimaster
{
	class Optimizer : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			network::Client client;
			
			size_t id;

			base::signals::Signal<optimization::messages::task::Batch> onBatch;
			void onClientData(os::FileDescriptor::DataArgs args);
		};
		
		Data *d_data;
		
		public:
			/* Constructor/destructor */
			Optimizer();
			Optimizer(network::Client &client);
			
			network::Client &client();
		
			/* Public functions */
			size_t id();
			
			bool operator==(size_t id) const;
			void response(optimization::messages::task::Response &response);
			
			base::signals::Signal<optimization::messages::task::Batch> &onBatch();
		private:
			/* Private functions */
	};
}

#endif /* __MASTER_OPTIMIZER_H__ */
