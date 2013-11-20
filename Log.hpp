#ifndef LOG_H
#define LOG_H

#include<mutex>
#include<ctime>
#include<cstdio>

#define MAX_LOG_BUF_LEN 1024*1024

class log
{
	struct log_buf{
		size_t len;
		char buf[0];
	};
public:
	log(){new_file();};
	~log(){close();}

	write(char *msg);	

	void flush();


private:
	close();
	new_file();

	swap_buf();
	get_free_buf();

private:
	std::list<char *>	full_buf_;
	std::list<char *>	empty_buf_;
	FILE				*file_;
	std::mutex			mutex_;
};

#endif


#ifndef LOG_MGR_H
#define LOG_MGR_H

#include "log.hpp"
#include "singleton.h"
#include <thread>   //c++11    gcc4.8 or later    -std=c++11
#include <memory>
#include <functional>
#include <cstdarg>
#include <chrono>	//c++11

class log_mgr
{
	enum log_type{
		ERROR = 1,
		DEBUG,
		SYSTEM,
		SQL,
	};

	typedef std::shared_ptr<log> log_ptr;
	typedef std::map<int ,log_ptr> log_map_t;
public:	
	log_mgr(){}
	~log_mgr(){stop();}	

	//外部控制
	init_logs();//fill four log_type file into the logs
	start()
	{
		close_ = false;
		thread_ = new std::thread(std::bind(&log_mgr::flush, this));
	}

	stop()
	{
		_close = true;
		thread.join();
	}

	//逻辑处理
	void log(int type, char *msg, ...)
	{
		log_ptr log = logs_[type];
		if(!log)
			//{	return ;}
			set_log(type, "");	

		char buf[2048] = {0};
		va_list args;

		va_start(args, msg);
		vsprintf(buf, msg, args);
		va_end(args);
		log->write(buf);
	}


private:
	void flush()//将所有Logs中的数据flush到磁盘上
	{
		while(1)
		{
			for(auto log : logs_)	
			{
				log.second->flush();				
			}

			thread_.sleep(std::chrono::seconds(5));
		}
	}

	void set_log(int type, std::string type_name);

private:
	std::thread thread_;	
	log_map_t	logs_;
	bool		close_;
};

typedef singleton<log_mgr> singleton_log_mgr;
#define LOG_MGR singleton_log_mgr::get_instance();

#endif
