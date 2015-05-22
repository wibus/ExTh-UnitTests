#include "catch.hpp"

#include <PropRoom3D/Prop/Prop.h>

#include <PropRoom3D/Prop/Material/Chrome.h>

#include <PropRoom3D/Prop/ImplicitSurface/ImplicitSurface.h>
#include <PropRoom3D/Prop/ImplicitSurface/Sphere.h>
#include <PropRoom3D/Prop/ImplicitSurface/Plane.h>
#include <PropRoom3D/Prop/Ray/Ray.h>
#include <PropRoom3D/Prop/Ray/RayHitReport.h>


using namespace prop3;

typedef std::shared_ptr<ImplicitSurface> pSurf;


TEST_CASE("Shape/Surface/Planes/isIn",
          "Point position in combinations of the three planes")
{
    pSurf xPalne(
        new Plane(glm::dvec3(1, 0, 0), glm::dvec3(0)));
    pSurf yPalne(
        new Plane(glm::dvec3(0, 1, 0), glm::dvec3(0)));
    pSurf zPalne(
        new Plane(glm::dvec3(0, 0, 1), glm::dvec3(0)));


    SECTION("OR combination")
    {
        pSurf comb = xPalne | yPalne | zPalne;


        // Negative z quadrans
        REQUIRE(comb->isIn( 1,  1, -1) == EPointPosition::IN);
        REQUIRE(comb->isIn(-1,  1, -1) == EPointPosition::IN);
        REQUIRE(comb->isIn(-1, -1, -1) == EPointPosition::IN);
        REQUIRE(comb->isIn( 1, -1, -1) == EPointPosition::IN);

        // Poistive z quadrans
        REQUIRE(comb->isIn(-1,  1,  1) == EPointPosition::IN);
        REQUIRE(comb->isIn(-1, -1,  1) == EPointPosition::IN);
        REQUIRE(comb->isIn( 1, -1,  1) == EPointPosition::IN);

        // Only out quadran
        REQUIRE(comb->isIn( 1,  1,  1) == EPointPosition::OUT);
    }

    SECTION("AND combination")
    {
        pSurf comb = xPalne & yPalne & zPalne;


        // Only in quadran
        REQUIRE(comb->isIn(-1, -1, -1) == EPointPosition::IN);

        // Out quadrans
        // Positive z quadrans
        REQUIRE(comb->isIn( 1,  1,  1) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1,  1,  1) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1, -1,  1) == EPointPosition::OUT);
        REQUIRE(comb->isIn( 1, -1,  1) == EPointPosition::OUT);

        // Positive z quadrans
        REQUIRE(comb->isIn( 1,  1, -1) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1,  1, -1) == EPointPosition::OUT);
        REQUIRE(comb->isIn( 1, -1, -1) == EPointPosition::OUT);
    }
}

TEST_CASE("Shape/Surface/Planes/Raycast",
          "Raycasts in combinations of the three planes")
{
    pSurf xPalne(
        new Plane(glm::dvec3(1, 0, 0), glm::dvec3(0)));
    pSurf yPalne(
        new Plane(glm::dvec3(0, 1, 0), glm::dvec3(0)));
    pSurf zPalne(
        new Plane(glm::dvec3(0, 0, 1), glm::dvec3(0)));


    SECTION("OR combination")
    {
        pSurf comb = xPalne | yPalne | zPalne;
        std::vector<RayHitReport> reports;


        // Corner intersection
        reports.clear();
        Ray cRay(glm::dvec3( 1, 1, 1), glm::dvec3(-1,-1,-1));
        comb->raycast(cRay, reports);
        REQUIRE(reports.size() == 3);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 1.0);
        REQUIRE(reports[2].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0));
        REQUIRE(reports[1].position == glm::dvec3(0));
        REQUIRE(reports[2].position == glm::dvec3(0));

        // Line intersection
        reports.clear();
        Ray lRay(glm::dvec3( 1, 1, 1), glm::dvec3(-1,-.75,-1));
        comb->raycast(lRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0, 0.25, 0));
        REQUIRE(reports[1].position == glm::dvec3(0, 0.25, 0));

        // Point intersection
        reports.clear();
        Ray pRay(glm::dvec3( 1, 1, 2), glm::dvec3(-1,-.75,-1));
        comb->raycast(pRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0, 0.25, 1));
    }

    SECTION("AND combination")
    {
        pSurf comb = xPalne & yPalne & zPalne;
        std::vector<RayHitReport> reports;


        // Corner intersection
        reports.clear();
        Ray cRay(glm::dvec3( 1,  1,  1), glm::dvec3(-1,  -1, -1));
        comb->raycast(cRay, reports);
        REQUIRE(reports.size() == 3);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 1.0);
        REQUIRE(reports[2].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0));
        REQUIRE(reports[1].position == glm::dvec3(0));
        REQUIRE(reports[2].position == glm::dvec3(0));

        // Line intersection
        reports.clear();
        Ray lRay(glm::dvec3( 0,  1,  1), glm::dvec3(-1,  -1, -1));
        comb->raycast(lRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(-1.0, 0.0, 0));
        REQUIRE(reports[1].position == glm::dvec3(-1.0, 0.0, 0));

        // Point intersection
        reports.clear();
        Ray pRay(glm::dvec3( 1,  1,  2), glm::dvec3(-1,  -1, -1));
        comb->raycast(pRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].distance == 2.0);
        REQUIRE(reports[0].position == glm::dvec3(-1, -1, 0));
    }
}

TEST_CASE("Shape/Surface/Spheres/isIn",
          "Point position in combinations of two spheres")
{
    pSurf negSphere(
        new Sphere(glm::dvec3(-1, 0, 0), 2.0));
    pSurf posSphere(
        new Sphere(glm::dvec3(1,  0, 0), 2.0));

    SECTION("OR combination")
    {
        pSurf comb = negSphere | posSphere;

        REQUIRE(comb->isIn(-2,  0, -2) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1,  0, -1) == EPointPosition::IN);
        REQUIRE(comb->isIn( 0,  0,  0) == EPointPosition::IN);
        REQUIRE(comb->isIn( 1,  0,  1) == EPointPosition::IN);
        REQUIRE(comb->isIn( 2,  0,  2) == EPointPosition::OUT);

    }

    SECTION("AND combination")
    {
        pSurf comb = negSphere & posSphere;

        REQUIRE(comb->isIn(-2,  0, -2) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1,  0, -1) == EPointPosition::OUT);
        REQUIRE(comb->isIn( 0,  0,  0) == EPointPosition::IN);
        REQUIRE(comb->isIn( 1,  0,  1) == EPointPosition::OUT);
        REQUIRE(comb->isIn( 2,  0,  2) == EPointPosition::OUT);
    }
}

TEST_CASE("Shape/Surface/Spheres/Raycast",
          "Raycasts in combinations of two spheres")
{
    pSurf negSphere(
        new Sphere(glm::dvec3(-1, 0, 0), 2.0));
    pSurf posSphere(
        new Sphere(glm::dvec3(1,  0, 0), 2.0));

    Ray xRay(    glm::dvec3( -4,  0,  0), glm::dvec3(1,  0,  0));
    Ray yNegXRay(glm::dvec3( -1,  0,  4), glm::dvec3(0,  0, -1));
    Ray yPosXRay(glm::dvec3(  1,  0, -4), glm::dvec3(0,  0,  1));


    SECTION("OR combination")
    {
        pSurf comb = negSphere | posSphere;
        std::vector<RayHitReport> reports;

        // Aligned with spheres and x axis
        reports.clear();
        comb->raycast(xRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 7.0);
        REQUIRE(reports[0].position == glm::dvec3(-3, 0, 0));
        REQUIRE(reports[1].position == glm::dvec3( 3, 0, 0));

        // Aligned with y axis and on negative x sphere
        reports.clear();
        comb->raycast(yNegXRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 2.0);
        REQUIRE(reports[1].distance == 6.0);
        REQUIRE(reports[0].position == glm::dvec3(-1, 0, 2));
        REQUIRE(reports[1].position == glm::dvec3(-1, 0,-2));

        // Aligned with y axis and under positive x sphere
        reports.clear();
        comb->raycast(yPosXRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 2.0);
        REQUIRE(reports[1].distance == 6.0);
        REQUIRE(reports[0].position == glm::dvec3( 1, 0,-2));
        REQUIRE(reports[1].position == glm::dvec3( 1, 0, 2));
    }

    SECTION("AND combination")
    {
        pSurf comb = negSphere & posSphere;
        std::vector<RayHitReport> reports;

        // Aligned with spheres and x axis
        reports.clear();
        comb->raycast(xRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 5.0);
        REQUIRE(reports[1].distance == 3.0);
        REQUIRE(reports[0].position == glm::dvec3( 1, 0, 0));
        REQUIRE(reports[1].position == glm::dvec3(-1, 0, 0));

        // Aligned with y axis and on negative x sphere
        reports.clear();
        comb->raycast(yNegXRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].distance == 4.0);
        REQUIRE(reports[0].position == glm::dvec3(-1, 0, 0));

        // Aligned with y axis and under positive x sphere
        reports.clear();
        comb->raycast(yPosXRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].distance == 4.0);
        REQUIRE(reports[0].position == glm::dvec3(1, 0, 0));
    }
}
