#include "catch.hpp"

#include <PropRoom3D/Node/Prop/Prop.h>

#include <PropRoom3D/Node/Prop/Material/Metal.h>

#include <PropRoom3D/Node/Prop/Surface/Surface.h>
#include <PropRoom3D/Node/Prop/Surface/Sphere.h>
#include <PropRoom3D/Node/Prop/Surface/Plane.h>
#include <PropRoom3D/Node/Prop/Ray/Raycast.h>
#include <PropRoom3D/Node/Prop/Ray/RayHitList.h>


using namespace prop3;

typedef std::shared_ptr<Surface> pSurf;
typedef std::shared_ptr<Material> pMat;

std::vector<RayHitReport*> memoryPool;


TEST_CASE("Shape/Surface/Planes/isIn",
          "Point position in combinations of the three planes")
{
    pSurf xPalne(
        Plane::plane(glm::dvec3(1, 0, 0), glm::dvec3(0)));
    pSurf yPalne(
        Plane::plane(glm::dvec3(0, 1, 0), glm::dvec3(0)));
    pSurf zPalne(
        Plane::plane(glm::dvec3(0, 0, 1), glm::dvec3(0)));


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
        Plane::plane(glm::dvec3(1, 0, 0), glm::dvec3(0)));
    pSurf yPalne(
        Plane::plane(glm::dvec3(0, 1, 0), glm::dvec3(0)));
    pSurf zPalne(
        Plane::plane(glm::dvec3(0, 0, 1), glm::dvec3(0)));


    SECTION("OR combination")
    {
        pSurf comb = xPalne | yPalne | zPalne;
        RayHitList reports(memoryPool);


        // Corner intersection
        reports.clear();
        Raycast cRay(Raycast::BACKDROP_DISTANCE, Raycast::FULLY_SPECULAR_ENTROPY, glm::dvec3(), glm::dvec3( 1, 1, 1), glm::dvec3(-1,-1,-1), pMat());
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
        Raycast lRay(glm::dvec3( 1, 1, 1), glm::dvec3(-1,-.75,-1));
        comb->raycast(lRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0, 0.25, 0));
        REQUIRE(reports[1].position == glm::dvec3(0, 0.25, 0));

        // Point intersection
        reports.clear();
        Raycast pRay(glm::dvec3( 1, 1, 2), glm::dvec3(-1,-.75,-1));
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
        Raycast cRay(glm::dvec3( 1,  1,  1), glm::dvec3(-1,  -1, -1));
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
        Raycast lRay(glm::dvec3( 0,  1,  1), glm::dvec3(-1,  -1, -1));
        comb->raycast(lRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].distance == 1.0);
        REQUIRE(reports[1].distance == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(-1.0, 0.0, 0));
        REQUIRE(reports[1].position == glm::dvec3(-1.0, 0.0, 0));

        // Point intersection
        reports.clear();
        Raycast pRay(glm::dvec3( 1,  1,  2), glm::dvec3(-1,  -1, -1));
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

    Raycast xRay(    glm::dvec3( -4,  0,  0), glm::dvec3(1,  0,  0));
    Raycast yNegXRay(glm::dvec3( -1,  0,  4), glm::dvec3(0,  0, -1));
    Raycast yPosXRay(glm::dvec3(  1,  0, -4), glm::dvec3(0,  0,  1));


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
