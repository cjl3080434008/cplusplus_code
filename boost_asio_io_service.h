//每个#ifndef ---endif都是一个.hpp文件


#ifndef IO_SERVICE_H
#define IO_SERVICE_H

lass io_service{
	class ip;			//标识	
	class work;			
	class service;

};

class work{
private:
	io_service *io_s_;
};

//所有io_service的services的基类
class service{

private:
	io_service *owner;
	service *next;		 //service链
};

#endif

#ifndef SERVICE_BASE_H
template<class TYPE>
class service_base:public service
{
public:
	service_base(io_service &io_service):
		<if has namespace use namespace::>service(io_service){}
	
};
#endif

#ifndef TASK_IO_SERVICE_H
class task_io_service:public service_base<task_io_service>
{
//真正一个service的调用，run,  run_one, dispatch, stop, 

};
#endif


#ifndef SERVICE_REGISTER_H
#define SERVICE_REGISTER_H

class service_register{
public:
	service_register(const io_service &owner):owner(&owner){}

	void use_service();//以作用链从前往后调用
	void add_service(service *new_service);
	bool has_service() const;

private:
	io_service *owner_;
	boost::Mutex mutex_;
	service *first_;
};


#endif





