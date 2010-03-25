#include "command.hh"
#include "application.hh"

#include "config.hh"

#include <optimization/constants.hh>
#include <gcrypt.h>

using namespace optimaster;
using namespace jessevdk::network;
using namespace jessevdk::os;
using namespace optimization::messages;
using namespace optimization;
using namespace std;

#define CHALLENGE_SIZE 32

Command::Data::Data(Application &application)
:
	application(application)
{
	gcry_check_version(NULL);
}

Client
Command::Data::Accept()
{
	Client client = TcpServer::Data::Accept();

	if (client)
	{
		ClientData data;
		data.authenticated = false;

		clientData[client.Fd()] = data;

		client.OnData().AddData(*this, &Data::OnClientData, client);
		client.OnClosed().AddData(*this, &Data::OnClientClosed, client);
	}

	return client;
}

void
Command::Data::HandleList(Client                     &client,
                          command::ListCommand const &command)
{
	command::Response response = CreateResponse(command::List,
	                                            true);

	command::ListResponse *list = response.mutable_list();
	vector<Job> jobs = application.Manager().Jobs();

	for (vector<Job>::iterator iter = jobs.begin(); iter != jobs.end(); ++iter)
	{
		command::Job *cjob = list->add_jobs();
		FillJob(*iter, *cjob);
	}

	Respond(client, response);
}

void
Command::Data::HandleInfo(Client                     &client,
                          command::InfoCommand const &command)
{
	Job job;

	if (!application.Manager().Find(command.id(), job))
	{
		command::Response response = CreateResponse(command::Info,
	                                                false,
	                                                "Job not found");
		Respond(client, response);
		return;
	}

	command::Response response = CreateResponse(command::Info, true);
	command::InfoResponse *info = response.mutable_info();
	command::Job *cjob = info->mutable_job();

	FillJob(job, *cjob);
	Respond(client, response);
}

bool
Command::Data::CheckAuthentication(Client               &client,
                                   command::CommandType  type)
{
	ClientData &data = clientData[client.Fd()];

	if (!data.authenticated)
	{
		command::Response response = CreateResponse(type,
		                                            false,
		                                            "Not authenticated");
		Respond(client, response);
		return false;
	}

	return true;
}

void
Command::Data::HandleKill(Client                     &client,
                          command::KillCommand const &command)
{
	if (!CheckAuthentication(client, command::Kill))
	{
		return;
	}

	Job job;

	if (!application.Manager().Find(command.id(), job))
	{
		command::Response response = CreateResponse(command::Kill,
		                                            false,
		                                            "Job not found");
		Respond(client, response);
		return;
	}

	string message = "Job " + job.Name() + " (" + job.User() + ") has been killed...";
	job.Client().Close();

	command::Response response = CreateResponse(command::Kill,
	                                            true,
	                                            message);
	response.mutable_kill();

	Respond(client, response);
}

void
Command::Data::HandleSetPriority(Client                            &client,
                                 command::SetPriorityCommand const &command)
{
	if (!CheckAuthentication(client, command::Kill))
	{
		return;
	}

	Job job;

	if (!application.Manager().Find(command.id(), job))
	{
		command::Response response = CreateResponse(command::SetPriority,
		                                            false,
		                                            "Job not found");
		Respond(client, response);
		return;
	}

	job.SetPriority(command.priority());

	command::Response response = CreateResponse(command::SetPriority,
	                                            true,
	                                            "Job priority set...");
	Respond(client, response);
}

void
Command::Data::GenerateChallenge(Client                             &client,
                                 command::AuthenticateCommand const &command)
{
	unsigned char blk[CHALLENGE_SIZE / 2];
	stringstream challenge;

	gcry_create_nonce(blk, sizeof(blk));

	for (size_t i = 0; i < CHALLENGE_SIZE / 2; ++i)
	{
		challenge << hex << blk[i];
	}

	ClientData &data = clientData[client.Fd()];
	data.challenge = challenge.str();
	
	command::Response response = CreateResponse(command::Authenticate, true);
	response.mutable_authenticate()->set_challenge(data.challenge.c_str());

	Respond(client, response);
}

void
Command::Data::ValidateChallenge(Client                             &client,
                                 command::AuthenticateCommand const &command)
{
	ClientData &data = clientData[client.Fd()];
	string passwd = Config::Instance().CommandPassword;

	gcry_md_hd_t hd;

	if (gcry_md_open(&hd, GCRY_MD_SHA1, 0) != GPG_ERR_NO_ERROR)
	{
		command::Response response = CreateResponse(command::Authenticate,
		                                            false,
		                                            "Invalid password (unable to initialize gcrypt)");
		Respond(client, response);
		return;
	}

	gcry_md_write(hd, passwd.c_str(), strlen(passwd.c_str()));
	gcry_md_write(hd, data.challenge.c_str(), strlen(data.challenge.c_str()));

	uint8_t *ptr = gcry_md_read(hd, GCRY_MD_SHA1);
	stringstream token;

	for (size_t i = 0; i < gcry_md_get_algo_dlen(GCRY_MD_SHA1); ++i)
	{
		token << hex << ptr[i];
	}

	gcry_md_close(hd);

	if (command.token() == token.str() && passwd != "")
	{
		data.authenticated = true;

		command::Response response = CreateResponse(command::Authenticate,
		                                            true);
		response.mutable_authenticate();

		Respond(client, response);
	}
	else
	{
		command::Response response = CreateResponse(command::Authenticate,
		                                            false,
		                                            "Invalid password");
		Respond(client, response);
	}
}

void
Command::Data::HandleAuthenticate(Client                             &client,
                                  command::AuthenticateCommand const &command)
{
	ClientData &data = clientData[client.Fd()];

	if (data.challenge != "")
	{
		ValidateChallenge(client, command);
	}
	else
	{
		GenerateChallenge(client, command);
	}
}

void
Command::Data::OnClientData(FileDescriptor::DataArgs &args, Client client)
{
	vector<command::Command> cmds;
	vector<command::Command>::iterator iter;

	Messages::Extract(args, cmds);

	for (iter = cmds.begin(); iter != cmds.end(); ++iter)
	{
		command::Command &c = *iter;
		bool invalid = false;
		
		switch (c.type())
		{
			case command::List:
				if (c.has_list())
				{
					HandleList(client, c.list());
				}
				else
				{
					invalid = true;
				}
			break;
			case command::Info:
				if (c.has_info())
				{
					HandleInfo(client, c.info());
				}
				else
				{
					invalid = true;
				}
			break;
			case command::Kill:
				if (c.has_kill())
				{
					HandleKill(client, c.kill());
				}
				else
				{
					invalid = true;
				}
			break;
			case command::SetPriority:
				if (c.has_setpriority())
				{
					HandleSetPriority(client, c.setpriority());
				}
				else
				{
					invalid = true;
				}
			break;
			case command::Authenticate:
				if (c.has_authenticate())
				{
					HandleAuthenticate(client, c.authenticate());
				}
				else
				{
					invalid = true;
				}
			break;
			default:
			break;
		}

		if (invalid)
		{
			/* Write back response... */
			Respond(client, c.type(), false, "Invalid command");
		}
	}
}

void
Command::Data::OnClientClosed(int fd, Client client)
{
	clientData.erase(fd);

	client.OnClosed().RemoveData(*this, &Command::Data::OnClientClosed, client);
}

command::Response
Command::Data::CreateResponse(command::CommandType  type,
                              bool                  status,
                              string const         &message) const
{
	command::Response response;

	response.set_type(type);
	response.set_status(status);
	response.set_message(message);

	return response;
}

void
Command::Data::Respond(Client &client, command::CommandType type, bool status, string const &data)
{
	command::Response response = CreateResponse(type, status, data);

	string serialized;

	if (Messages::Create(response, serialized))
	{
		client.Write(serialized);
	}
}

void
Command::Data::Respond(Client            &client,
                       command::Response &response)
{
	string serialized;

	if (Messages::Create(response, serialized))
	{
		client.Write(serialized);
	}
}

Command::Command(Application &application)
{
	d_data = new Data(application);
	AddPrivateData(d_data);

	SetData(d_data);

	Set(Constants::CommandPort);
}

void
Command::Data::FillJob(Job const    &job,
                       command::Job &cjob)
{
	cjob.set_id(job.Id());
	cjob.set_name(job.Name());
	cjob.set_user(job.User());
	cjob.set_started((size_t)job.Started().as_double());
	cjob.set_priority(job.Priority());
	cjob.set_progress(job.Progress());

	cjob.set_taskssuccess(job.TasksSuccess());
	cjob.set_tasksfailed(job.TasksFailed());
}
