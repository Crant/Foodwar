#pragma once

#ifndef NULL
	#define NULL 0
#endif

#define SAFE_DELETE(x) if (x != NULL) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) if (x != NULL) { delete [] x; x = NULL; }	

 //SINGLETONS

 //Define a class as a singleton (Add to class definition in header file)

#define CDEFINE_SINGLETONS(the_class)			\
private:										\
	static the_class* _instance;				\
	the_class() {}								\
	~the_class() {}								\
	the_class(const the_class &);				\
	the_class& operator=(const the_class &);	\
public:											\
	static void Create();						\
	static void Destroy();						\
	static the_class* GetInstance();			

//
// Declare singleton methods (Add to source file)
//
#define CDECLARE_SINGLETONS(the_class)			\
	the_class* the_class::_instance = NULL;		\
void the_class::Create()						\
{												\
	if (_instance == NULL)						\
		_instance = new the_class;				\
}												\
void the_class::Destroy()						\
{												\
	if (_instance != NULL)						\
	{											\
		delete _instance;						\
		_instance = NULL;						\
	}											\
}												\
the_class* the_class::GetInstance()				\
{												\
	return _instance;							\
}	