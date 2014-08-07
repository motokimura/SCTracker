/********************************************************************************
* File Name : despatch.cpp
* Title     : 
* Programmer: Motki Kimura
* Belonging : 
* Date      : 2014.8.7
* Language  : C++
*********************************************************************************
* class to calculate the mode of DESPATCH
*
********************************************************************************/
#include "despatch.h"

DespatchTracker:: DespatchTracker (void) : SpacecraftTracker (), departureMjd_ (0.0)
{
	
}

DespatchTracker:: DespatchTracker (double departureMjd, SCDRec const& scd) : SpacecraftTracker (scd), departureMjd_ (departureMjd)
{
	
}

DespatchTracker:: ~DespatchTracker (void)
{
	
}

void DespatchTracker:: setDepartureTime (double departureMjd)
{
	departureMjd_ = departureMjd;
}

void DespatchTracker:: getDepartureTime (double* departureMjd) const
{
	*departureMjd = departureMjd_;
}

void DespatchTracker:: getDespatchMode (string* mode) const
{
	const double PeriodMorse = 8.0 * 3600.0;
	const double PeriodBaudot = 77.0 * 3600.0;
	const double PeriodBeacon = (7.0 * 24.0) * 3600.0;
	
	double secondsFromDeparture;
	calcSecondsFromDeparture (&secondsFromDeparture);
	
	if (secondsFromDeparture > PeriodBeacon) {
		*mode = "radio_stop";
	}
	else if (secondsFromDeparture > PeriodBaudot) {
		*mode = "beacon";
	}
	else if (secondsFromDeparture > PeriodMorse) {
		*mode = "baudot";
	}
	else if (secondsFromDeparture > 0.0) {
		*mode = "morse";
	}
	else {
		*mode = "not_defined";
	}
}

void DespatchTracker:: test (double unixtime_s, double unixtime_e, double outputDt)
{
	if (unixtime_s > unixtime_e) {
		cout << "[ERROR] unixtime_e must be bigger than unixtime_s" << endl;
		return;
	}
	
	if (outputDt <= 0.0) {
		cout << "[ERROR] outputDt must bigger than zero." << endl;
		return;
	}
	
	// set departure time
	const double DepartureMjd = 56992.264444;
	setDepartureTime (DepartureMjd);
	
	// set spacecraft orbit information and parameter
	SCDRec scd;
	scd.orbitInfo.epochMjd = DepartureMjd;
	scd.orbitInfo.positionEci[0] = 10718921.0;
	scd.orbitInfo.positionEci[1] =   747344.0;
	scd.orbitInfo.positionEci[2] = -1050332.0;
	scd.orbitInfo.velocityEci[0] =  5616.853;
	scd.orbitInfo.velocityEci[1] =  6764.471;
	scd.orbitInfo.velocityEci[2] = -4193.746;
	scd.param.ballisticCoeff = 150.0;
	scd.param.transmitterFrequency = 437.325e6f;
	setSpacecraftInfo (scd);
	
	// variables
	double distance;
	string mode;
	
	double utime = unixtime_s;
	
	cout << "unixtime, distance, mode" << endl;
	do {
		setTargetTime (utime);
		
		getDistanceEarthCentered (&distance);
		getDespatchMode (&mode);
		
		cout << setprecision (10);
		cout << utime << ",";
		cout << distance << ",";
		cout << mode;
		cout << endl;
		
		utime += outputDt;
		
	} while (utime < unixtime_e);
}

void DespatchTracker:: calcSecondsFromDeparture (double* secondsFromDeparture) const
{
	double epochMjd;
	getEpochTime (&epochMjd);
	double epochUnixtime;
	tf:: convertMjdToUnixtime (&epochUnixtime, epochMjd);
	
	double departureUnixtime;
	tf:: convertMjdToUnixtime (&departureUnixtime, departureMjd_);
	
	double secondsFromEpoch;
	getSecondsFromEpoch (&secondsFromEpoch);
	
	*secondsFromDeparture = (epochUnixtime - departureUnixtime) + secondsFromEpoch;
}