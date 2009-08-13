#ifndef __MASTER_MANAGERS_SCANNER_H__
#define __MASTER_MANAGERS_SCANNER_H__

#include <string>

namespace optimaster
{
namespace managers
{
	class Scanner
	{
		protected:
			/* Abstract */
			virtual std::string searchEnvironmentPath() const = 0;
			virtual std::string searchDirectory() const = 0;			

			void scan();
			void scan(std::string const &directory);
			
			virtual void addModule(std::string const &filename) = 0;
		private:
			/* Private functions */
	};
}
}

#endif /* __MASTER_MANAGERS_SCANNER_H__ */
