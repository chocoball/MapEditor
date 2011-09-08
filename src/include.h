#ifndef INCLUDE_H
#define INCLUDE_H

#define kAccessor(type, member, func_name)					\
	public:													\
		type get##func_name () { return member ; }			\
		void set##func_name (type val) { member = val ; }	\
	private:												\
		type	member ;

#define kScreenW	720
#define kScreenH	480
#define kPI			3.14159265358979323846

#include "csetting.h"
#include "ceditdata.h"


#endif // INCLUDE_H
