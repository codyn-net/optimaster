#ifndef __MASTER_TASK_QUEUE_TASK_H__
#define __MASTER_TAKS_QUEUE_TASK_H__

#include <optimization/Messages/messages.hh>

namespace optimaster
{
namespace taskqueue
{
	class Task : public base::Object
	{
		struct Data : public base::Object::PrivateData
		{
			optimization::messages::task::Task task;
			base::Cloneable<Task> nextInSequence;

			size_t id;
			double priority;
			double overtake;
		};
		
		Data *d_data;
		
		public:
			Task();
			Task(size_t idx, double priority, optimization::messages::task::Task const &task);

			size_t id() const;
			double priority() const;
			double overtake() const;

			bool canOvertake(Task const &other) const;
			void overtake(Task &other);
			void setOvertake(double overtake);

			void sequence(Task &other);
			
			optimization::messages::task::Task &task();
			optimization::messages::task::Task const &task() const;
			
			
			Task *clone() const;
		private:
			void sequenceDecrease(double overtake);
	};
	
	inline optimization::messages::task::Task &Task::task()
	{
		return d_data->task;
	}
	
	inline optimization::messages::task::Task const &Task::task() const
	{
		return d_data->task;
	}
	
	inline double Task::overtake() const
	{
		return d_data->overtake;
	}
	
	inline void Task::setOvertake(double overtake)
	{
		d_data->overtake = overtake;
	}
	
	inline size_t Task::id() const
	{
		return d_data->id;
	}
}
}

#endif /* __MASTER_TASK_QUEUE_TASK_H__ */

