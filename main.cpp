#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include "Jobs.hpp"
using namespace std;
/*
need to output:
start time, finish time, total time elapsed, response time
*/
void FIFO(Jobs *jobsArry, int numberOfJobs);
void SJF(Jobs *jobsArry, int numberOfJobs);
void BJF(Jobs *jobsArry, int numberOfJobs);
void STCF(Jobs *jobsArry, int numberOfJobs);
void RR(Jobs *jobsArry, int numberOfJobs);
int findNumJobs();
void outputJobs(Jobs *jobsArry, int numberOfJobs);
int getMaxIndexOfCurrentAvailableJobs(Jobs *jobsArry, int currentTimeStamp, int numberOfJobs);
int main() {
	int numberOfJobsFound = 0;
	ifstream in_stream;
	in_stream.open("jobs.dat");

	numberOfJobsFound = findNumJobs();

	Jobs *jobsArry = new Jobs[numberOfJobsFound];

	for(int i = 0; i < numberOfJobsFound; i++)
	{
		in_stream >> jobsArry[i];
//		cout << jobsArry[i];
	}
	cout << "FIFO: ";
	FIFO(jobsArry, numberOfJobsFound);
	cout << endl;
	cout << "SJF: ";
	SJF(jobsArry, numberOfJobsFound);
	cout << endl;
	cout << "BJF: ";
	BJF(jobsArry, numberOfJobsFound);
	cout << endl;
	cout << "RR: ";
	RR(jobsArry, numberOfJobsFound);
	cout << endl;

	return 0;
}
//*NOTE: this FIFO function also sorts the array of jobs in increasing order of arrival times
void FIFO(Jobs *jobsArry, int numberOfJobs){//no preemption
	int i, j, k, min;
	Jobs temp;
	int currentTimeStamp = 0;//time stamp starts at zero

	//sorting the jobs array by increasing order of arrival times using selection sort (for simplicity)
    for (i = 0; i < (numberOfJobs - 1); i++)
    {
        min = i;
        for (j = i + 1; j < numberOfJobs; j++){
        	if (jobsArry[j].getArrival() < jobsArry[min].getArrival()){
        		min = j;
        	}
        }
        temp = jobsArry[min];
        jobsArry[min] = jobsArry[i];
        jobsArry[i] = temp;
    }
    /*
    need to set the following:
    start time, finish time, total time elapsed, response time
    */
    for(k = 0; k < numberOfJobs; k++){
    	if(jobsArry[k].getArrival() > currentTimeStamp)//if there are no jobs to schedule
    	{
    		currentTimeStamp = jobsArry[k].getArrival();//adjust the current time stamp to skip to the next job's arrival time
    	}
    	jobsArry[k].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
    	jobsArry[k].setFinishTime((jobsArry[k].getDuration() + currentTimeStamp));
    	currentTimeStamp = jobsArry[k].getFinishTime();
    }
    outputJobs(jobsArry, numberOfJobs);
}
void SJF(Jobs *jobsArry, int numberOfJobs){//no preemption

    int i, j;
    Jobs key;
	int k, currentTimeStamp = 0;//time stamp starts at zero

	//at this point (assuming FIFO already ran), the jobs are sorted in increasing order of arrival times

	//run the first job to get a starting time stamp (assuming no 2 jobs arrive at the same time)
	if(jobsArry[0].getArrival() > currentTimeStamp)//if there are no jobs to schedule
	{
		currentTimeStamp = jobsArry[0].getArrival();//adjust the current time stamp to skip to the next job's arrival time
	}
	jobsArry[0].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
	jobsArry[0].setFinishTime((jobsArry[0].getDuration() + currentTimeStamp));
	currentTimeStamp = jobsArry[0].getFinishTime();

	for(k = 1; k < numberOfJobs; k++){
		/*
		run insertion sort (based on increasing duration time) on the array from the job's index up to the job with the highest
		arrival time within (<=) the currentTimeStamp
		*/
	    for (i = (k + 1); i <= getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs); i++) {
	        key = jobsArry[i];
	        j = i - 1;

	        /* Move elements of arr[0..i-1], that are
	          greater than key, to one position ahead
	          of their current position */
	        while (j >= k && jobsArry[j].getDuration() > key.getDuration()) {
	            jobsArry[j + 1] = jobsArry[j];
	            j = j - 1;
	        }
	        jobsArry[j + 1] = key;
	    }
	    //set the appropriate values
    	if(jobsArry[k].getArrival() > currentTimeStamp)//if there are no jobs to schedule
    	{
    		currentTimeStamp = jobsArry[k].getArrival();//adjust the current time stamp to skip to the next job's arrival time
    	}
    	jobsArry[k].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
    	jobsArry[k].setFinishTime((jobsArry[k].getDuration() + currentTimeStamp));
    	currentTimeStamp = jobsArry[k].getFinishTime();
	}
	outputJobs(jobsArry, numberOfJobs);
	/*
	Need to account for arrival times--if a long job arrives earlier than a short job,
	then the long job will run first (this is because we are not required to have preemption
	in this implementation of SJF). Thus, the jobs whose arrival times are within the current
	time stamp should be sorted by increasing order of duration times.
	*/
}
void BJF(Jobs *jobsArry, int numberOfJobs){//no preemption

    int i, j, a, min, b;
    Jobs key, temp;
	int k, currentTimeStamp = 0;//time stamp starts at zero

	//running selection sort (for simplicity--should be improved) just to sort the jobs by increasing order of arrival times
    for (a = 0; a < (numberOfJobs - 1); a++)
    {
        min = a;
        for (b = a + 1; b < numberOfJobs; b++){
        	if (jobsArry[b].getArrival() < jobsArry[min].getArrival()){
        		min = b;
        	}
        }
        temp = jobsArry[min];
        jobsArry[min] = jobsArry[a];
        jobsArry[a] = temp;
    }

	//run the first job to get a starting time stamp (assuming no 2 jobs arrive at the same time)
	if(jobsArry[0].getArrival() > currentTimeStamp)//if there are no jobs to schedule
	{
		currentTimeStamp = jobsArry[0].getArrival();//adjust the current time stamp to skip to the next job's arrival time
	}
	jobsArry[0].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
	jobsArry[0].setFinishTime((jobsArry[0].getDuration() + currentTimeStamp));
	currentTimeStamp = jobsArry[0].getFinishTime();

	for(k = 1; k < numberOfJobs; k++){
		/*
		run insertion sort (based on decreasing duration time) on the array from the job's index up to the job with the highest
		arrival time within (<=) the currentTimeStamp
		*/
	    for (i = (k + 1); i <= getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs); i++) {
	        key = jobsArry[i];
	        j = i - 1;

	        /* Move elements of arr[0..i-1], that are
	          less than key, to one position ahead
	          of their current position */
	        while (j >= k && jobsArry[j].getDuration() < key.getDuration()) {
	            jobsArry[j + 1] = jobsArry[j];
	            j = j - 1;
	        }
	        jobsArry[j + 1] = key;
	    }
	    //set the appropriate values
    	if(jobsArry[k].getArrival() > currentTimeStamp)//if there are no jobs to schedule
    	{
    		currentTimeStamp = jobsArry[k].getArrival();//adjust the current time stamp to skip to the next job's arrival time
    	}
    	jobsArry[k].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
    	jobsArry[k].setFinishTime((jobsArry[k].getDuration() + currentTimeStamp));
    	currentTimeStamp = jobsArry[k].getFinishTime();
	}
	outputJobs(jobsArry, numberOfJobs);
}
void STCF(Jobs *jobsArry, int numberOfJobs){}//yet to be implemented
void RR(Jobs *jobsArry, int numberOfJobs){
    int a, min, b, numberOfJobsDone = 0;
    Jobs temp;
	int currentTimeStamp = 0;//time stamp starts at zero
	bool jobAlreadyStarted[numberOfJobs], alreadyCheckedIfJobDone[numberOfJobs];

	//running selection sort (for simplicity--should be improved) just to sort the jobs by increasing order of arrival times
    for (a = 0; a < (numberOfJobs - 1); a++)
    {
    	jobAlreadyStarted[a] = false;//initializing all jobs to 'not yet started'
    	alreadyCheckedIfJobDone[a] = false;
        min = a;
        for (b = a + 1; b < numberOfJobs; b++){
        	if (jobsArry[b].getArrival() < jobsArry[min].getArrival()){
        		min = b;
        	}
        }
        temp = jobsArry[min];
        jobsArry[min] = jobsArry[a];
        jobsArry[a] = temp;
    }
    //the selection sort before this does not initialize the last element of the bool arrays
    jobAlreadyStarted[(numberOfJobs - 1)] = false;
    alreadyCheckedIfJobDone[(numberOfJobs - 1)] = false;

    /*
    jobs whose arrival time is within the current time stamp need to be time sliced into predetermined quanta of time
    and swapped in a 'round robin' fashion
     */
    while(numberOfJobsDone != numberOfJobs){//while all jobs are not done
    	/*
    	run through all jobs currently available (all jobs whose arrival time is within (<=) the currentTimeStamp)
    	 */
	    for (int i = 0; i <= getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs); i++) {
	    	if(jobsArry[i].getRemainingTime() <= 0){//the 'i' job is done (duration has expired)
	    		if(alreadyCheckedIfJobDone[i] == true){//you already updated numberOfJobsDone and set finish time so do nothing
	    		}
	    		else{//you have not updated numberOfJobsDone so set finish time, increment numberOfJobsDone, and update alreadyCheckedIfJobsDone
	    			alreadyCheckedIfJobDone[i] = true;
		    		numberOfJobsDone = numberOfJobsDone + 1;
		    		jobsArry[i].setFinishTime(currentTimeStamp);
	    		}
	    	}
	    	else{//the 'i' job is not done (duration has not expired) so take a time slice from this job and update current time stamp
	        	if(jobsArry[i].getArrival() > currentTimeStamp){//if there are no jobs to schedule
	        		currentTimeStamp = jobsArry[i].getArrival();//adjust the current time stamp to skip to the next job's arrival time
	        	}
	    		if(jobAlreadyStarted[i] == true){//job already started so don't need to set the start time
	    			if(jobsArry[i].getRemainingTime() >= 5){
	    				jobsArry[i].takeTimeSliceAway(5);//we chose 5 as the quanta of time that a job is alloted in the RR scheduler
		    			currentTimeStamp = currentTimeStamp + 5;//increase currentTimeStamp by one time interval
	    			}
	    			else{//time is less than 5 so just take it all away
	    				currentTimeStamp = currentTimeStamp + jobsArry[i].getRemainingTime();//increase currentTimeStamp by remainingTime
	    				jobsArry[i].takeTimeSliceAway(jobsArry[i].getRemainingTime());
	    			}
	    		}
	    		else{//this is first time job starts, so set start time
	    			jobAlreadyStarted[i] = true;
	    			jobsArry[i].setStartTime(currentTimeStamp);
	    			if(jobsArry[i].getRemainingTime() >= 5){
	    				jobsArry[i].takeTimeSliceAway(5);//we chose 5 as the quanta of time that a job is alloted in the RR scheduler
		    			currentTimeStamp = currentTimeStamp + 5;//increase currentTimeStamp by one time interval
	    			}
	    			else{//time is less than 5 so just take it all away
	    				currentTimeStamp = currentTimeStamp + jobsArry[i].getRemainingTime();//increase currentTimeStamp by one time interval
	    				jobsArry[i].takeTimeSliceAway(jobsArry[i].getRemainingTime());
	    			}
	    		}
	    	}
	    }

	    //fixing the currentTimeStamp if all jobs within the currentTimeStamp are done
    	if(numberOfJobsDone == (getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs) + 1))//if all the jobs within the currentTimeStamp are done
    	{
    		currentTimeStamp = jobsArry[numberOfJobsDone].getArrival();//adjust the current time stamp to skip to the next job's arrival time
    	}
    }
	outputJobs(jobsArry, numberOfJobs);
}
void outputJobs(Jobs *jobsArry, int numberOfJobs){//print jobs results
	cout << "\t\tStart Time" << "\tFinish Time" << "\tTotal Time Elapsed" << "\tResponse Time\n";
	cout << "_____________________________________________________________________________________\n";
    for(int a = 0; a < numberOfJobs; a++){
    	cout << "Job ID " << jobsArry[a].getID() << ": "
    			<< "\t" << jobsArry[a].getStartTime() << "\t"
				<< "\t" << jobsArry[a].getFinishTime() << "\t"
				<< "\t" << (jobsArry[a].getFinishTime() - jobsArry[a].getArrival()) << "\t"
				<< "\t\t" << (jobsArry[a].getStartTime() - jobsArry[a].getArrival()) << endl;
    }
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
int getMaxIndexOfCurrentAvailableJobs(Jobs *jobsArry, int currentTimeStamp, int numberOfJobs){//if the current job's arrival time is <= currentTimeStamp
	int index = 0;
	/*
	note that this function assumes you have already sorted the jobsArry in increasing order
	of job arrival times
	*/

	for(int i = 1; i < numberOfJobs; i++){
		if(jobsArry[i].getArrival() <= currentTimeStamp){
			index = i;
		}
	}
	return index;

}

