#include "catch.hpp"

#include <PropRoom3D/Shape/AbstractShape.h>
#include <PropRoom3D/Shape/SpaceEquation/SpaceEquation.h>
#include <PropRoom3D/Shape/SpaceEquation/QuadricEquation.h>

using namespace prop3;

TEST_CASE("Shape/SpaceEquation/Planes/isIn",
          "Point position in combinations of the three planes")
{
    std::shared_ptr<SpaceEquation> xPalne(
        new PlaneEquation(glm::dvec3(1, 0, 0), glm::dvec3(0)));
    std::shared_ptr<SpaceEquation> yPalne(
        new PlaneEquation(glm::dvec3(0, 1, 0), glm::dvec3(0)));
    std::shared_ptr<SpaceEquation> zPalne(
        new PlaneEquation(glm::dvec3(0, 0, 1), glm::dvec3(0)));


    SECTION("OR combination")
    {
        std::shared_ptr<SpaceEquation> comb = xPalne | yPalne | zPalne;


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
        std::shared_ptr<SpaceEquation> comb = xPalne & yPalne & zPalne;


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

TEST_CASE("Shape/SpaceEquation/Planes/Raycast",
          "Raycasts in combinations of the three planes")
{
    std::shared_ptr<SpaceEquation> xPalne(
        new PlaneEquation(glm::dvec3(1, 0, 0), glm::dvec3(0)));
    std::shared_ptr<SpaceEquation> yPalne(
        new PlaneEquation(glm::dvec3(0, 1, 0), glm::dvec3(0)));
    std::shared_ptr<SpaceEquation> zPalne(
        new PlaneEquation(glm::dvec3(0, 0, 1), glm::dvec3(0)));


    SECTION("OR combination")
    {
        std::shared_ptr<SpaceEquation> comb = xPalne | yPalne | zPalne;
        std::vector<RaycastReport> reports;


        // Corner intersection
        reports.clear();
        Ray cRay(glm::dvec3( 1, 1, 1), glm::dvec3(-1,-1,-1));
        comb->raycast(cRay, reports);
        REQUIRE(reports.size() == 3);
        REQUIRE(reports[0].t == 1.0);
        REQUIRE(reports[1].t == 1.0);
        REQUIRE(reports[2].t == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0));
        REQUIRE(reports[1].position == glm::dvec3(0));
        REQUIRE(reports[2].position == glm::dvec3(0));

        // Line intersection
        reports.clear();
        Ray lRay(glm::dvec3( 1, 1, 1), glm::dvec3(-1,-.75,-1));
        comb->raycast(lRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].t == 1.0);
        REQUIRE(reports[1].t == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0, 0.25, 0));
        REQUIRE(reports[1].position == glm::dvec3(0, 0.25, 0));

        // Point intersection
        reports.clear();
        Ray pRay(glm::dvec3( 1, 1, 2), glm::dvec3(-1,-.75,-1));
        comb->raycast(pRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].t == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0, 0.25, 1));
    }

    SECTION("AND combination")
    {
        std::shared_ptr<SpaceEquation> comb = xPalne & yPalne & zPalne;
        std::vector<RaycastReport> reports;


        // Corner intersection
        reports.clear();
        Ray cRay(glm::dvec3( 1,  1,  1), glm::dvec3(-1,  -1, -1));
        comb->raycast(cRay, reports);
        REQUIRE(reports.size() == 3);
        REQUIRE(reports[0].t == 1.0);
        REQUIRE(reports[1].t == 1.0);
        REQUIRE(reports[2].t == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(0));
        REQUIRE(reports[1].position == glm::dvec3(0));
        REQUIRE(reports[2].position == glm::dvec3(0));

        // Line intersection
        reports.clear();
        Ray lRay(glm::dvec3( 0,  1,  1), glm::dvec3(-1,  -1, -1));
        comb->raycast(lRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].t == 1.0);
        REQUIRE(reports[1].t == 1.0);
        REQUIRE(reports[0].position == glm::dvec3(-1.0, 0.0, 0));
        REQUIRE(reports[1].position == glm::dvec3(-1.0, 0.0, 0));

        // Point intersection
        reports.clear();
        Ray pRay(glm::dvec3( 1,  1,  2), glm::dvec3(-1,  -1, -1));
        comb->raycast(pRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].t == 2.0);
        REQUIRE(reports[0].position == glm::dvec3(-1, -1, 0));
    }
}

TEST_CASE("Shape/SpaceEquation/Spheres/isIn",
          "Point position in combinations of two spheres")
{
    std::shared_ptr<SpaceEquation> negSphere(
        new SphereEquation(glm::dvec3(-1, 0, 0), 2.0));
    std::shared_ptr<SpaceEquation> posSphere(
        new SphereEquation(glm::dvec3(1,  0, 0), 2.0));

    SECTION("OR combination")
    {
        std::shared_ptr<SpaceEquation> comb = negSphere | posSphere;

        REQUIRE(comb->isIn(-2,  0, -2) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1,  0, -1) == EPointPosition::IN);
        REQUIRE(comb->isIn( 0,  0,  0) == EPointPosition::IN);
        REQUIRE(comb->isIn( 1,  0,  1) == EPointPosition::IN);
        REQUIRE(comb->isIn( 2,  0,  2) == EPointPosition::OUT);

    }

    SECTION("AND combination")
    {
        std::shared_ptr<SpaceEquation> comb = negSphere & posSphere;

        REQUIRE(comb->isIn(-2,  0, -2) == EPointPosition::OUT);
        REQUIRE(comb->isIn(-1,  0, -1) == EPointPosition::OUT);
        REQUIRE(comb->isIn( 0,  0,  0) == EPointPosition::IN);
        REQUIRE(comb->isIn( 1,  0,  1) == EPointPosition::OUT);
        REQUIRE(comb->isIn( 2,  0,  2) == EPointPosition::OUT);
    }
}

TEST_CASE("Shape/SpaceEquation/Spheres/Raycast",
          "Raycasts in combinations of two spheres")
{
    std::shared_ptr<SpaceEquation> negSphere(
        new SphereEquation(glm::dvec3(-1, 0, 0), 2.0));
    std::shared_ptr<SpaceEquation> posSphere(
        new SphereEquation(glm::dvec3(1,  0, 0), 2.0));

    Ray xRay(    glm::dvec3( -4,  0,  0), glm::dvec3(1,  0,  0));
    Ray yNegXRay(glm::dvec3( -1,  0,  4), glm::dvec3(0,  0, -1));
    Ray yPosXRay(glm::dvec3(  1,  0, -4), glm::dvec3(0,  0,  1));


    SECTION("OR combination")
    {
        std::shared_ptr<SpaceEquation> comb = negSphere | posSphere;
        std::vector<RaycastReport> reports;

        // Aligned with spheres and x axis
        reports.clear();
        comb->raycast(xRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].t == 1.0);
        REQUIRE(reports[1].t == 7.0);
        REQUIRE(reports[0].position == glm::dvec3(-3, 0, 0));
        REQUIRE(reports[1].position == glm::dvec3( 3, 0, 0));

        // Aligned with y axis and on negative x sphere
        reports.clear();
        comb->raycast(yNegXRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].t == 2.0);
        REQUIRE(reports[1].t == 6.0);
        REQUIRE(reports[0].position == glm::dvec3(-1, 0, 2));
        REQUIRE(reports[1].position == glm::dvec3(-1, 0,-2));

        // Aligned with y axis and under positive x sphere
        reports.clear();
        comb->raycast(yPosXRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].t == 2.0);
        REQUIRE(reports[1].t == 6.0);
        REQUIRE(reports[0].position == glm::dvec3( 1, 0,-2));
        REQUIRE(reports[1].position == glm::dvec3( 1, 0, 2));
    }

    SECTION("AND combination")
    {
        std::shared_ptr<SpaceEquation> comb = negSphere & posSphere;
        std::vector<RaycastReport> reports;

        // Aligned with spheres and x axis
        reports.clear();
        comb->raycast(xRay, reports);
        REQUIRE(reports.size() == 2);
        REQUIRE(reports[0].t == 5.0);
        REQUIRE(reports[1].t == 3.0);
        REQUIRE(reports[0].position == glm::dvec3( 1, 0, 0));
        REQUIRE(reports[1].position == glm::dvec3(-1, 0, 0));

        // Aligned with y axis and on negative x sphere
        reports.clear();
        comb->raycast(yNegXRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].t == 4.0);
        REQUIRE(reports[0].position == glm::dvec3(-1, 0, 0));

        // Aligned with y axis and under positive x sphere
        reports.clear();
        comb->raycast(yPosXRay, reports);
        REQUIRE(reports.size() == 1);
        REQUIRE(reports[0].t == 4.0);
        REQUIRE(reports[0].position == glm::dvec3(1, 0, 0));
    }
}
