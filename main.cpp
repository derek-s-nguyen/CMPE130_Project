#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include "Jobs.hpp"
using namespace std;
int findNumJobs();
int main() {
	int numberOfJobsFound = 0;
	ifstream in_stream;
	in_stream.open("jobs.dat");

	numberOfJobsFound = findNumJobs();

	Jobs *jobsArry = new Jobs[numberOfJobsFound];
	for(int i = 0; i < numberOfJobsFound; i++)
	{
		in_stream >> jobsArry[i];
		cout << jobsArry[i];
	}

	return 0;
}

int findNumJobs()
{
	int numberOfJobsFound = 0, counter = 0;
	int next;
	ifstream in_stream;
	in_stream.open("jobs.dat");
	in_stream >> next;

	while(!in_stream.eof())
	{
		if((counter % 3) == 0)
		{
			numberOfJobsFound++;
		}
		in_stream >> next;
		counter++;
	}
	in_stream.close();
	return numberOfJobsFound;
}
