#ifndef CONFIG_EXCEPTIONS_H
#define CONFIG_EXCEPTIONS_H

#include <exception>
namespace seap_implement {

	class ConfigException : public exception
	{
		public:
			virtual const char* what() const throw() {
				return "Error genérico seap_implement::Config";
			}
	};

	class ConfigExceptionNotFound : public ConfigException
	{
		public:
			virtual const char* what() const throw() {
				return "No se registró una variable antes de acceder a ella";
			}
		private:

	};

	class ConfigExceptionBadType : public ConfigException
	{
		public:
			virtual const char* what() const throw() {
				return "Se intenta acceder a una variable de tipo incorrecto";
			}
		private:

	};

	class ConfigExceptionNoData : public ConfigException
	{
		public:
			virtual const char* what() const throw() {
				return "Se intenta acceder a una variable sin datos";
			}
		private:

	};

}

#endif // CONFIG_EXCEPTIONS_H

