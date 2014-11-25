/********************************************************************************
* File Name : tracker.cpp
* Title     : 
* Programmer: Motki Kimura
* Belonging : 
* Date      : 2014.8.3
* Language  : C++
*********************************************************************************
* class to calculate the position & velocity of the spacecraft in geo coordinate
*
********************************************************************************/
#include "tracker.h"


SpacecraftTracker:: SpacecraftTracker (void) : SpacecraftCalculator (), unixtime_ (0.0)
{
	
}

SpacecraftTracker:: SpacecraftTracker (SCDRec const& scd) : SpacecraftCalculator (), unixtime_ (0.0)
{
	setSpacecraftInfo (scd);
}

SpacecraftTracker:: ~SpacecraftTracker (void)
{
	
}

void SpacecraftTracker:: setSpacecraftInfo (SCDRec const& scd)
{
	scd_ = scd;
	
	setSpacecraftOrbitInfo (scd_.orbitInfo.epochMjd, scd_.orbitInfo.positionEci, scd_.orbitInfo.velocityEci);
	setSpacecraftParams (scd_.param.ballisticCoeff);
}

void SpacecraftTracker:: getSpacecraftInfo (SCDRec *scd) const
{
	*scd = scd_;
}

int SpacecraftTracker:: setTargetTime (double unixtime)
{
	unixtime_ = unixtime;
	
	updateSpacecraftState ();
	
	return 0;
}

void SpacecraftTracker:: getTargetTime (double* unixtime) const
{
	*unixtime = unixtime_;
}

void SpacecraftTracker:: test (double unixtime_s, double unixtime_e, double outputDt)
{
	if (unixtime_s > unixtime_e) {
		cout << "[ERROR] unixtime_e must be bigger than unixtime_s" << endl;
		return;
	}
	
	if (outputDt <= 0.0) {
		cout << "[ERROR] outputDt must bigger than zero." << endl;
		return;
	}
	
	// set spacecraft orbit information and parameter
	SCDRec scd;
	scd.orbitInfo.epochMjd = 56992.264444;
	scd.orbitInfo.positionEci[0] = 10718921.0;
	scd.orbitInfo.positionEci[1] =   747344.0;
	scd.orbitInfo.positionEci[2] = -1050332.0;
	scd.orbitInfo.velocityEci[0] =  5616.853;
	scd.orbitInfo.velocityEci[1] =  6764.471;
	scd.orbitInfo.velocityEci[2] = -4193.746;
	scd.param.ballisticCoeff = 150.0;
	setSpacecraftInfo (scd);
	
	// variables
	double declination, ra, doppler_ratio, distance, speed;
	
	double utime = unixtime_s;
	
    const double Frequency = 437.325e6f;
    
	cout << "unixtime, x, y, z, u, v, w, declination, RA, doppler, distance, speed" << endl;
	do {
		setTargetTime (utime);
		
		getGeometryEarthCentered (&declination, &ra);
		getDopplerRatioEarthCentered (&doppler_ratio);
		getDistanceEarthCentered (&distance);
		getSpacecraftRelativeSpeed (&speed);
		
		cout << setprecision (10);
		cout << utime << ",";
		cout << declination << "," << ra << ",";
		cout << (doppler_ratio - 1.0) * Frequency << ",";
		cout << distance << ",";
		cout << speed;
		cout << endl;
		
		utime += outputDt;
		
	} while (utime < unixtime_e);
}

void SpacecraftTracker:: updateSpacecraftState (void)
{
	double period;
	calcIntegrationPeriod (&period);
    
    double sgn = 1.0;
    if (period < 0.0) {
        sgn = -1.0;
    }
    
    double distance;
    const double RangeNearEarth = 100000000.0;
    
    double dt;
    const double DtNearEarth = 1.0;
    const double DtFarFromEarth = 60.0;
	
	double t = 0.0;
	
    while (1) {
        
		getDistanceEarthCentered (&distance);
        if (distance < RangeNearEarth) {
            dt = sgn * DtNearEarth;   // dt should be small enough when the spacecraft is near Earth for the accuracy
        }
        else {
            dt  = sgn * DtFarFromEarth;
        }
    
		if (abs (t + dt) > abs (period)) {
			break;
		}
        
		integrateSpacecraftState (dt);
		t += dt;
	};
	integrateSpacecraftState (period - t);
}

void SpacecraftTracker:: calcIntegrationPeriod (double* period) const
{
	double epochMjd;
	getEpochTime (&epochMjd);
	double epochUnixtime;
	tf:: convertMjdToUnixtime (&epochUnixtime, epochMjd);
	
	double secondsFromEpoch;
	getSecondsFromEpoch (&secondsFromEpoch);
	
	double tFinal = unixtime_ - epochUnixtime;
	if (tFinal < 0.0) {
		cout << "[WARNING] Specified unixtime is smaller than the unixtime of the epoch" << endl;
		cout << "[WARNING] Specified unixtime: " << unixtime_ << ", Unixtime of the epoch: " << epochUnixtime << endl;
	}
	
	*period = tFinal - secondsFromEpoch;	// period of the numerical integration
}
