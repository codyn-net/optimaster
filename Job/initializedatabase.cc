#include "job.ih"

void Job::initializeDatabase()
{
	SQLite &db = d_data->optimizer->db();
	
	if (!db)
	{
		return;
	}
	
	db.begin();
	
	db.query() << "CREATE TABLE IF NOT EXISTS `dispatcher` ("
		       << "`name` TEXT, "
		       << "`value` TEXT)"
		       << SQLite::Query::End();

	vector<string> props = d_data->dispatcher.properties();
	
	for (vector<string>::iterator iter = props.begin(); iter != props.end(); ++iter)
	{
		db.query() << "INSERT INTO dispatcher (`name`, `value`) VALUES ('"
			       << SQLite::serialize(*iter) << "', '"
			       << SQLite::serialize(d_data->dispatcher.property(*iter)) << "')"
		           << SQLite::Query::end();
	}
	
	// Store optimizer name
	db.query() << "INSERT INTO dispatcher (`name`, `value`) VALUES ('dispatcher', "
	           << "'" << SQLite::serialize(d_data->dispatcher.name()) << "')"
	           << SQLite::Query::end();

	db.commit();
}
