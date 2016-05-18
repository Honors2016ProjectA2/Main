
#include "file_manager.h"

#include <fstream>

void File_manager::Get_ranking(const char *filename, int scores[], const int max)
{
	using namespace std;

	ifstream ifs(filename);

	for (int i = 0; i < max; i++)
	{
		if (!ifs.eof())
		{
			ifs >> scores[i];
			continue;
		}

		scores[i] = 0;
	}
}

void File_manager::Set_ranking(const char *filename, int scores[], const int max)
{
	using namespace std;

	ofstream ofs(filename);

	for (int i = 0; i < max; i++)
	{
		ofs << scores[i] << '\n';
	}

	ofs.close();
}

void File_manager::Sort(int scores[], const int max)
{
	int c(0);
	for (int i = 0; i < max; i++)
	{
		for (int j = i + 1; j < max; j++)
		{
			if (scores[i] < scores[j])
			{
				c = scores[i];
				scores[i] = scores[j];
				scores[j] = c;
			}
		}
	}
}
