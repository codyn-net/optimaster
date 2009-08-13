#ifndef __MASTER_JOB_QUEUE_H__
#define __MASTER_JOB_QUEUE_H__

#include <base/base.hh>

#include "Item/item.hh"
#include "Job/job.hh"

#include <list>

namespace optimaster
{
	class JobQueue : public base::Object
	{
		class Item : public base::Object
		{
			struct Data : public base::Object::PrivateData
			{
				Job job;
				base::Cloneable<optimization::Solution> solution;
				base::Cloneable<Item> nextInSequence;

				double overtake;
			};
			
			Data *d_data;
			
			public:
				Item(Job &job, optimization::Solution &solution);

				double overtake() const;

				bool canOvertake(Item const &other) const;
				void overtake(Item &other);
				void setOvertake(double overtake);

				double priority() const;
				void sequence(Item &other);
				
				Job &job();
				Job const &job() const;
				optimization::Solution &solution();
				
				Item *clone() const;
			private:
				void sequenceDecrease(double overtake);
		};

		struct Data : public base::Object::PrivateData
		{
			/* Instance data */
			std::list<Item> queue;
			std::map<std::string, size_t> solutionsPerJob;
			
			double prioritySum;
			
			base::signals::Signal<> onJobAdded;
		};

		Data *d_data;

		public:
			/* Constructor/destructor */
			JobQueue();
		
			/* Public functions */
			void queue(Job &job, optimization::Solution &solution);
			void queue(Job &job);
			
			bool pop(Job &job, optimization::Solution &solution);
			
			bool jobDone(Job &job) const;
			bool hasJobs() const;
			
			size_t inQueue(Job &job) const;
			void remove(Job const &job);
			
			base::signals::Signal<> &onJobAdded();
		private:
			/* Private functions */
			bool insert(Item &item);
			void addSolutionsPerJob(Job &job, size_t num);
			void updatePrioritySum(double num);
			
			bool matchItem(Item const &item, Job const &job);
	};

	inline double JobQueue::Item::priority() const
	{
		return d_data->job.priority();
	}
	
	inline double JobQueue::Item::overtake() const
	{
		return d_data->overtake;
	}
	
	inline void JobQueue::Item::setOvertake(double overtake)
	{
		d_data->overtake = overtake;
	}
	
	inline Job &JobQueue::Item::job()
	{
		return d_data->job;
	}

	inline Job const &JobQueue::Item::job() const
	{
		return d_data->job;
	}
	
	inline optimization::Solution &JobQueue::Item::solution()
	{
		return d_data->solution;
	}
	
	inline base::signals::Signal<> &JobQueue::onJobAdded()
	{
		return d_data->onJobAdded;
	}
	
	inline bool JobQueue::hasJobs() const
	{
		return d_data->queue.size() != 0;
	}
}

#endif /* __MASTER_JOB_QUEUE_H__ */
