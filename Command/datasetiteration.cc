#include "command.ih"

void Command::Data::setIteration(db::Row &row, command::Iteration &iteration) 
{
	vector<string>::iterator iter;
	
	iteration.set_id(row.get<size_t>("iteration"));
	iteration.set_time(row.get<size_t>("time"));

	vector<string> fitness = String(row.get<string>("best_fitness")).split(",");
	
	for (size_t i = 0; i < fitness.size(); ++i)
	{
		iteration.add_fitness(String(fitness[i]));
	}
}
