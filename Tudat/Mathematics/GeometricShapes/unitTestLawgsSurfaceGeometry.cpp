/*    Copyright (c) 2010-2012 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      110206    D. Dirkx          First version of file.
 *      110208    D. Dirkx          Finalized for code check.
 *      110212    J. Melman         Fixed many minor formatting issues.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *
 *    References
 *      Craidon, C.B. A Desription of the Langley Wireframe Geometry Standard
 *          (LaWGS) format, NASA TECHNICAL MEMORANDUM 85767.
 *
 */

#include <cmath>
#include <iostream>
#include <Eigen/Core>
#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>
#include "Tudat/Mathematics/GeometricShapes/lawgsPartGeometry.h"
#include "Tudat/Mathematics/GeometricShapes/sphereSegment.h"

using tudat::mathematics::PI;

//! Test implementation of Lawgs surface geometry.
int main( )
{
    // Using declarations.
    using namespace tudat;

    // Test result initialised to false.
    bool isLawgsSurfaceGeometryBad = false;

    // Create a full sphere as test geometry.
    SphereSegment sphere;
    double sphereRadius = 2.0;
    sphere.setRadius( sphereRadius );
    sphere.setMaximumAzimuthAngle( 2.0 * PI );
    sphere.setMinimumAzimuthAngle( 0.0 );
    sphere.setMaximumZenithAngle( PI );
    sphere.setMinimumZenithAngle( 0.0 );

    // Create a Lawgs mesh of the sphere.
    LawgsPartGeometry lawgsSurface;
    int numberOfLines = 21;
    int numberOfPoints = 21;
    lawgsSurface.setMesh( &sphere, numberOfLines, numberOfPoints );

    // Retrieve the total surface area and check if it is sufficiently close
    // to the expected value.
    double totalArea_ = lawgsSurface.getTotalArea( );

    if ( std::fabs( totalArea_ - 4.0 * PI * ( std::pow( sphereRadius, 2.0 ) ) ) > 0.6 )
    {
        std::cerr << "Total mesh area does not match sphere area sufficiently well." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Test if number of lines on mesh is correct.
    if ( lawgsSurface.getNumberOfLines( ) != numberOfLines )
    {
        std::cerr << " Number of lines in mesh incorrect." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Test if number of points per line on mesh is correct.
    if ( lawgsSurface.getNumberOfPoints( ) != numberOfPoints )
    {
        std::cerr << " Number of points in mesh is incorrect." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Set part name.
    std::string partName_ = "sphere";
    lawgsSurface.setName( partName_ );

    // Test if part name is properly retrieved.
    if ( lawgsSurface.getName ( ) != partName_ )
    {
        std::cerr << " Error in part name of mesh." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Retrieve normal and centroid for panel 0, 0.
    Eigen::Vector3d testNormal_ = lawgsSurface.getPanelSurfaceNormal( 0, 0 );
    Eigen::Vector3d testCentroid_ = lawgsSurface.getPanelCentroid( 0, 0 );

    // Test whether centroid and normal are collinear for panel 0, 0.
    if ( std::fabs( testCentroid_.normalized( ).dot( testNormal_.normalized( ) ) - 1.0 ) > 1.0e-5 )
    {
        std::cerr << "Normal and centroid of sphere segment mesh not collinear." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Test if the position of the x- and y-coordinate of panel 0, 0 is correct.
    if ( std::fabs( std::atan( testCentroid_.y( ) / testCentroid_.x( ) ) - PI / 20.0 ) >
         std::numeric_limits< double >::epsilon( ) )
    {
        std::cerr << "x- and y-coordinate of centroid of panel 0, 0 of "
                  << "sphere mesh is incorrect." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Return test result.
    // If test is succesful return 0, if test fails, return 1.
    if ( isLawgsSurfaceGeometryBad )
    {
        std::cerr << "testLawgsSurfaceGeometry failed!" << std::endl;
    }

    return  isLawgsSurfaceGeometryBad;
}