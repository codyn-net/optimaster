#ifndef __MASTER_TASK_QUEUE_H__
#define __MASTER_TASK_QUEUE_H__

#include <base/base.hh>
#include "Task/task.hh"
#include <list>

namespace optimaster
{
	class TaskQueue : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::list<taskqueue::Task> queue;
			double prioritySum;
		};

		Data *d_data;

		public:
			/* Constructor/destructor */
			TaskQueue();
		
			/* Public functions */
			void queue(size_t id, optimization::messages::task::Batch &tasks);
			void queue(taskqueue::Task &task);

			bool pop(taskqueue::Task &item);
			
			bool empty() const;
			
			void remove(size_t id);
		private:
			/* Private functions */
			bool insert(taskqueue::Task &item);
			void updatePrioritySum(double num);
	};
}

#endif /* __MASTER_TASK_QUEUE_H__ */
