/**
 * \class arg::cConfigurator
 * \brief A 'configurator' for other classes.
 *
 * The class is supposed to serve as a configurator of other classes and separate
 * configuration from function.
 *
 * \author Pavel Krömer (c) 2011
 *
 */

#ifndef CCONFIGURATOR_H_
#define CCONFIGURATOR_H_

#include "../utils/cSetting.h"
#include "../core/cGlobalDef.h"

namespace arg
{
	template<class T>
	class cConfigurator
	{		
			inline const cParamItem * Setting(const unsigned int id);
			static const t_CodeTable m_BasicTable[];		

		protected:
			static std::ostream * m_LastLogStream;
			
				enum
			{
				Debug = 1, InputFile, OutputFile
			};			

			cSetting & m_Setting;			
			
			/** This is just to avoid silly warning C4512 in MSVC. */
			cConfigurator<T> & operator=(const cConfigurator<T> &);

			inline int Integer(const unsigned int id, const int def);
			inline double Double(const unsigned int id, const double def);
			inline bool Bool(const unsigned int id);
			inline const char * String(const unsigned int id, const char * def = NULL);

			void Subscribe(const t_CodeTable table[], const unsigned int table_size);

		public:
			cConfigurator(cSetting & setting, const t_CodeTable table[], const unsigned int table_size);
			virtual bool Configure(T& object) = 0;
			void LogConfiguration(void);
			static std::ostream & LastLogStream(void) {return *(cConfigurator<T>::m_LastLogStream);};
	};

	template<class T>
	const t_CodeTable cConfigurator<T>::m_BasicTable[] =
	{
			CodeTable(Debug, tt_Integer),
			CodeTable(InputFile, tt_Text),
			CodeTable(OutputFile, tt_Text)
	};

	template<class T>
	std::ostream * cConfigurator<T>::m_LastLogStream = &std::cout;

	template<class T>
	cConfigurator<T>::cConfigurator(cSetting & setting, const t_CodeTable table[], const unsigned int table_size) :
		m_Setting(setting)
	{
		cParamItem * pi = NULL;
		m_Setting.GetParam(Debug, pi);

		if (pi == NULL) 
		{
			/// \todo Remove constant (it is here only because DIM (sizeof) cannot be used on templates)
			Subscribe(m_BasicTable, 3);
		}
		
		Subscribe(table, table_size);
	}

	template<class T>
	void cConfigurator<T>::Subscribe(const t_CodeTable table[], const unsigned int table_size)
	{
		for (unsigned int i = 0; i < table_size; i++)
		{
			m_Setting.enterSymbol(table[i]);
		}
	}

	template<class T>
	inline const cParamItem * cConfigurator<T>::Setting(const unsigned int id)
	{
		cParamItem* mParamItem = NULL;
		m_Setting.Refresh();

		if (m_Setting.GetParam(id, mParamItem) == true && mParamItem->Read())
		{
			return mParamItem;
		}
		else
		{
			return NULL;
		}
	}

	template<class T>
	inline int cConfigurator<T>::Integer(const unsigned int id, const int def)
	{
		const cParamItem* mParamItem = Setting(id);
		return (mParamItem == NULL) ? def : mParamItem->iValue();
	}

	template<class T>
	inline double cConfigurator<T>::Double(const unsigned int id, const double def)
	{
		const cParamItem* mParamItem = Setting(id);
		return (mParamItem == NULL) ? def : mParamItem->dValue();
	}

	template<class T>
	inline const char * cConfigurator<T>::String(const unsigned int id, const char * def)
	{
		const cParamItem* mParamItem = Setting(id);
		return (mParamItem == NULL) ? def : mParamItem->tValue();
	}

	template<class T>
	inline bool cConfigurator<T>::Bool(const unsigned int id)
	{
		const cParamItem* mParamItem = Setting(id);
		return (mParamItem == NULL) ? false : mParamItem->iValue() > 0;
	}

	template<class T>
	inline void cConfigurator<T>::LogConfiguration(void)
	{
		cArray<cParamItem> & params = m_Setting.ParamArray();
		*m_LastLogStream << "Program Configuration:\n";
		for (unsigned int i = 0; i < params.Count(); i++) 
		{
			cParamItem * pi = params[i];
			*m_LastLogStream << pi->Name() << "\t\t"; 
			
			switch(pi->Type())
			{
			case tt_Integer:
				*m_LastLogStream << pi->iValue();				
				break;
			case tt_Double:
				*m_LastLogStream << pi->dValue();
				break;
			case tt_Text:
				*m_LastLogStream << pi->tValue();
				break;				
			}
			*m_LastLogStream << "\t\t[" << pi->Read() << "]\n";
		}
		*m_LastLogStream << "===\n";
	}
}

#endif /* CCONFIGURATOR_H_ */
