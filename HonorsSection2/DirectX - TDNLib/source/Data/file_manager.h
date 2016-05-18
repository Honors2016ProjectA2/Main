
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

class File_manager
{
private:
	File_manager(){};
	~File_manager(){};

public:
	static void Get_ranking(const char *filename, int scores[], const int max);
	static void Set_ranking(const char *filename, int scores[], const int max);
	static void Sort(int scores[], const int max);
};

#endif // !FILE_MANAGER_H
