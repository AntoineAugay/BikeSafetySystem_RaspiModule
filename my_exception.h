#ifndef ERROR_H
#define ERROR_H


#include <exception>
#include <string>


class myException : public std::exception{
private :
	
	int id;           // exception id
    std::string desc; // exception description
	int level; // exception description

public :

	/*!
	 *  \brief myException constructor 
	 *
	 *	\param _id : erro id
	 *	\param _desc : exception description
 	 */	
	myException(int _id=0, std::string const& _desc="", int _level=0)
         :id(_id), desc(_desc), level(_level)
    {}

    /*!
	 *  \brief what function, use to acces to the exception description
	 *  
	 *	\return const char* : exception description
 	 */	
	virtual const char* what()
	{
		return desc.c_str();
	}

	/*!
	 *  \brief getId function, use to acces to the exception id
	 *  
	 *	\return int : exception id
 	 */	
	int getId()
	{
		return id;
	}

	/*!
	 *  \brief getLevel function, use to acces to the exception level
	 * 	
	 *	level = 0 : quit program
	 *	level = 1 : doesn't quit program 
	 *
	 *	\return int : exception level
 	 */	
	int getLevel()
	{
		return level;
	}


    virtual ~myException()
    {}

};


#endif