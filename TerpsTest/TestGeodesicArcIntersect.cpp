/**	\file TestGeodesicArcIntersect.cpp
*	\brief 
*/

/****************************************************************************/
/*	TestGeodesicArcIntersect.cpp											*/
/****************************************************************************/
/*                                                                          */
/*  Copyright 2008 - 2010 Paul Kohut                                        */
/*  Licensed under the Apache License, Version 2.0 (the "License"); you may */
/*  not use this file except in compliance with the License. You may obtain */
/*  a copy of the License at                                                */
/*                                                                          */
/*  http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                          */
/*  Unless required by applicable law or agreed to in writing, software     */
/*  distributed under the License is distributed on an "AS IS" BASIS,       */
/*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or         */
/*  implied. See the License for the specific language governing            */
/*  permissions and limitations under the License.                          */
/*                                                                          */
/****************************************************************************/

//#include "stdafx.h"
#include <string>
#include <fstream>
#include "LatLongConversions.h"
#include "..\GeoFormulas\Conversions.h"
#include "..\GeoFormulas\GeoFormulas.h"
#include <boost/regex.hpp>

using namespace boost;
using namespace GeoCalcs;
using namespace std;

bool ParseTestGeodesicArcIntersect(string sString)
{
	bool bPassed = true;
	TrimWhitespace(sString);
	string soTestId, soGeodesicStartLat, soGeodesicStartLon;
	string soGeodesicAzimuth, soArcCenterLat, soArcCenterLon;
	string soArcRadius, soIntersection1Lat, soIntersection1Lon;
	string soIntersection2Lat, soIntersection2Lon;

	try
	{
		regex_constants::syntax_option_type flags =  regex_constants::icase | regex_constants::perl;

		string sRxPat = "([a-z]+|[A-Z]+\\d+)[,]";
		sRxPat += "([0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[NS])[,]([0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[WE])[,]";
		sRxPat += "([-+]?[0-9]*[.]?[0-9]+)[,]";
		sRxPat += "([0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[NS])[,]([0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[WE])[,]";
		sRxPat += "([-+]?[0-9]*[.]?[0-9]+)[,]";
		sRxPat += "([N/A]+|[0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[NS])[,]([N/A]+|[0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[WE])[,]";
		sRxPat += "([N/A]+|[0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[NS])[,]([N/A]+|[0-9]*[:][0-9]*[:][0-9]*[.][0-9]*[WE])";

		regex pat(sRxPat, flags);

		int const sub_matches[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, };
		sregex_token_iterator it(sString.begin(), sString.end(), pat, sub_matches);
		if(it != sregex_token_iterator())
		{
			soTestId = *it++;
			soGeodesicStartLat = *it++;
			soGeodesicStartLon = *it++;
			soGeodesicAzimuth = *it++;
			soArcCenterLat = *it++;
			soArcCenterLon = *it++;
			soArcRadius = *it++;
			soIntersection1Lat = *it++;
			soIntersection1Lon = *it++;
			soIntersection2Lat = *it++;
			soIntersection2Lon = *it++;

		}
	}
	catch(regex_error & e)
	{
		cout << "\n" << e.what();
		return false;
	}

	LLPoint geodesicStart(Deg2Rad(ParseLatitude(soGeodesicStartLat)), Deg2Rad(ParseLongitude(soGeodesicStartLon)));
	LLPoint arcCenter(Deg2Rad(ParseLatitude(soArcCenterLat)), Deg2Rad(ParseLongitude(soArcCenterLon)));
	double geodesicAzimuth = Deg2Rad(atof(soGeodesicAzimuth.c_str()));
	double arcRadius = NmToMeters(atof(soArcRadius.c_str()));
	LLPoint intersection1;
	LLPoint intersection2;
	if(soIntersection1Lat != "N/A")
	{
		intersection1.Set(Deg2Rad(ParseLatitude(soIntersection1Lat)), Deg2Rad(ParseLongitude(soIntersection1Lon)));
		intersection2.Set(Deg2Rad(ParseLatitude(soIntersection2Lat)), Deg2Rad(ParseLongitude(soIntersection2Lon)));		
	}

	LLPoint intPtC1, intPtC2;
	int nIndex = GeodesicArcIntercept(geodesicStart, geodesicAzimuth, arcCenter, arcRadius, intPtC1, intPtC2, Tol());
	if(nIndex < 1)
	{
		if(soIntersection1Lat != "N/A")
			bPassed = false;

	} else {
		if(IsApprox(intPtC1.latitude, Deg2Rad(ParseLatitude(soIntersection2Lat)), Deg2Rad(0.1)) &&
			IsApprox(intPtC1.longitude, Deg2Rad(ParseLongitude(soIntersection2Lon)), Deg2Rad(0.1)) )
		{
			LLPoint pt = intPtC1;
			intPtC1 = intPtC2;
			intPtC2 = pt;
		}

		string sIntPt1Lat = ConvertLatitudeDdToDms(Rad2Deg(intPtC1.latitude));
		string sIntPt1Lon = ConvertLongitudeDdToDms(Rad2Deg(intPtC1.longitude));
		string sIntPt2Lat = ConvertLatitudeDdToDms(Rad2Deg(intPtC2.latitude));
		string sIntPt2Lon = ConvertLongitudeDdToDms(Rad2Deg(intPtC2.longitude));

		if(sIntPt1Lat.compare(soIntersection1Lat) != 0)
		{
			double dLat = Deg2Rad(ParseLatitude(soIntersection1Lat));
			if(IsApprox(dLat, intPtC1.latitude, 1e-10))
			{
				cout << "\n" << soTestId << " within rounding tolerance of 1e-10: Input Intersection 1 latitude: " << soIntersection1Lat << "  calced: " << sIntPt1Lat;
			} else {
				cout << "\n" << soTestId << " failed: Expected Intersection 1 latitude: " << soIntersection1Lat << "  calced: " << sIntPt1Lat;
				bPassed = false;
			}
		}

		if(sIntPt1Lon.compare(soIntersection1Lon) != 0)
		{
			double dLon = Deg2Rad(ParseLatitude(soIntersection1Lon));
			if(IsApprox(dLon, intPtC1.longitude, 1e-10))
			{
				cout << "\n" << soTestId << " within rounding tolerance of 1e-10: Input Intersection 1 longitude: " << soIntersection1Lon << "  calced: " << sIntPt1Lon;
			} else {
				cout << "\n" << soTestId << " failed: Expected Intersection 1 longitude: " << soIntersection1Lon << "  calced: " << sIntPt1Lon;
				bPassed = false;
			}
		}

		if(sIntPt2Lat.compare(soIntersection2Lat) != 0)
		{
			double dLat = Deg2Rad(ParseLatitude(soIntersection2Lat));
			if(IsApprox(dLat, intPtC2.latitude, 1e-10))
			{
				cout << "\n" << soTestId << " within rounding tolerance of 1e-10: Input Intersection 2 latitude: " << soIntersection2Lat << "  calced: " << sIntPt2Lat;
			} else {
				cout << "\n" << soTestId << " failed: Expected Intersection 2 latitude: " << soIntersection2Lat << "  calced: " << sIntPt2Lat;
				bPassed = false;
			}
		}

		if(sIntPt2Lon.compare(soIntersection2Lon) != 0)
		{
			double dLon = Deg2Rad(ParseLatitude(soIntersection2Lon));
			if(IsApprox(dLon, intPtC2.longitude, 1e-10))
			{
				cout << "\n" << soTestId << " within rounding tolerance of 1e-10: Input Intersection 2 longitude: " << soIntersection2Lon << "  calced: " << sIntPt2Lon;
			} else {
				cout << "\n" << soTestId << " failed: Expected Intersection 2 longitude: " << soIntersection2Lon << "  calced: " << sIntPt2Lon;
				bPassed = false;
			}
		}

	}



	return bPassed;	
}

int TestGeodesicArcIntersect(const string & sFilePath)
{
	ifstream infile;
	infile.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
	int nCount = 0;
	int nCommentCount = 0;
	bool bPassed = true;
	try
	{
		string sLine;
		infile.open(sFilePath.c_str(), ifstream::in);

		while(!infile.eof())
		{
			getline(infile, sLine);			
			if(sLine.at(0) == '#')
			{
				nCommentCount++;
			}
			else
			{
				if(!ParseTestGeodesicArcIntersect(sLine))
					bPassed = false;
				nCount++;
			}
		}
		infile.close();
		return bPassed;
	}

	catch(ifstream::failure e)
	{
		int nError = -99;
		// Per C++ standards for ifstream::failbit with global function getline
		// No characters were extracted because the end was prematurely found.Notice
		// that some eofbit cases will also set failbit.
		// In this case the end of the file is read and causes both flags to be raised,
		// so this presumably means all the data has been read correctly.
		if((infile.rdstate() & ifstream::failbit) && (infile.rdstate() & ifstream::eofbit) != 0)
			nError = bPassed;
		else if((infile.rdstate() & ifstream::failbit) != 0)
			nError = -1;
		else if((infile.rdstate() & ifstream::badbit) != 0)
			nError = -2;
		else if((infile.rdstate() & ifstream::eofbit) != 0)
			nError = -3;
		if(infile.is_open())
			infile.close();
		return nError;
	}
}