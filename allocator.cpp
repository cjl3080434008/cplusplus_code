#define MAX_BYTES 512 //一页的大小，多余一页就
#define ALIGIN    8
#define FREEBLOCK_INDEX(size) ((size+ALIGIN-1)/ALIGIN-1)


class allocator{
public:
	void *alloc(size_t size);
	void free(void *ptr, size_t size);
	void get_allocate_info();
private:
	void align(size_t size);

	char *volatile free_block_[FREEBLOCK_INDEX(MAX_BYTES)+1];
};


size_t allocator::align(size_t size)
{
	return (size+ALIGIN-1)&~(ALIGIN-1);
}

void allocator::free(void *ptr, size_t size)
{
	if(size > MAX_BYTES)
	{
		::free(ptr);
		return;
	}

	boost::mutex::scoped_lock lock(mutex_);

	size_t align_size = align(size);
	int idx = getIndex(align_size);
	char *pNext = free_block_[idx];
	free_block_[idx] = (char *)ptr;
	*(char **)ptr = pNext;
	/*************************************
	 * (char **)ptr其实是这里的一个约定
	 * 可以假设其内存布局为：
	 *--------
	 *|	  p  |  p用于指向当前的地址空间
	 *|------|
	 *|	**pre|  pre用于指向pNext下一个
	 *--------	  连续空间地址 
	 ************************************/
}

void *allocator::alloc(size_t size)
{
	assert(size > 0);
	if(size > MAX_BYTES)
		return ::malloc(ALIGIN(size));

	boost::mutex::scoped_lock lock(mutex_);
	int idx = FREEBLOCK_INDEX(size);
	if(free_block_[idx] == NULL) //分配空间
		allocate_new_chunk(ALIGIN(size), idx);

	char *ptr = free_block_[idx];
	free_block_[idx] = *(char **)ptr;

	return ptr;
}

//这里只解决了free_block_[idx]从未分配到分配的情况，
//而没有解决free_block_[idx]分配了以后用完内存的情况
//下面的refill可以处理这种情况。
void allocator::allocate_new_chunk(size_t size, uint32 idx)
{
	free_block_[idx] = (char *)::malloc(MALLOCBYTES);
	int chunk_nums = MALLOCBYTES/size;

	char *pCur = free_block_[idx];
	//建立前后块链接
	for(int i = 0; i < chunk_nums; ++i)
	{
		char *pNext = pCur+size;
		if(i == chunk_nums-1)
			pNext = 0;
		*(char **)pCur = pNext;
		pCur = pNext;
	}
	
}

/*
 * 这边的处理更多是为了考虑现实内存的使用情况，以及内存用完继续申请的情况
void refill(short n)
	{
		if(_end_free-_start_free<n) alloc_chunk();

		unsigned long ctAlloc = (_end_free-_start_free)/n;
		if(ctAlloc>FIRSTALLOCATENUMS) ctAlloc = FIRSTALLOCATENUMS;

		int idx = FREEBLOCK_INDEX(n);
		_mem_info_list[idx].fillnum++;
		_mem_info_list[idx].nowremain = ctAlloc;

		char* pCur = _free_block[idx] = _start_free;
		for(unsigned long i=0; i<ctAlloc; i++)
		{
			char* pNext = pCur+n;
			if(i==ctAlloc-1) pNext = 0;

			*(char**)pCur = pNext; 
			pCur = pNext;
		}
		_start_free+=n*ctAlloc;
	}

	void alloc_chunk()
	{
		_start_free = (char*)malloc(CHUNK_SIZE);
		if(_start_free==NULL)
		{
			//DWORD error = GetLastError(); 
			//处理内存分配错误
			//ErrorCodeMessage errmsg(error);
			//throw Win32Tools::CWin32Exception(_T("CMemoryPool::chunk_alloc()-VirtualAlloc fail"),::GetLastError());
			//exit(0);
		}
		else
		{
			_mem_list.push_back(_start_free);
			_end_free = _start_free + CHUNK_SIZE;
			_heap_size += CHUNK_SIZE;
		}
	}
 */ 


