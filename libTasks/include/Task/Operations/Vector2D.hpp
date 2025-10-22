#pragma once

/*! Core */
#include <MathTools/AngularUnits/Radian.hpp>
#include <MathTools/Constants.hpp>
#include <Point/Point2D.hpp>

/*! std */
#include <cmath>
#include <limits>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Operations {

//==============================================================================
/*!
 * \brief Класс геометрических объектов, по большей части свойств, эквивалентных векторам на плоскости.
 * \ Отличие лишь в том, что, начало объекта Vector2D - привязано к определенной реперной точке плоскости.
*/
template<class Point_t>
class Vector2D
{
public:
    Point_t b;
    Point_t e;
    double X, Y;

public:
    /*!
     * \brief Vector2D может быть построен (определён) по двум точкам: начальной и конечной.
     * \param[in] точка начала
     * \param[in] точка конца
     */
    explicit Vector2D(const Point_t& b_, const Point_t& e_)
        : b(b_), e(e_)
    {
        X = e.getX() - b.getX();
        Y = e.getY() - b.getY();
    }

    /*!
     * \brief Vector2D может быть также определён точкой начала и значениями координат  X, Y для самого вектора
     * \ (флаг polar = 0)
     * \param[in] точка начала
     * \param[in] X
     * \param[in] Y
     * \param[in] polar=0
     * \
     * \brief Vector2D может быть также определён точкой начала и значениями радиус вектора X и угла Y (в радианах, 0+ рад: N->W)
     * \ (флаг polar = 1)
     * \param[in] точка начала
     * \param[in] X
     * \param[in] Y
     * \param[in] polar=1
     */
    explicit Vector2D(const Point_t& b_, double X_, double Y_, bool polar = 0)
        : b(b_)
    {
        if (!polar)
        {
            X = X_;
            Y = Y_;
            e = Point_t(b_.getX() + X_, b_.getY() + Y_);
        }
        else /* now {X_,Y_} means {rho, phi} --'azimuthal' polar coords,\
                with azimuth phi==0 to be pointed to North, CW-direction corresponds to positive phi values*/
        {
            X = X_ * cos(Y_);
            Y = X_ * sin(Y_);
            e = Point_t(b.getX() + X, b.getY() + Y);
        }
    }

    /*
     * \brief Vector2D может быть также определён точкой начала и величиной радиус-вектора R и объектом класса Radian(в радианах, 0+ рад: N->W)
     * \param[in] точка начала
     * \param[in] R
     * \param[in] Radian phi
     */
    explicit Vector2D(const Point_t& b_, double R_, Radian phi_)
        : b(b_)
    {
        double phi{phi_.get()};
        X = R_ * cos(phi);
        Y = R_ * sin(phi);
        e = Point_t(b.getX() + X, b.getY() + Y);
    }

    /*!
     * \brief Copy ctr.
     */
    Vector2D(const Vector2D& rhs)
        : b(rhs.b), e(rhs.e), X(rhs.X), Y(rhs.Y)
    {
    } /*Copy c-tor*/
    /*!
     * \brief Copy operator=
     */
    Vector2D& operator=(const Vector2D& rhs)
    {
        if (this == &rhs)
            return *this;

        Vector2D tmp(rhs);
        std::swap(*this, tmp);
        return *this;
    }
    Vector2D(Vector2D&& rhs) = default;
    Vector2D& operator=(Vector2D&& rhs) = default;

    //   Vector2D( Vector2D&& rhs ): X( rhs.X ), Y( rhs.Y ), b(std::move(rhs.b)), e(std::move(rhs.e)) {}
    //
    //   Vector2D& operator=( Vector2D&& rhs )
    //   {
    //
    //       if ( this == &rhs ) return *this;
    //
    //       std::swap( *this, rhs );
    //       return *this;
    //
    //   }
    //
    /*!
     * \brief возвращает длину вектора
     */
    inline double Abs() const
    {
        return sqrt(X * X + Y * Y);
    }
    /*!
     * \brief возвращает длину вектора
     */

    inline double length() const
    {
        return this->Abs();
    }
    /*!
     * \brief возвращает направление вектора в радианах , отсчитываемых от N по часовой.
     */
    inline double getCourse() const
    {
        // TODO: не все ветки возвращают значение
        if (fabs(X) < ACCURACY_DOUBLE) // case Y==0
        {
            if (fabs(Y) < ACCURACY_DOUBLE)
            {
                throw std::runtime_error("Vector2D::getCourse() : segment of zero length!");
            }

            return Y > 0 ? M_PI / 2 : -M_PI / 2;
        }

        auto t = Y / X;

        if (fabs(t) < ACCURACY_DOUBLE)
        {
            return X > 0 ? 0 : M_PI;
        } //case X==0

        auto phi = atan(t);

        if (t > 0)
        {
            return X > 0 ? phi : -M_PI + phi;
        }
        else if (t < 0)
        {
            return X > 0 ? phi : M_PI + phi;
        }
    }

    /*!
     * \brief getCoreCourse
     * \return направление вектора в виде объекта класса Radian (из библиотеки Core)
     */
    inline Radian getCoreCourse() const
    {
        return Radian(this->getCourse());
    }

    /*!
     * \brief getAngleTo угол относительно другого вектора, ориентированный или нет
     * \param vec  вектор, относительно которого считается угол
     * \param isOriented   если истинно, то считается ориентированный угол
     * \return
     */
    inline Radian getAngleTo(const Vector2D& vec, bool isOriented = false) const
    {
        Radian courseTo = (this->getCoreCourse() - vec.getCoreCourse()).normalize();
        if (isOriented)
        {
            return courseTo;
        }
        else
        {
            return fabs(courseTo);
        }
    }

    /*!
     * \brief перенос вектора на новую реперную точку.
     */
    void rebase(const Point_t& pnt)
    {
        this->e = Point_t(pnt.getX() + this->X, pnt.getY() + this->Y);
        this->b = pnt;
    }
};
//==============================================================================
//==============================================================================
/*!
 * \brief перенос вектора на новую реперную точку.
 */
//==============================================================================
template<typename Point_t>
constexpr auto tie = Vector2D<Point_t>::rebase;

template<typename Point_t>
inline bool operator==(const Vector2D<Point_t>& lhs, const Vector2D<Point_t>& rhs)
{
    return (lhs.b == rhs.b) && (lhs.e == rhs.e);
}

template<typename Point_t>
inline bool operator!=(const Vector2D<Point_t>& lhs, const Vector2D<Point_t>& rhs)
{
    return (lhs.b != rhs.b) || (lhs.e != rhs.e);
}

template<typename Point_t>
inline Vector2D<Point_t>& operator+=(Vector2D<Point_t>& lhs, const Vector2D<Point_t>& rhs)
{
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    lhs.e.set(lhs.b.getX() + lhs.X, lhs.b.getY() + lhs.Y);
    return lhs;
}

template<typename Point_t>
inline Vector2D<Point_t> operator+(Vector2D<Point_t> lhs, const Vector2D<Point_t>& rhs)
{
    return lhs += rhs;
}

template<typename Point_t>
inline Vector2D<Point_t>& operator*=(Vector2D<Point_t>& lhs, double mul)
{
    lhs.X *= mul;
    lhs.Y *= mul;
    lhs.e.set(lhs.b.getX() + lhs.X, lhs.b.getY() + lhs.Y);
    return lhs;
}

template<typename Point_t>
inline Vector2D<Point_t>& operator*(Vector2D<Point_t> lhs, double mul)
{
    return lhs *= mul;
}

template<typename Point_t>
inline Vector2D<Point_t>& operator/=(Vector2D<Point_t>& lhs, double delim)
{
    lhs.X /= delim;
    lhs.Y /= delim;
    lhs.e.set(lhs.b.getX() + lhs.X, lhs.b.getY() + lhs.Y);
    return lhs;
}

template<typename Point_t>
inline Vector2D<Point_t>& operator/(Vector2D<Point_t> lhs, double delim)
{
    return lhs /= delim;
}

//==============================================================================
/*!
 * \brief скалярное произведение двух векторов.
 * \param[in] Vector2D
 * \param[in] Vector2D
 * \return double
 */
//==============================================================================
template<typename Point_t>
inline double dot(const Vector2D<Point_t>& lhs, const Vector2D<Point_t>& rhs)
{
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
}
//==============================================================================
/*!
 * \brief векторное произведение двух векторов.
 * \param[in] Vector2D
 * \param[in] Vector2D
 * \return double
 */
//==============================================================================
template<typename Point_t>
inline double crossZ(const Vector2D<Point_t>& lhs, const Vector2D<Point_t>& rhs)
{
    return lhs.X * rhs.Y - lhs.Y * rhs.X;
}
//==============================================================================
/*!
 * \brief зеркальное отражение от плоскости, проходящей через заданную точку, нормально вектору.
 * \param[in] Vector2D<Point_t>
 * \param[in] Point_t
 * \return Vector2D<Point_t>
 */
//==============================================================================
template<typename Point_t>
Vector2D<Point_t> normalReflect(const Vector2D<Point_t>& vec, const Point_t& pnt) // yields a vector that is a reflection from the normal plane passing through point P
{
    //    auto absVec = vec.Abs();
    Point_t mirrPnt = (vec * (dot(Vector2D<Point_t>{vec.b, pnt}, vec) / vec.Abs())).e; //project of P to the line defined by vector vec

    Point_t refPnt_b = (Vector2D<Point_t>{mirrPnt, vec.b} * (-1)).e; // mirrored image of vec.b
    Point_t refPnt_e = (Vector2D<Point_t>{mirrPnt, vec.e} * (-1)).e; // mirrored image of vec.e
    return Vector2D<Point_t>{refPnt_b, refPnt_e};

    // TEST : wether V + normalReflect(V) == null_vector ?
}

//==============================================================================
/*!
 * \brief меняет местами начало и конец вектора.
 * \param[in] Vector2D<Point_t>
 * \return Vector2D<Point_t>
 */
//==============================================================================

template<typename Point_t>
Vector2D<Point_t> reverse(const Vector2D<Point_t>& vec) // yields the vector with swapped .b and .e
{
    return Vector2D<Point_t>{vec.e, vec.b};
    // TEST : wether V + V.reverse() == null_vector ?
}

//==============================================================================
/*!
 * \ Таким образом, решение ПГЗ состоит в вызове соотв. конструктора:
 * \ auto obj = Vector2D(const Point_t& ref_point, double R, double Azimuth_rad, bool{1}) ,
 * \ и определением, таким образом, конечной точки: end_point = obj.e .
 * \  Пример:
 * \    #include <Math/Vector2D.hpp>
 * \    ......
 * \    //input:
 * \    auto point_0=Point2D{4.,5.};
 * \    double R = 10; // e.g. n-miles
 * \    double Azmth= M_PI/4;
 * \    //
 * \    auto Vobj = Vector2D(point_0, R, Azmth, 1);
 * \    //output:
 * \    auto point_1= Vobj.e;
 * \
 * \ Решение же ОГЗ -соcтоит в вызове конструктора "по двум точкам":
 * \ obj = Vector2D(const Point_t& p1, const Point_t& p2) ,
 * \ c последующим вызовом методов obj.length() и obj.getCourse(),
 * \ для получения значений расстояния и азимутального угла в радианах,
 * \ в диапазоне от -Pi до Pi .
 * \  Пример:
 * \    #include <Math/Vector2D.hpp>
 * \    ......
 * \    //input:
 * \    auto point_0=Point2D{4.,5.};
 * \    auto point_1=Point2D{14.,15.};
 * \    //
 * \    auto Vobj = Vector2D(point_0, point_1);
 * \    //
 * \    //output:
 * \    auto R = Vobj.length();
 * \    auto Azmth= Vobj.getCourse();
 * \
*/
//==============================================================================

//==============================================================================
/*!
 * \ brief Класс разных утилитарных методов и опций, для точек и векторов на плоскости
 * \
 */
//==============================================================================
template<typename Point_t, typename Vec_t = Vector2D<Point_t>>
class layoutOpts
{
public:
    static std::optional<Point_t> projection(const Point_t& pnt, Vec_t vec)
    {
        auto len_v = vec.Abs();
        auto proj_vec_to_point_on_v = dot(vec, Vec_t(vec.b, pnt)) / len_v; // projection of vector 'nv' to vector 'v'

        if (proj_vec_to_point_on_v < 0 || proj_vec_to_point_on_v > len_v) // case of proj. point doesn't lie on the segment = {vec.b, vec.e}
        {
            return {}; //return nullopt_t
        }
        // vec /= len_v; // normalize 'vec'
        // vec *= proj_vec_to_point_on_v; // now vec is  a vector with length == proj_vec_to_point_on_v
        vec *= (proj_vec_to_point_on_v / len_v); // now vec is  a vector with length == proj_vec_to_point_on_v
        return vec.e;
    }

    static double distancePointToSegment(const Point_t& pnt, const Vec_t& vec, bool anyWay = 1)
    {
        auto Opt = projection(pnt, vec);
        if (Opt.has_value()) //projection point  lie on the segment = {v.b,v.e}
        {
            // return abs(cross(v,Vector2D<Point_t>(v.b,p))  )
            return (Vec_t{pnt, Opt.value()}).Abs();
        }
        else //projection poin lies beyond segment's ending poins
        {
            if (anyWay)
            {
                auto d1 = (Vec_t{pnt, vec.b}).Abs();
                auto d2 = (Vec_t{pnt, vec.e}).Abs();
                return d1 < d2 ? d1 : d2;
            }
        }
    }

    static inline void ifZeroLengthSegment(const Vec_t& vec)
    {
        if (vec.Abs() < ACCURACY_DOUBLE)
        {
            throw std::runtime_error(std::string("segment of zero length"));
        }
        //    catch ( std::invalid_argument const& ex )
        //    {
        //        std::cerr << ex.what() << "\n";
        //    }
    }

    static inline bool isPointTheLeft(const Point_t& start, const Point_t& end, const Point_t& target) // проверяет: лежит ли точка target левее вектора Vec_t{ start, end } ?
    {
        Vector2D lhs(start, end);
        Vector2D rhs(start, target);
        return crossZ(lhs, rhs) < 0;
    }

    static bool ifPointOnSegment(const Point_t& P, const Vec_t& AB) // проверяет: лежит ли точка P  на отрезке AB ?
    {
        ifZeroLengthSegment(AB);

        Vec_t AP{AB.b, P};
        Vec_t BP{AB.e, P};
        BP.tie(AB.b);

        if ((AP.Abs() < ACCURACY_DOUBLE) || (BP.Abs() < ACCURACY_DOUBLE))
            return 1; // case of a point coincide with one of segment's ends

        Vec_t BA{reverse(AB)};
        return (fabs(crossZ(AP, AB)) < ACCURACY_DOUBLE) && (dot(AP, AB) > 0) && (dot(BP, BA) > 0) ? 1 : 0;
    }

    static bool isCoincide(const Point_t& a, const Point_t& b)
    {
        return ((a.getX() - b.getX()) * (a.getX() - b.getX()) + (a.getY() - b.getY()) * (a.getY() - b.getY())) < ACCURACY_DOUBLE;
    }

    static inline bool ifTouch(const Vec_t& AB, const Vec_t& CD)
    {
        return (isCoincide(AB.b, CD.b) || isCoincide(AB.b, CD.e) || isCoincide(AB.e, CD.b) || isCoincide(AB.e, CD.e));
    }

    static inline uint8_t defineLayout(const Vec_t& AB, const Vec_t& CD) //  Best  boy
    {
        if (ifTouch(AB, CD))
        {
            return 0;
        } //case of touching ends

        Vec_t AC{AB.b, CD.b};
        Vec_t AD{AB.b, CD.e};
        Vec_t BC{AB.e, CD.b};
        Vec_t BD{AB.e, CD.e};
        Vec_t BA{reverse(AB)};
        Vec_t CA{reverse(AC)};
        Vec_t CB{reverse(BC)};

        if (fabs(crossZ(AB, CD)) < ACCURACY_DOUBLE) // case of parallel or colinear
        {
            if (fabs(crossZ(AB, AC)) > ACCURACY_DOUBLE)
            {
                // std::cout<< "case of parallel\n";

                return 1; // parallel
            }
            else // collinear !!!!!!!!
            {
                // std::cout<< "case of collinear\n";

                double d1(dot(AB, AC));
                double d2(dot(BA, BC));
                double d3(dot(AB, AD));
                double d4(dot(BA, BD));

                // cases kind of:  A----B   C----D have only 2 kinds of signatures ( dot(AB,AC),dot(BA,BC),dot(AB,AD),dot(BA,BD)) =  (+-+-) or (-+-+)
                // other cases of in-line arrangements of 2 segments have other different signatures: (++++), (+++-), (+-++), (++--), ...
                return (((d1 > 0) && (d2 < 0) && (d3 > 0) && (d4 < 0)) || ((d1 < 0) && (d2 > 0) && (d3 < 0) && (d4 > 0))) ? 3 : 2; //| 3=> not intersect : 2=>intersect
            }
        }
        else // case of segments belong to crossing lines
        {
            double abc{crossZ(AB, AC)};
            if (fabs(abc) < ACCURACY_DOUBLE) //case 3 points on line
            {
                return (dot(AC, AB) * dot(BC, BA) < 0) ? 5 : 4; //| 5=> not intersect : 4=>intersect;
            }
            double abd{crossZ(AB, AD)};

            if (fabs(abd) < ACCURACY_DOUBLE) //case 3 points on line
            {
                return (dot(AD, AB) * dot(BD, BA) < 0) ? 5 : 4; //| 5=> not intersect : 4=>intersect;
            }

            double cda{crossZ(CD, CA)};
            if (fabs(cda) < ACCURACY_DOUBLE) //case 3 points on line
            {
                return (dot(CD, reverse(AC)) * dot(reverse(CD), reverse(AD)) < 0) ? 5 : 4; // i.e. (CD*CA)*(DC*DA) <0 | 5=> not intersect : 4=>intersect;
            }

            double cdb{crossZ(CD, CB)};
            if (fabs(cdb) < ACCURACY_DOUBLE) //case 3 points on line
            {
                return (dot(CD, reverse(BC)) * dot(reverse(CD), reverse(BD)) < 0) ? 5 : 4; // i.e. (CD*CB)*(DC*DB) <0 | 5=> not intersect : 4=>intersect;
            }
            return ((abc * abd < 0) && (cda * cdb < 0)) ? 6 : 7; // case of clear intersection | 6 => intersect : 7 => not intersect
        }
    }

    static bool areIntersect(const Vec_t& AB, const Vec_t& CD)
    {
        ifZeroLengthSegment(AB);
        ifZeroLengthSegment(CD);

        auto result = defineLayout(AB, CD);

        // std::cout<< "result = " << (int)result << std::endl;

        return result & 1 ? 0 : 1; // if  result % 2 !=0 => Ne peresekaet
    }

    static Point_t pointOfIntersction(const Vec_t& lhs, const Vec_t& rhs, bool checkIntersection = true) // находит точку пересечения двух отрезков, если такое возможно.
    {
        if (checkIntersection)
        {
            if (!areIntersect(lhs, rhs))
            {
                throw std::runtime_error(std::string("Vector2D:: pointOfIntersction() :\n segments dont intersect!"));
            }
        }
        if (rhs.e.getX() < rhs.b.getX()) //for my convenience
        {
            rhs = rhs.reverse();
        }
        if (lhs.e.getX() < lhs.b.getX())
        {
            lhs = lhs.reverse();
        }

        //
        auto y0 = lhs.b.getY();
        auto y1 = lhs.e.getY();
        auto x0 = lhs.b.getX();
        auto x1 = lhs.e.getX();
        // line eq. lhs:
        auto k1 = (y1 - y0) / (x1 - x0);
        auto b1 = y0 - k1 * x0;

        //
        auto y_0 = rhs.b.getY();
        auto y_1 = rhs.e.getY();
        auto x_0 = rhs.b.getX();
        auto x_1 = rhs.e.getX();
        // line eq. rhs:
        auto k2 = (y_1 - y_0) / (x_1 - x_0);
        auto b2 = y_0 - k1 * x_0;
        // solution:
        auto x = (b2 - b1) / (k1 - k2);
        auto y = k1 * x + b1;
        // проверка на принадлежность отрезкам (достаточно проверить пределы для одного отрезка ):
        if (!((x > x0 - ACCURACY_DOUBLE) && (x < x1 + ACCURACY_DOUBLE) && (y > min(y0, y1) - ACCURACY_DOUBLE) && (y < max(y0, y1) + ACCURACY_DOUBLE)))
        {
            throw std::runtime_error(std::string("Vector2D::pointOfIntersction() :\n point of intersection lies beyond the segments ! "));
        }
        return Point_t{x, y};
    }
};

} //namespace Operations
