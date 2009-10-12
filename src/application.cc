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

#include <os/os.hh>
#include <sstream>
#include <network/network.hh>
#include <os/os.hh>
#include <vector>

using namespace std;
using namespace optimaster;
using namespace base;
using namespace os;
using namespace network;
using namespace optimization::messages;

/** \brief Create new application.
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
{
	EnableEnvironment();

	d_discovery.onGreeting().add(*this, &Application::OnGreeting);

	d_workerManager.OnAdded.add(*this, &Application::OnWorkerAdded);
	d_workerManager.OnRemoved.add(*this, &Application::OnWorkerRemoved);
	d_workerManager.OnNotifyAvailable.add(*this, &Application::OnNotifyAvailable);

	ParseArguments(argc, argv);

	d_optimizerManager.OnAdded.add(*this, &Application::OnOptimizerAdded);
	d_optimizerManager.OnRemoved.add(*this, &Application::OnOptimizerRemoved);

	d_taskQueue.OnNotifyAvailable.add(*this, &Application::OnNotifyAvailable);

	d_discovery.listen();
	d_optimizerManager.Listen();
}

/** \brief Application destructor.
 *
 * Destructor.
 *
 * Application destructor.
 *
 */
Application::~Application()
{
	d_optimizerManager.OnAdded.remove(*this, &Application::OnOptimizerAdded);
	d_optimizerManager.OnRemoved.remove(*this, &Application::OnOptimizerRemoved);

	d_workerManager.OnAdded.remove(*this, &Application::OnWorkerAdded);
	d_workerManager.OnRemoved.remove(*this, &Application::OnWorkerRemoved);
	d_workerManager.OnNotifyAvailable.remove(*this, &Application::OnNotifyAvailable);

	d_taskQueue.OnNotifyAvailable.remove(*this, &Application::OnNotifyAvailable);

	d_discovery.onGreeting().remove(*this, &Application::OnGreeting);
}

/** \brief Parse command line arguments.
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

	vector<string> parts = String(disc).split(":", 2);
	string host = parts[0];
	string port = parts.size() == 2 ? parts[1] : "";

	if (AddressInfo::isIPAddress(host))
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

	parts = String(config.DiscoveryAddress).split(":", 2);

	if (parts[0] == "")
	{
		parts[0] = optimization::Constants::DiscoveryGroup;
	}

	d_discovery.set(parts[0], parts[1]);
	d_discovery.setNs(config.DiscoveryNamespace);

	parts = String(config.ListenAddress).split(":", 2);

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

/** \brief Enable debug levels using environment variables.
 *
 * Sets debug levels based on environment variables.
 *
 */
void
Application::EnableEnvironment()
{
	static DebugInfo infos[] = {
		{"DEBUG_BASE", Debug::Domain::Base},
		{"DEBUG_NETWORK", Debug::Domain::Network},
		{"DEBUG_DATA", Debug::Domain::Data},
		{"DEBUG_OS", Debug::Domain::OS},
		{"DEBUG_OPTIMIZATION", Debug::Domain::Optimization},
		{"DEBUG_MODULES", Debug::Domain::Modules},
		{"DEBUG_WORKER", Debug::Domain::Worker},
		{"DEBUG_MASTER", Debug::Domain::Master},
	};

	for (size_t i = 0; i < sizeof(infos) / sizeof(DebugInfo); ++i)
	{
		if (Environment::variable(infos[i].name))
		{
			Debug::enable(infos[i].flag);
		}
	}
}

/** \brief Greeting message callback.
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
	string host = info.host;
	string port;

	AddressInfo::Split(info.connection, protocol, host, port);

	if (host == "" || host == "0.0.0.0")
	{
		host = info.host;
	}

	d_workerManager.Add(protocol + "://" + host + ":" + port);
}

/** \brief Keyboard interrupt callback.
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

/** \brief Optimizer added callback.
 * @param optimizer the optimizer
 *
 * Called when a new optimizer is connected.
 *
 */
void
Application::OnOptimizerAdded(Optimizer &optimizer)
{
	optimizer.OnCommunication().add(*this, &Application::OnOptimizerCommunication);
}

/** \brief Optimizer removed callback.
 * @param optimizer the optimizer
 *
 * Called when an optimizer is disconnected.
 *
 */
void
Application::OnOptimizerRemoved(Optimizer &optimizer)
{
	debug_master << "Optimizer disconnected: " << optimizer.Id() << endl;

	optimizer.OnCommunication().remove(*this, &Application::OnOptimizerCommunication);

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
}

/** \brief Handle optimizer batch.
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
	d_taskQueue.Queue(optimizer.Id(), communication.batch());
}

/** \brief Handle optimizer token.
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
	
	for (iter = optimizer.ActiveWorkers().begin(); iter != optimizer.ActiveWorkers().end(); ++iter)
	{
		if (iter->ActiveTask().Id() == communication.token().id())
		{
			iter->Send(communication);
			break;
		}
	}
}

/** \brief Optmizer communication callback.
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

/** \brief Worker added callback.
 * @param worker the worker
 *
 * Called when a worker has been added.
 *
 */
void
Application::OnWorkerAdded(Worker &worker)
{
	worker.OnCommunication().add(*this, &Application::OnWorkerCommunication);
}

/** \brief Worker removed callback.
 * @param worker the worker
 *
 * Called when a worker has been removed.
 *
 */
void
Application::OnWorkerRemoved(Worker &worker)
{
	worker.OnCommunication().remove(*this, &Application::OnWorkerCommunication);

	// Reschedule the task the worker was working on
	if (worker.Active())
	{
		d_taskQueue.Queue(worker.ActiveTask());
	}
}

/** \brief Worker communication message processing.
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
	if (!d_optimizerManager.Find(task.Id(), optimizer))
	{
		debug_worker << "Optimizer no longer connected..." << endl;
		worker.Deactivate();
		return;
	}

	switch (response.status())
	{
		// Task was completed successfully
		case task::Response::Success:
			if (Debug::enabled(Debug::Domain::Worker))
			{
				debug_worker << "Worker success ("
				             << worker.Client().address().host(true)
				             << ":"
				             << worker.Client().address().port(true)
				             << ") for (" << task.Id()
				             << ", " << task.Message().id() << ")"
				             << endl;
			}

			optimizer.Send(args.Communication);
			worker.Deactivate();
		break;
		// Task execution failed
		case task::Response::Failed:
			task.Failed();

			if (task.Failures() >= Config::Instance().MaxTaskFailures)
			{
				// Task failed too many times
				if (Debug::enabled(Debug::Domain::Worker))
				{
					debug_worker << "Task failed to many times ("
					             << worker.Client().address().host(true)
					             << ":" << worker.Client().address().port(true) <<
					             ") for (" << task.Id() << ", "
					             << task.Message().id() << ")" << endl;
				}

				// Relay failure to optimizer
				optimizer.Send(args.Communication);
				worker.Deactivate();
			}
			else
			{
				// Reschedule the task because the failure might be due to a
				// faulty worker, or maybe some network problems
				if (Debug::enabled(Debug::Domain::Worker))
				{
					debug_worker << "Task failed, rescheduling ("
					             << worker.Client().address().host(true) <<
					             ":" << worker.Client().address().port(true)
					             << ") for (" << task.Id() << ", "
					             << task.Message().id() << ")" << endl;
				}

				// Reschedule task
				d_taskQueue.Queue(task);
				worker.Deactivate();
			}
		break;
		case task::Response::Challenge:
			// Simply relay to optimizer
			if (Debug::enabled(Debug::Domain::Worker))
			{
				debug_worker << "Worker challenge ("
				             << worker.Client().address().host(true) << ":"
				             << worker.Client().address().port(true) << ") for ("
				             << task.Id() << ", "
				             << task.Message().id() << ")" << endl;
			}

			optimizer.Send(args.Communication);
		break;
		default:
		break;
	}
}

/** \brief Dispatch tasks in an idle function.
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
		d_idleDispatch = Glib::signal_idle().connect(sigc::mem_fun(*this, &Application::OnDispatch));
	}
}

/** \brief Dispatch available tasks to available workers.
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
		if (!worker.Activate(task))
		{
			break;
		}
	}

	d_idleDispatch.disconnect();
	return false;
}

/** \brief Main application loop.
 * @param loop the glib main loop to use
 *
 * Runs the main application loop.
 *
 */
void
Application::Run(Glib::RefPtr<Glib::MainLoop> loop) 
{
	os::Signals::install();
	os::Signals::onInterrupt.addData(*this, &Application::OnInterrupt, loop);

	if (d_discovery.isMulticast())
	{
		// Send discovery wakeup message if we are in multicast mode
		SendWakeup();
	}

	d_mainLoop = loop;
	loop->run();
}

/** \brief Send discovery wakeup message.
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
	disc.set_namespace_(d_discovery.ns());

	optimization::messages::discovery::Wakeup *wakeup = disc.mutable_wakeup();

	// Set the connection
	wakeup->set_connection(d_discovery.connection());
	string serialized;

	if (optimization::Messages::create(disc, serialized))
	{
		client.write(serialized);
	}
}

/** \brief Notfy available callback.
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
