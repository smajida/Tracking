#ifndef _IDENTIFICATION_H_
#define _IDENTIFICATION_H_

#include "../Frame.h"
#include <list>
#include <cmath>
#include <queue>
#include <algorithm>

namespace Identification
{
	
	class ProbabilityContainer;
	enum Algorithm
	{
		NearestFit = 0,
		Ultimate
	};

	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
	float errorFunction(Object * a, Object * b);


	//////////     Module     ///////////
	/////////////////////////////////////

	class Identifier
	{
	public:
		Identifier() {uniqueIDPool = 1; algorithm = &Identifier::algorithm_nearestFit;}
		void identify(std::list<Frame> & frames);
		
		void init(Algorithm algorithmName);
		
	private:
		Algorithm algorithmName;
		void (Identifier::*algorithm)(std::list<Frame> & frames);
		
		int uniqueIDPool;
		int newID() {return uniqueIDPool++;}
		
		void algorithm_nearestFit(std::list<Frame> & frames);
		void algorithm_ultimate(std::list<Frame> & frames);
	};
	

	//////////  Internal structures  ///////////
	////////////////////////////////////////////

	class Error
	{
	public:
		Object * current;
		Object * previous;
		int previousIndex;
		float error;
		Error(Object * current, Object * previous, int previousIndex, float error)
			: current(current),previous(previous), previousIndex(previousIndex), error(error) {}
		bool operator<(const Error & e) { return error < e.error; }
	};	



	////////// TEST GENERATION ///////////
	//////////////////////////////////////

	void generate_testdata(std::list<Frame> & frameList, std::string test = "simple1");
	float randf();
}

#endif