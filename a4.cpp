//441 Assignment 4
//Daniel Artuso 30029826

#include <iostream>
#include <list>
#include <cmath>

//#define Bonus 1
//#define DifferentSongLength 1
#define Regular 1

//This is the time
#define MonthMinutes 43800
using namespace std;

float Uniform01();

float Exponential(float mu);

class Budgie {
	bool isSinging = false;
	float length;
	float startTime;
	float endTime;
public:
	Budgie(float songLength, float quietTime) {
		float totalLength = quietTime + songLength;

		if (Uniform01() <= (quietTime / totalLength)) {
			isSinging = false;
			doQuiet(quietTime);
		} else {
			isSinging = true;
			doSong(songLength);
		}
	}

	bool operator<(const Budgie &comp) {
		return endTime < comp.endTime;
	}


	pair<bool, float> getInfo() {
		pair<bool, float> info;
		info.first = isSinging;
		info.second = length;
		return info;
	}

	bool isSing() const {
		return isSinging;
	}

	float getStartTime() const {
		return startTime;
	}

	float getEndTime() const {
		return endTime;
	}

	void setEventTimes(float time) {
		Budgie::startTime = time;
		Budgie::endTime = time + length;
	}

	float getLength() const {
		return length;
	}

	float doSong(float averageSongLength) {
		isSinging = true;
		length = Exponential(averageSongLength);
#ifdef Bonus
		length = 3;
#endif
		return length;
	}

	float doQuiet(float averageQuietLength) {
		isSinging = false;
		length = Exponential(averageQuietLength);
		return length;
	}
};


class BLAN {
	int numberBudgies;
	float avgSongLength = 3.0;
	float avgQuietLength = 30.0;
	list<Budgie> birds;
	float time = 0.0;

	int singingCounter = 0;
	int numberSinging = 0;
	float perfectSongs = 0;
	float totalSongs = 0;

	float quietTime = 0;
	float melodiousTime = 0;
	float squawkyTime = 0;


	void printInputs() {
		cout << "N birds: " << numberBudgies
		     << "\tAvg. Quiet time: " << avgQuietLength
		     << "\tAvg. Song time: " << avgSongLength
		     << "\tS: " << avgSongLength / avgQuietLength + avgSongLength << endl;
	}

	void summary() {
		cout << time << "\t" << quietTime << "\t"
		     <<  melodiousTime << "\t"
		     << squawkyTime << "\t"
		      << perfectSongs/totalSongs  << "\t" << perfectSongs << endl;
	}

public:
	BLAN(int numberBudgies) : numberBudgies(numberBudgies) {}

	BLAN(int numberBudgies, float avgSongLength, float avgSilentLength) : numberBudgies(numberBudgies),
	                                                                      avgSongLength(avgSongLength),
	                                                                      avgQuietLength(avgSilentLength) {}

	virtual ~BLAN() {
		birds.clear();
	}


	pair<float, int> simulate() {
//		printInputs();
		for (int i = 0; i < numberBudgies; ++i) {
			birds.emplace_back(avgSongLength, avgQuietLength);
			birds.back().setEventTimes(0);
		}
		for (auto &bird : birds) {
			if (bird.isSing()) {
				singingCounter++;
			}
		}
		birds.sort();

		while (time < MonthMinutes) {
			Budgie bird = birds.front();
			birds.pop_front();

			float currTime = bird.getEndTime();
			float temp;
			if (singingCounter == 0) {
				temp = currTime - time;
				quietTime = quietTime + temp;
			} else if (singingCounter >= 2) {
				temp = currTime - time;
				squawkyTime = squawkyTime + temp;
				totalSongs++;
			} else if (singingCounter == 1) {
				temp = currTime - time;
				melodiousTime = melodiousTime + temp;
				if (time == bird.getStartTime()) {
					perfectSongs++;
				}
				totalSongs++;
			} else {
				cerr << "Error may occured counting singing\n";
			}

			if (bird.isSing()) {
				bird.doQuiet(avgQuietLength);
				singingCounter--;
			} else {
				bird.doSong(avgSongLength);
				singingCounter++;
			}
			time = currTime;
			bird.setEventTimes(time);
			birds.insert(lower_bound(birds.begin(), birds.end(), bird), bird);
		}

		summary();
		pair<float, int> p1(melodiousTime, perfectSongs);
		return p1;
	}


};

int main(int argc, char **argv) {
	int numberBudgies;
	float avgSongLength = 3.0;
	float avgPercentSilent = 30.0;


	if (argc == 1) {
#ifdef DifferentSongLength
		cout << "alt input: Budgies, Song Length, Quiet Length, Iterations" << endl;
		pair<float, int> maxMelody(0, 0);
		pair<int, int> maxPerfect(0, 0);
		for (float j = 1; j < 5; ++j) {
			cout << j <<"SongLength\tTotaltime\t"  << "Idle time\t" << "Melodious time\t" << "Squawky time\t" << "%Perfect Songs\t"<< "#Perfect Songs\t"<<endl;
			for (float i = 1; i < 30; ++i) {
				cout << i << "\t";
				BLAN blan(i,j,30);
				blan.simulate();
			}

		}
#endif
#ifdef Regular
		cout << "Birds\tTotaltime\t"  << "Idle time\t" << "Melodious time\t" << "Squawky time\t" << "%Perfect Songs\t"<< "#Perfect Songs\t"<<endl;
		for (int k = 1; k < 30; ++k) {
			cout << k << "\t";
			BLAN blan(k);
			blan.simulate();
		}
#endif

	} else {
		numberBudgies = stoi(argv[2]);
		avgSongLength = stof(argv[3]);
		avgPercentSilent = stof(argv[4]);
		BLAN blan(numberBudgies, avgSongLength, avgPercentSilent);
		blan.simulate();
	}

}

/***********************************************************************/
/*                 RANDOM NUMBER GENERATION STUFF                      */
/***********************************************************************/
//Via Carey Williamson

/* Parameters for random number generation. */
#define MAX_INT 2147483647       /* Maximum positive integer 2^31 - 1 */

/* Generate a random floating point value uniformly distributed in [0,1] */
float Uniform01() {
	float randnum;

	/* get a random positive integer from random() */
	randnum = (float) 1.0 * random();

	/* divide by max int to get something in 0..1  */
	randnum = (float) randnum / (1.0 * MAX_INT);

	return (randnum);
}

/* Generate a random floating point number from an exponential    */
/* distribution with mean mu.                                     */

float Exponential(float mu) {
	float randnum, ans;

	randnum = Uniform01();
	ans = -(mu) * log(randnum);

	return (ans);
}