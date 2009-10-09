#ifndef __MASTER_OPTIMIZER_MANAGER_H__
#define __MASTER_OPTIMIZER_MANAGER_H__

#include <network/network.hh>
#include "Optimizer/optimizer.hh"

namespace optimaster
{
	class OptimizerManager
	{
		std::map<int, Optimizer> d_optimizers;
		network::TcpServer d_server;

		public:
			struct BatchArgs
			{
				Optimizer optimizer;
				optimization::messages::task::Batch batch;
			};

			/* Constructor/destructor */
			OptimizerManager();
			~OptimizerManager();
		
			/* Public functions */
			void set(std::string const &host, std::string const &port);
			void listen();
			
			bool find(size_t id, Optimizer &optimizer);
			
			base::signals::Signal<BatchArgs> onBatch;
			base::signals::Signal<Optimizer> onOptimizerClosed;
		private:
			/* Private functions */
			void onNewConnection(network::Client &client);
			void onConnectionClosed(int fd);
			
			bool fromDescriptor(int fd, Optimizer &optimizer);
			bool onOptimizerBatch(optimization::messages::task::Batch &batch, int fd);
	};
}

#endif /* __MASTER_OPTIMIZER_MANAGER_H__ */
