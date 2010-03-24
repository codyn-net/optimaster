/*
 * application.cc
 * This file is part of optimaster
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
 *
 * optimaster is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * optimaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with optimaster; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#include "application.hh"
#include "config.hh"
#include "debug.hh"

#include <jessevdk/os/os.hh>
#include <sstream>
#include <jessevdk/network/network.hh>
#include <vector>
#include <syslog.h>

using namespace std;
using namespace optimaster;
using namespace jessevdk::base;
using namespace jessevdk::os;
using namespace jessevdk::network;
using namespace optimization::messages;

/**
 * @class optimaster::Application
 * @brief Main application class
 */

/**
 * @brief Create new application.
 * @param argc number of command line arguments
 * @param argv command line arguments
 *
 * Constructor.
 *
 * Create a new application object.
 *
 */
Application::Application(int    &argc,
                         char **&argv)
:
	d_tasksFailed(0),
	d_tasksSuccess(0)
{
	EnableEnvironment();

	d_discovery.OnGreeting().Add(*this, &Application::OnGreeting);

	d_workerManager.OnAdded.Add(*this, &Application::OnWorkerAdded);
	d_workerManager.OnRemoved.Add(*this, &Application::OnWorkerRemoved);
	d_workerManager.OnNotifyAvailable.Add(*this, &Application::OnNotifyAvailable);

	ParseArguments(argc, argv);

	d_optimizerManager.OnAdded.Add(*this, &Application::OnOptimizerAdded);
	d_optimizerManager.OnRemoved.Add(*this, &Application::OnOptimizerRemoved);

	d_taskQueue.OnNotifyAvailable.Add(*this, &Application::OnNotifyAvailable);

	d_discovery.Listen();

	Glib::signal_timeout().connect_seconds(sigc::mem_fun(*this, &Application::OnPeriodicLogStatus),
	                                       1800);

	if (!d_optimizerManager.Listen())
	{
		cerr << "Failed to start master, could not listen for optimizers (maybe there is another master running?)" << endl;
	}
	else
	{
		openlog("optimaster", 0, LOG_USER);
	}
}

/**
 * @brief Application destructor.
 *
 * Destructor.
 *
 * Application destructor.
 *
 */
Application::~Application()
{
	d_optimizerManager.OnAdded.Remove(*this, &Application::OnOptimizerAdded);
	d_optimizerManager.OnRemoved.Remove(*this, &Application::OnOptimizerRemoved);

	d_workerManager.OnAdded.Remove(*this, &Application::OnWorkerAdded);
	d_workerManager.OnRemoved.Remove(*this, &Application::OnWorkerRemoved);
	d_workerManager.OnNotifyAvailable.Remove(*this, &Application::OnNotifyAvailable);

	d_taskQueue.OnNotifyAvailable.Remove(*this, &Application::OnNotifyAvailable);

	d_discovery.OnGreeting().Remove(*this, &Application::OnGreeting);
	closelog();
}

/**
 * @brief Parse command line arguments.
 * @param argc number of command line arguments
 * @param argv command line arguments
 *
 * Parses command line arguments.
 *
 */
void 
Application::ParseArguments(int    &argc, 
                            char **&argv) 
{
	// Parse config file first
	optimaster::Config &config = optimaster::Config::Initialize(CONFDIR "/optimaster.conf");

	Glib::OptionGroup group("Master", "Optimization Master");
	Glib::OptionEntry discovery;

	discovery.set_long_name("discovery");
	discovery.set_short_name('d');
	discovery.set_description("Discovery address/namespace");

	stringstream s;
	s << optimization::Constants::DiscoveryGroup << ":" << optimization::Constants::DiscoveryPort;

	Glib::ustring disc = s.str();
	group.add_entry(discovery, disc);

	Glib::OptionEntry listen;

	listen.set_long_name("listen");
	listen.set_short_name('l');
	listen.set_description("Listen address");

	group.add_entry(listen, config.ListenAddress);

	Glib::OptionContext context;

	context.set_main_group(group);
	context.parse(argc, argv);

	vector<string> parts = String(disc).Split(":", 2);
	string host = parts[0];
	string port = parts.size() == 2 ? parts[1] : "";

	if (AddressInfo::IsIPAddress(host))
	{
		if (port != "")
		{
			config.DiscoveryAddress = host + ":" + port;
		}
		else
		{
			s.str("");
			s << host << ":" << optimization::Constants::DiscoveryPort;

			config.DiscoveryAddress = s.str();
		}
	}
	else
	{
		if (port != "")
		{
			config.DiscoveryAddress = optimization::Constants::DiscoveryGroup + ":" + port;
		}

		config.DiscoveryNamespace = host;
	}

	parts = String(config.DiscoveryAddress).Split(":", 2);

	if (parts[0] == "")
	{
		parts[0] = optimization::Constants::DiscoveryGroup;
	}

	d_discovery.Set(parts[0], parts[1]);
	d_discovery.SetNamespace(config.DiscoveryNamespace);

	parts = String(config.ListenAddress).Split(":", 2);

	if (parts.size() == 1 || parts[1] == "")
	{
		if (parts.size() == 1)
		{
			parts.push_back("");
		}

		stringstream s;
		s << optimization::Constants::MasterPort;

		parts[1] = s.str();
	}

	d_optimizerManager.Set(parts[0], parts[1]);
}

struct DebugInfo
{
	char const *name;
	int flag;
};

/**
 * @brief Enable debug levels using environment variables.
 *
 * Sets debug levels based on environment variables.
 *
 */
void
Application::EnableEnvironment()
{
	static DebugInfo infos[] = {
		{"DEBUG_BASE", jessevdk::base::Debug::Domain::Base},
		{"DEBUG_NETWORK", jessevdk::base::Debug::Domain::Network},
		{"DEBUG_DATA", jessevdk::base::Debug::Domain::Data},
		{"DEBUG_OS", jessevdk::base::Debug::Domain::OS},
		{"DEBUG_WORKER", optimization::Debug::Domain::Worker},
		{"DEBUG_MASTER", optimization::Debug::Domain::Master},
		{"DEBUG_SCHEDULER", optimaster::Debug::Domain::Scheduler}
	};

	for (size_t i = 0; i < sizeof(infos) / sizeof(DebugInfo); ++i)
	{
		if (Environment::Variable(infos[i].name))
		{
			Debug::Enable(infos[i].flag);
		}
	}
}

/**
 * @brief Greeting message callback.
 * @param info discovery info
 *
 * Called when a discovery greeting has been received. The worker who send it
 * will be registered in the worker manager.
 *
 */
void
Application::OnGreeting(optimization::Discovery::Info &info) 
{
	string protocol;
	string host = info.Host;
	string port;

	AddressInfo::Split(info.Connection, protocol, host, port);

	if (host == "" || host == "0.0.0.0")
	{
		host = info.Host;
	}

	d_workerManager.Add(protocol + "://" + host + ":" + port);
}

/**
 * @brief Keyboard interrupt callback.
 * @param loop the glib main loop
 *
 * Called on keyboard interrupt.
 *
 * @return: true
 *
 */
bool
Application::OnInterrupt(Glib::RefPtr<Glib::MainLoop> loop) 
{
	loop->quit();
	return true;
}

/**
 * @brief Optimizer added callback.
 * @param optimizer the optimizer
 *
 * Called when a new optimizer is connected.
 *
 */
void
Application::OnOptimizerAdded(Optimizer &optimizer)
{
	syslog(LOG_NOTICE,
	       "job-connected: %lu, %s",
	       optimizer.Id(),
	       optimizer.Client().Address().Host(true).c_str());

	optimizer.OnCommunication().Add(*this, &Application::OnOptimizerCommunication);
}

/**
 * @brief Optimizer removed callback.
 * @param optimizer the optimizer
 *
 * Called when an optimizer is disconnected.
 *
 */
void
Application::OnOptimizerRemoved(Optimizer &optimizer)
{
	debug_master << "Job disconnected: " << optimizer.Id() << endl;

	syslog(LOG_NOTICE, "job-disconnected: %lu", optimizer.Id());
	optimizer.OnCommunication().Remove(*this, &Application::OnOptimizerCommunication);

	// Remove all tasks from the task queue for this optimizer
	d_taskQueue.Remove(optimizer.Id());

	// Cancel all the workers that are still active for this optimizer
	vector<Worker> activeWorkers = optimizer.ActiveWorkers();
	vector<Worker>::iterator iter;
	
	for (iter = activeWorkers.begin(); iter != activeWorkers.end(); ++iter)
	{
		if (!iter->Cancel())
		{
			iter->Deactivate();
		}
	}

	LogStatus();
}

/**
 * @brief Handle optimizer batch.
 * @param optimizer the optimizer
 * @param batch the batch
 *
 * Handles a batch message from the optimizer.
 *
 */
void
Application::HandleOptimizerBatch(Optimizer                 &optimizer,
                                  task::Communication const &communication)
{
	// Add batch to the task queue
	debug_master << "Received batch from optimizer ("
	             << optimizer.Id() << "): "
	             << communication.batch().tasks_size() << endl;

	syslog(LOG_NOTICE,
	       "job-batch: %lu, %d, %.3f",
	       optimizer.Id(),
	       communication.batch().tasks_size(),
	       communication.batch().priority());

	d_taskQueue.Push(optimizer.Id(), optimizer.AverageRunTime(), communication.batch());
}

/**
 * @brief Handle optimizer token.
 * @param optimizer the optimizer
 * @param token the token
 *
 * Handles a token message from the optimizer.
 *
 */
void
Application::HandleOptimizerToken(Optimizer                 &optimizer,
                                  task::Communication const &communication)
{
	// Relay the token response back to the worker
	vector<Worker>::iterator iter;
	
	for (iter = optimizer.ActiveWorkers().begin();
	     iter != optimizer.ActiveWorkers().end();
	     ++iter)
	{
		if (iter->ActiveTask().Id() == communication.token().id())
		{
			debug_master << "Relaying token message to worker" << endl;
			iter->Send(communication);
			break;
		}
	}
}

/**
 * @brief Optmizer communication callback.
 * @param args communication arguments
 *
 * Called when a communication message from an optimizer has been received.
 *
 */
void
Application::OnOptimizerCommunication(Optimizer::CommunicationArgs &args)
{
	Optimizer optimizer;
	
	if (!d_optimizerManager.Find(args.Source.Id(), optimizer))
	{
		return;
	}

	switch (args.Communication.type())
	{
		case task::Communication::CommunicationBatch:
			HandleOptimizerBatch(optimizer, args.Communication);
		break;
		case task::Communication::CommunicationToken:
			HandleOptimizerToken(optimizer, args.Communication);
		break;
	}
}

/**
 * @brief Worker added callback.
 * @param worker the worker
 *
 * Called when a worker has been added.
 *
 */
void
Application::OnWorkerAdded(Worker &worker)
{
	worker.OnCommunication().Add(*this, &Application::OnWorkerCommunication);
	worker.OnTimeout().Add(*this, &Application::OnWorkerTimeout);
}

/**
 * @brief Worker removed callback.
 * @param worker the worker
 *
 * Called when a worker has been removed.
 *
 */
void
Application::OnWorkerRemoved(Worker &worker)
{
	worker.OnCommunication().Remove(*this, &Application::OnWorkerCommunication);
	worker.OnTimeout().Remove(*this, &Application::OnWorkerTimeout);

	// Reschedule the task the worker was working on
	if (worker.Active())
	{
		d_taskQueue.Push(worker.ActiveTask());
	}

	LogStatus();
}

/**
 * @brief Worker communication message processing.
 * @param args the CommunicationArgs
 *
 * Process communication messages from the worker.
 *
 */
void
Application::OnWorkerCommunication(Communicator::CommunicationArgs &args)
{
	Worker worker;

	if (!d_workerManager.Find(args.Source.Id(), worker))
	{
		debug_worker << "Could not find worker: " << worker.Id() << endl;
		return;
	}

	Task &task = worker.ActiveTask();
	task::Response &response = *args.Communication.mutable_response();
	Optimizer optimizer;

	// Check if the optimizer for this task is still present
	if (!d_optimizerManager.Find(task.Group(), optimizer))
	{
		debug_worker << "Optimizer no longer connected..." << endl;
		worker.Deactivate();
		return;
	}

	switch (response.status())
	{
		// Task was completed successfully
		case task::Response::Success:
			if (Debug::Enabled(optimization::Debug::Domain::Worker))
			{
				debug_worker << "Worker success ("
				             << worker.Client().Address().Host(true)
				             << ":"
				             << worker.Client().Address().Port(true)
				             << ") for (" << task.Group()
				             << ", " << task.Message().id() << ")"
				             << endl;
			}

			optimizer.Send(args.Communication);
			worker.Deactivate();

			d_taskQueue.Finished(task);

			++d_tasksSuccess;
		break;
		// Task execution failed
		case task::Response::Failed:
			task.Failed();

			++d_tasksFailed;

			if (task.Failures() >= Config::Instance().MaxTaskFailures)
			{
				// Task failed too many times
				if (Debug::Enabled(optimization::Debug::Domain::Worker))
				{
					debug_worker << "Task failed to many times ("
					             << worker.Client().Address().Host(true)
					             << ":"
					             << worker.Client().Address().Port(true)
					             << ") for (" << task.Group() << ", "
					             << task.Message().id() << ")" << endl;
				}

				syslog(LOG_ERR,
				       "task-failed: %lu, %u, task failed too many times",
				       task.Group(),
				       task.Message().id());

				// Relay failure to optimizer
				optimizer.Send(args.Communication);
				worker.Deactivate();

				d_taskQueue.Finished(task);
			}
			else
			{
				// Reschedule the task because the failure might be due to a
				// faulty worker, or maybe some network problems
				if (Debug::Enabled(optimization::Debug::Domain::Worker))
				{
					debug_worker << "Task failed (" << FailureToString(response.failure()) << "), rescheduling ("
					             << worker.Client().Address().Host(true) <<
					             ":" << worker.Client().Address().Port(true)
					             << ") for (" << task.Group() << ", "
					             << task.Message().id() << ")" << endl;
				}

				syslog(LOG_ERR,
				       "task-failed: %lu, %u, task failed and rescheduling: %s",
				       task.Group(),
				       task.Message().id(),
				       FailureToString(response.failure()).c_str());

				// Reschedule task
				d_taskQueue.Push(task);
				worker.Deactivate();
			}
		break;
		case task::Response::Challenge:
			// Simply relay to optimizer
			if (Debug::Enabled(optimization::Debug::Domain::Worker))
			{
				debug_worker << "Worker challenge ("
				             << worker.Client().Address().Host(true)
				             << ":"
				             << worker.Client().Address().Port(true)
				             << ") for ("
				             << task.Group() << ", "
				             << task.Message().id() << ")" << endl;
			}

			optimizer.Send(args.Communication);
		break;
		default:
		break;
	}
}

/**
 * @brief Dispatch tasks in an idle function.
 *
 * Add a glib idle function for dispatching tasks. This is useful if you
 * want to collect multiple dispatches in one call.
 *
 */
void
Application::IdleDispatch()
{
	if (!d_idleDispatch)
	{
		d_idleDispatch = Glib::signal_idle().connect(sigc::mem_fun(*this,
		                                             &Application::OnDispatch));
	}
}

/**
 * @brief Dispatch available tasks to available workers.
 *
 * Dispatches tasks to workers as long as there are idle workers and tasks
 * in the task queue.
 *
 * @return: false
 *
 */
bool
Application::OnDispatch()
{
	Task task;
	Worker worker;

	// As long as there are idle workers and tasks
	while (d_workerManager.Idle(worker) && d_taskQueue.Pop(task))
	{
		// Activate the worker with the task
		Batch batch;
		d_taskQueue.Lookup(task.Group(), batch);

		if (!worker.Activate(task, batch.Timeout()))
		{
			d_taskQueue.Push(task);
			continue;
		}

		Optimizer optimizer;
		if (d_optimizerManager.Find(task.Group(), optimizer))
		{
			optimizer.Add(worker);
		}

		debug_master << "Worker activated for: " << task.Group() << " (" << task.Id() << ")" << endl;
	}

	LogStatus();

	d_idleDispatch.disconnect();
	return false;
}

/**
 * @brief Main application loop.
 * @param loop the glib main loop to use
 *
 * Runs the main application loop.
 *
 */
void
Application::Run(Glib::RefPtr<Glib::MainLoop> loop) 
{
	if (!d_optimizerManager)
	{
		return;
	}
	
	jessevdk::os::Signals::Install();
	jessevdk::os::Signals::OnInterrupt.AddData(*this, &Application::OnInterrupt, loop);

	if (d_discovery.IsMulticast())
	{
		// Send discovery wakeup message if we are in multicast mode
		SendWakeup();
	}

	d_mainLoop = loop;
	loop->run();
}

/**
 * @brief Send discovery wakeup message.
 *
 * Send a discovery wakeup message. The wakup message is used to ask any
 * workers to notify themselves to the master process. All workers receiving
 * the wakup will consequently send a 'greeting' message back.
 *
 */
void
Application::SendWakeup()
{
	// Create new multicast client
	Client client = Client::Multicast(optimization::Constants::DiscoveryGroup, optimization::Constants::DiscoveryPort, 2);

	// Make wakeup discovery message with certain namespace
	optimization::messages::discovery::Discovery disc;

	disc.set_type(optimization::messages::discovery::Discovery::TypeWakeup);
	disc.set_namespace_(d_discovery.Namespace());

	optimization::messages::discovery::Wakeup *wakeup = disc.mutable_wakeup();

	// Set the connection
	wakeup->set_connection(d_discovery.Connection());
	string serialized;

	if (optimization::Messages::Create(disc, serialized))
	{
		client.Write(serialized);
	}
}

/**
 * @brief Notfy available callback.
 *
 * Called when there are idle workers available or when there are new tasks
 * to be processed.
 *
 */
void
Application::OnNotifyAvailable()
{
	// Start idle dispatch
	IdleDispatch();
}

void
Application::OnWorkerTimeout(Worker &worker)
{
	// Reschedule and cancel
	Task task = worker.ActiveTask();
	worker.Cancel();
	
	d_taskQueue.Push(task);
}

string
Application::FailureToString(task::Response::Failure const &failure) const
{
	string type;

	switch (failure.type())
	{
		case task::Response::Failure::Disconnected:
			type = "Disconnected";
		break;
		case task::Response::Failure::Dispatcher:
			type = "Dispatcher failure";
		break;
		case task::Response::Failure::DispatcherNotFound:
			type = "Dispatcher not found";
		break;
		case task::Response::Failure::NoResponse:
			type = "No response";
		break;
		case task::Response::Failure::Timeout:
			type = "Timeout";
		break;
		case task::Response::Failure::WrongRequest:
			type = "Wrong request";
		break;
		default:
			type = "Unknown";
		break;
	}

	if (failure.has_message())
	{
		return type + ": " + failure.message();
	}
	else
	{
		return type;
	}
}

void
Application::LogStatus() const
{
	syslog(LOG_NOTICE, "workers-status: %lu/%lu", d_workerManager.Active(), d_workerManager.Size());
	syslog(LOG_NOTICE, "queue-status: %lu", d_taskQueue.Size());
}

bool
Application::OnPeriodicLogStatus()
{
	LogStatus();

	syslog(LOG_NOTICE, "tasks-status: %lu/%lu", d_tasksFailed, d_tasksSuccess);

	d_tasksFailed = 0;
	d_tasksSuccess = 0;
}
