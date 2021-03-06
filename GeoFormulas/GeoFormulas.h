/**	\file GeoCalcs.h
*	\brief 
*/

/****************************************************************************/
/*	GeoCalcs.h																*/
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

#pragma once
#include "Geolib.h"
#include "VectorMath.h"

 namespace GeoCalcs {

	/**
	*	\remark Order 8260.54A, Appendix 2, 2.1.1
	*/
    GEOAPI
    typedef struct LLPOINT {
        double latitude;
        double longitude;
        LLPOINT()
			: latitude(0.0), longitude(0.0) { }

        LLPOINT(double dLat, double dLon)
			: latitude(dLat), longitude(dLon) { }
        
		void Set(double dLat, double dLon)
		{
			latitude = dLat;
			longitude = dLon;
		}


    } LLPoint;

    
	/**
	*	\brief Locus of Points Relative to a Geodesic
	*	\remark Order 8260.54A, Appendix 2, 2.1.4
	*/
    GEOAPI
    typedef struct LOCUS {
        LLPoint geoStart;	/**< start point of geodesic */
        LLPoint geoEnd;		/**< end point of geodesic */
        LLPoint locusStart;	/**< start point of locus */
        LLPoint locusEnd;	/**< end point of locus */
        double startDist;	/**< distance from geodesic to locus at geoStart */
        double endDist;		/**< distance from geodesic to locus at geoEnd
							*
							* if startDist == endDist, the locus is parallel to the geodesic<br>							
							* if startDist != endDist, the locus is splayed.<br>
							* if dist is positive then locus lies to the right of geodesic<br>
							* if dist is negative then locus lies to the left of geodesic
							*/

        int lineType;		/**< 0 = the locus exists only between geoStart and geoEnd
							*
							* 1 = the locus begins at geoStart but extends beyond geoEnd<br>
							* 2 = the locus extends beyond both geoStart and geoEnd
							*/
    } Locus ;

	/**
	*	\brief Parameter used by DistVincenty function
	*
	*	When DistVincenty returns true, INVERSE_RESULT referenced
	*	parameter to DistVincenty will be be updated.
	*/
    GEOAPI
    typedef struct INVERSE_RESULT {
        double azimuth;			/**< calculated azimuth in radians */
        double reverseAzimuth;	/**< calculated reverse azimuth in radians */
        double distance;		/**< calculated distance in meters */
    } InverseResult;

	// returns true if value was calculated (PtIsOnGeodesicResult structure
	// will be updated with computed values), PtIsOnGeodesicResult.result == true
	// if Point is on Geodesic, false otherwise.
	// return false if there was a problem in performing calculations (main DistVincenty())


	/**
	*	\brief Structured used by the PtIsOnGeodesic function
	*
	*	When PtIsOnGeodesic returns true, then referenced instance of
	*	PTISONGEODESICV_RESULT is updated with computed values.
	*/
    GEOAPI
    typedef struct PTISONGEODESIC_RESULT {
        LLPoint geoStart;	/**< Geodetic coordinate of line start point */
        LLPoint geoEnd;		/**< Geodetic coordinate of line end point */
        LLPoint geoPt;		/**< Geodetic coordinate of point to test */
        int lengthCode;		/**< Integer that specifies extent of line<br>
							*	 0: geodesic exists only between geoStart and
							*		geoEnd.<br>
							*	 1: geodesic extends beyound geoEnt
							*/
        int result;			/**< Set true if point is on geodesic, false otherwise. */
    } PtIsOnGeodesicResult;

	/*
	*	\brief Ellipsoid Inverse Flattening constant
	*
	*	298.25722356366546517369570015525
	*	\remarks WGS84 Ellipsoid constant
	*/
//#define InverseFlattening 298.25722356366546517369570015525
    GEOAPI
    const inline double _stdcall InverseFlattening(void) { return 298.25722356366546517369570015525; }

	/*
	*	\brief Ellipsoid Flattening constant
	*
	*	Approximately 0.00335281
	*	\remarks WGS84 Ellipsoid constant
	*/
//#define Flattening 1.0 / InverseFlattening
    GEOAPI
    const inline double _stdcall Flattening(void) { return 1.0 / InverseFlattening(); }

	/*
	*	\brief Ellipsoid Semi Major Axis
	*
	*	6378137.0
	*	\remarks WGS84 Ellipsoid constant
	*/
//#define SemiMajorAxis 6378137.0
    GEOAPI
    const inline double _stdcall SemiMajorAxis(void) { return 6378137.0; }

	/*
	*	\brief Ellipsoid Semi Minor Axis
	*
	*	Approximately 6356752.314245
	*	\remarks WGS84 Ellipsoid constant
	*/
//#define SemiMinorAxis SemiMajorAxis * (1 - Flattening)
    GEOAPI
    const inline double _stdcall SemiMinorAxis(void) { return SemiMajorAxis() * (1 - Flattening()); }


	/*	\brief SphereRadius in meters	
	*
	*	WGS84 Ellipsoid constant. Calculated to large precision.
	*	\warning
	*	If a calculated sphere radius is used, such as sqrt(SemiMajorAxis *
	*	SemiMinorAxis) then FindLinearRoot may return a root of QNAN
	*	because x[0] == x[1]. Don't recall off the top which test suite
	*	causes this type of result, but using the constant of out 29
	*	places is safe. I'm just leaving it this way because that was
	*	what was calculated with the large precision calculator.
	*/
//#define SphereRadius 6367435.679716102288521224771085329133283527893713982476828820197
    GEOAPI
    const inline double _stdcall SphereRadius(void) { return 6367435.679716102288521224771085329133283527893713982476828820197; }
//	const double SphereRadius = sqrt(SemiMajorAxis * SemiMinorAxis);


	/*
	*	\brief 2 * PI
	*/
#define M_2PI M_PI * 2.0



	/*
	*	\brief Epsilon
	*	\returns double epsilon of 0.5e-15.
	*	\note Order 8260.54A Appendix 2, 3.3 Tolerances, states
	*	"Empirical studies have shown that eps = 0.5e-13 and
	*	tol = 1.0-e9 work well."
	*
	*	When implementing the TerpsTest validation application
	*	eps must be set to 0.5e-15 for all tests to pass. If
	*	set as stated in section 3.3 of 8260.54A then the
	*	Tangent Fixed Radius Arc and Locus Tan Fixed Radius Arc
	*	fails validation.
	*	
	*/
    GEOAPI
    inline double _stdcall Eps(void) { return 0.5e-15; }

	/*
	*	\brief Tolerance
	*	\returns double tolerance of 1.0e-9
	*	\note Order 8260.54A Appendix 2, 3.3 Tolerances, states
	*	"Empirical studies have shown that eps = 0.5e-13 and
	*	tol = 1.0-e9 work well."
	*
	*	Functions in the GeoFormulas that use different values
	*	will be clearly marked in the comments and the value
	*	used.
	*
	*	See file "Application test Results.txt" for information
	*	about tolerances used in the "TerpsTest" application.
	*	
	*/
    GEOAPI 
	inline double _stdcall Tol(void) { return 1.0e-9; }

	/*
	*	\brief The square of the ellipsoid eccentricity
	*/
    GEOAPI
    inline double _stdcall eSq(void) { return Flattening() * (2.0 - Flattening()); }

   	/*
	*	\brief Point Is on Geodesic Tolerance
	*	\note
	*	See T. Vincenty paper on accuracy, page 90 Survey Review Vol XXIII No. 176, April 1975
	*
	*	Note: PtIsOnGeodesic in 8260.54A uses 'tol' to determine if point is on geodesic or not.
	*	After much testing and reading I believe that tol in this case == 1e-3, which gives
	*	an accuracy of 1mm.  Further testing shows that tol can be pushed to .5e-3 and still
	*	pass the WGS84PtIsOnGeodesic Test Results presented on page A2-93 of Order 8260.54A.
	*	Change TolPtIsOnGeodesic to 1e-3 for the 1mm resolution if 0.5e-3 causes any QA issues.
	*/
    GEOAPI
	inline double _stdcall TolPtIsOnGeodesic(void) { return .5e-3; }



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	///////////////////// GeoFormulas Function Definitions ///////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

    GEOAPI
	double _stdcall PrimeVerticalCurvature(double dAngle);
    
    GEOAPI
	double _stdcall GeometricMeanMeridional(double dAngle);
	
    GEOAPI
	VMath::Vector3 _stdcall ECEF( const LLPoint & llPt, double dAltitude);

    GEOAPI
    bool _stdcall DistVincenty(const LLPoint & pt1, const LLPoint & pt2, InverseResult & result);

    GEOAPI
    LLPoint _stdcall DestVincenty(const LLPoint & pt, double brng, double dist);

    GEOAPI
    bool _stdcall PtIsOnGeodesic(const LLPoint & pt1, const LLPoint & pt2, const LLPoint & pt3, int lengthCode, PtIsOnGeodesicResult & result );

    GEOAPI
    void _stdcall FindLinearRoot(double *x, double *errArray, double & root);

    GEOAPI
    bool _stdcall PtIsOnArc( const LLPoint & llArcCenter, double dArcRadius,
        double dArcStartAzimuth, double dArcEndAzimuth, int nArcDirection,
        const LLPoint & llTestPt, int & bOnArc );

    GEOAPI
    double _stdcall GetArcExtent(double dStartCrs, double dEndCrs, const int nOrientation, double dTol);

    GEOAPI
    double _stdcall DiscretizedArcLength(const LLPoint & center, double dRadius,
        double dStartCrs, double dEndCrs,
        int nOrient, int nSegments, double dTol);

    GEOAPI
    double _stdcall DirectArcLength(const LLPoint & center, double dRadius, double dStartCrs,
        double dEndCrs, int nOrient, double dTol);

    GEOAPI
    double _stdcall DistToLocusD(const Locus & loc, double dDistance, double dEps);

    GEOAPI
    double _stdcall DistToLocusP(const Locus & loc, const LLPoint & geoPt, double dTol, double eps);

    GEOAPI
    LLPoint _stdcall PointOnLocusP(const Locus & loc, const LLPoint & geoPt, double dTol, double eps);

    GEOAPI
    bool _stdcall PtIsOnLocus(const Locus & loc, const LLPoint & testPt, LLPoint & projPt, double dTol);

    GEOAPI
    LLPoint _stdcall PerpIntercept(const LLPoint & llPt1, double dCrs13, const LLPoint & llPt2,
        double & dCrsFromPt, double & dDistFromPt, double dTol);

    GEOAPI
    double _stdcall SignAzimuthDifference(double az1, double az2);

    GEOAPI
    double _stdcall LocusCrsAtPoint(const Locus & locus, const LLPoint & testPt, LLPoint & geoPt, double & dPerpCrs, const double dTol);

    GEOAPI
    bool  _stdcall CrsIntersect(const LLPoint & llPt1, double az13,
        const LLPoint & llPt2, double az23, double dTol, LLPoint & llIntersect);

    GEOAPI
    bool _stdcall CrsIntersect(const LLPoint & llPt1, double az13,
        double & az31, double & dist13, const LLPoint & llPt2, double az23,
        double & az32, double & dist23, double dTol, LLPoint & llIntersect);

    GEOAPI
    int _stdcall ArcIntercept(const LLPoint & center1, double radius1,
        const LLPoint & center2, double radius2,
        LLPoint & intPtC1, LLPoint & intPtC2, double dTol);

    GEOAPI
    int _stdcall DistanceIntersection(const LLPoint & pt1, double dist13, const LLPoint & pt2, double dist23,
        LLPoint & intPtC1, LLPoint & intPtC2, double dTol);

    GEOAPI
    int _stdcall GeodesicArcIntercept(const LLPoint & pt1, double crs1,
        const LLPoint & center, double radius,
        LLPoint & intPtC1, LLPoint & intPtC2, double dTol);

    GEOAPI
    int _stdcall TangentFixedRadiusArc(const LLPoint & pt1, double crs12, const LLPoint & pt3,
        double crs3, double radius, int & dir,
        LLPoint & centerPt, LLPoint & tanPt1, LLPoint & tanPt2, double dTol);

    GEOAPI
    int _stdcall GeoLocusIntersect(const LLPoint & gStart, const LLPoint & gEnd, const Locus & loc, LLPoint & intersect,
        double dTol, double dEps);

    GEOAPI
    int _stdcall LocusArcIntersect(const Locus & locus, const LLPoint & arcCenter, double arcRadius,
        LLPoint & intersection1, LLPoint & intersection2, double dTol,
        double dEps);

    GEOAPI
    int _stdcall LocusIntersect(const Locus & loc1, const Locus & loc2, LLPoint & intersect,
        double dTol, double dEps);

    GEOAPI
    int _stdcall LocusTanFixedRadiusArc(const Locus & loc1, const Locus & loc2, double radius,
        int & dir, LLPoint & center, LLPoint & intersection1, LLPoint & intersection2,
        double dTol, double dEps);

    GEOAPI
    int _stdcall LocusPerpIntercept(const Locus & loc, const LLPoint & pt2, double & crsFromPt,
        double & distFromPt, LLPoint & intPt, double dTol);

    GEOAPI
    int _stdcall PointToArcTangents(const LLPoint & point, const LLPoint & center, double radius,
        LLPoint & tanPt1, LLPoint & tanPt2, double dTol);

    GEOAPI
    void _stdcall PerpTangentPoints(const LLPoint & lineStart, double crs,
        const LLPoint & center, double radius,
        LLPoint * linePts, LLPoint * tanPts, double dTol);

    GEOAPI
    double _stdcall Mod(double a, double b);

}