void* zalloc(size_t size);
void* zrealloc(void* mem, unsigned int amount);
void zfree(void* p);

void* zalignedalloc(size_t size, size_t align);
void* zalignedrealloc(void* mem, size_t size, size_t align);
void zalignedfree(void* p);

