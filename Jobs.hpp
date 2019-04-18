#ifndef JOBS_HPP_
#define JOBS_HPP_
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
/*
need to output:
start time, finish time, total time elapsed, response time
*/
class Jobs
{
public:
	Jobs();
	int getArrival();
	int getID();
	int getDuration();
	int getRemainingTime();
	void takeTimeSliceAway(int timeToTakeAway);
	friend istream &operator >>(istream &in_stream, Jobs &job);
	friend ostream &operator <<(ostream &out_stream, const Jobs &job);
	void operator =(const Jobs &job);
	~Jobs();
	//calculated values depending on scheduling algorithms:
	int getStartTime();
	int getFinishTime();
	int getTotalTimeElapsed();
	int getResponseTime();
	void setStartTime(int start);
	void setFinishTime(int finish);
	void setTotalTimeElapsed(int total);
	void setResponseTime(int response);
private:
	int arrival, completion, ID, duration, remainingTime;
	int startTime, finishTime, totalTimeElapsed, responseTime;
};

#endif /* JOBS_HPP_ */
