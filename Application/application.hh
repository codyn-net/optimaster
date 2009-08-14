#ifndef __MASTER_APPLICATION_H__
#define __MASTER_APPLICATION_H__

#include <optimization/Discovery/discovery.hh>
#include "Command/command.hh"

#include "Managers/Optimizer/optimizer.hh"

#include "WorkerPool/workerpool.hh"
#include "Job/job.hh"
#include "JobQueue/jobqueue.hh"

#include <glibmm.h>
#include <libxml++/libxml++.h>
#include <exception>

namespace optimaster
{
	class Application
	{
		optimization::Discovery d_discovery;

		managers::Optimizer d_optimizerManager;
		managers::Optimizer::Extension d_optimizerExtensionManager;
		
		WorkerPool d_workerPool;
		Command d_command;
		
		std::map<std::string, Job> d_jobs;
		JobQueue d_jobQueue;
		sigc::connection d_idleWork;
		
		bool d_exitAtLast;
		Glib::RefPtr<Glib::MainLoop> d_mainLoop;
		
		public:
			class InvalidJob : public std::exception
			{
				std::string d_message;

				public:
					InvalidJob(std::string const &message);
					~InvalidJob() throw () {};
				
					virtual char const *what() const throw ();
			};

			/* Constructor/destructor */
			Application(int &argc, char **&argv);

			/* Public functions */		
			void run(Glib::RefPtr<Glib::MainLoop> loop);
			
			void addJobFromXml(xmlpp::DomParser &parser, size_t priority, Job &job, std::string const &user = "", std::string const &chain = "");
			void addJobFromXml(std::string const &filename, size_t priority, Job &job, std::string const &user = "", std::string const &chain = "");
			void addJobFromXmlFile(std::string const &filename, size_t priority, Job &job, std::string const &user = "", std::string const &chain = "");
			
			std::vector<Job> jobs() const;
			bool job(std::string const &name, Job &job) const;
			
			bool removeJob(std::string const &name);
			bool removeJob(Job &job);

			std::string dataDirectory() const;
		private:
			/* Private functions */
			void parseArguments(int &argc, char **&argv);
			bool onGreeting(optimization::Discovery::Info &info);
			
			void registerWorker(std::string const &connection);
			bool onInterrupt(Glib::RefPtr<Glib::MainLoop> loop);
			
			void settingsFromXml(base::Properties &properties, xmlpp::Element *element);
			void extensionsFromXml(optimization::Optimizer &optimizer, xmlpp::Element *child);
			void parseBoundaries(optimization::Boundaries &boundaries, xmlpp::Element *child);
			void parseParameters(optimization::Parameters &parameters, optimization::Boundaries &boundaries, xmlpp::Element *child);
			
			void addJob(Job &job);
			
			bool jobByToken(std::string const &token, Job &job) const;
			
			void onIdleWork();
			bool onDispatch();
			
			void onWorkerRemoved(Worker &worker);
			void onWorkerFailed(optimization::messages::worker::Response::Failure &failure, Worker worker);
			void onWorkerSuccess(Worker::SuccessArgs &args);
			void onWorkerChallenge(std::string &challenge, Worker worker);
			
			void newIteration(Job &job);
			
			template <typename TParent, typename TManager>
			void extensionsFromXml(TManager &manager, TParent &parent, xmlpp::Element *child);
			
			void sendWakeup();
			void enableEnvironment();
			bool getFirstChild(xmlpp::Node *node, std::string const &name, xmlpp::Element *&child);
			
			std::string hashToken(Job const &job, std::string const &challenge);
			
			std::string failureError(optimization::messages::worker::Response::Failure const &failure) const;
			std::string failureMessage(optimization::messages::worker::Response::Failure const &failure) const;
	};
	
	template <typename TParent, typename TManager>
	void Application::extensionsFromXml(TManager &manager, TParent &parent, xmlpp::Element *child)
	{
		xmlpp::Node::NodeList extensions = child->get_children("extension");
		xmlpp::Node::NodeList::iterator iter;
	
		for (iter = extensions.begin(); iter != extensions.end(); ++iter)
		{
			xmlpp::Element *extension;
		
			try
			{
				extension = dynamic_cast<xmlpp::Element *>(*iter);
			}
			catch (std::bad_cast)
			{
				continue;
			}
		
			xmlpp::Attribute *attr = extension->get_attribute("name");
		
			if (!attr)
				continue;
		
			std::string name = attr->get_value();
		
			base::Cloneable<typename TManager::info_type::type> ext;
			ext = manager.lookup(attr->get_value());
	
			if (!ext)
			{
				throw InvalidJob("Extension `" + name + "' not found");
			}
		
			settingsFromXml(ext->settings(), extension);		
			parent->addExtension(ext);
		}
	}
}

#endif /* __MASTER_APPLICATION_H__ */
