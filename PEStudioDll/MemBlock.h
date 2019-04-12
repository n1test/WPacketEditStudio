class MemBlock
{
	public:
		MemBlock();
		MemBlock(unsigned char *buff, size_t sz);
		virtual ~MemBlock();

		bool allocate(size_t sz);
		bool reallocate(size_t sz);

		bool mem_attach(unsigned char *buff, size_t sz);
		bool mem_set(unsigned char *buff, size_t sz);
		
		bool copy_ptr(MemBlock &object);
		bool copy_mem(MemBlock &object);

		size_t get_size();
		unsigned char *get_ptr();
		unsigned char get_char(size_t index);

	private:
		size_t alloc_size;
		unsigned char *memory_ptr;
		bool readonly;

		void mem_init(size_t sz);
};
