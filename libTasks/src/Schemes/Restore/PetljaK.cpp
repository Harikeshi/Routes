#include <Task/Schemes/Restore/PetljaK.hpp>

#include <Task/Operations/AngularOperations.hpp>
#include <Task/Operations/Vector2D.hpp>

using namespace Operations;

namespace Schemes {
namespace Restore {

using Vec = Operations::Vector2D<point>;

PetljaK::PetljaK(const Input& input)
    : input{input}
{
}

Outputs::Route PetljaK::calculate()
{
    Outputs::Route result;
    point Tpk = input.Tpk;       //           точка потери контакта
    point Tbek_0 = input.Tbek_0; //   исходная точка БНК № 0 , в момент потери контакта

    point Tbek_exit = input.Tbek_exit; //конечная точка БНК, для выхода из района маневрирования по схеме

    double Dpgas = input.Dpgas; //     miles дальность  подкильной ГАС (по сути - "диаметр"  диаграммы направленности )
    double Dbgas = input.Dbgas; //     miles дальность Dbgas  ( в расчетах не участвует пока)

    double Kt_deg = input.Kt_deg;     // deg // курс цели в градусах, в момент потери контакта (он же будет оставаться предполагамым курсом цели во время вторичного поиска)
    double Kbek_deg = input.Kbek_deg; //deg // курс БНК в  t=0 - момент потери контакта.

    double Ut = input.Ut;       //         Vtc,   knots - скорость искомой цели
    double Ukp = input.Ukp;     //        Vkp ,  knots - скорость БНК поисковая
    double Ukph = input.Ukph;   //       Vkph,  knots - скорость БНК полного хода.
    double Ukp_r = input.Ukp_r; //      Vkp_raschetnaja ,  knots - скорость БНК поискова расчетная на последнем участке СТБМ

    float P_tab_calm = input.P_tab_calm;   // табличная величина Pконтакта в случае до 3х баллов
    float P_tab_storm = input.P_tab_storm; //табличная величина Pконтакта в случае свыше 3х баллов

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    double D_gas = std::max(Dpgas, Dbgas);

    double t_pervich_petlya = 1.3 * D_gas / Ut; // время , пока лодка находится в зоне 1 этапа поиска.

    bool if2ndSearchPossible = (Ut < Ukph);

    ty index1stBegin{0};
    ty index1stEnd{0};
    ty index2ndBegin{0};
    ty index2ndEnd{0};

    auto Kt = toRad(Kt_deg);     // переводим курсы в радианы
    auto Kbek = toRad(Kbek_deg); // ---//---

    auto R1 = 0.3 * D_gas;      //     полусторона внутреннего малого квадрата
    auto R1_ = R1 * sqrt(2.);   //     полудиагональ малого квадрата.
    auto R11 = 0.65 * D_gas;    //     полусторона среднего квадрата.
    auto R11_ = R11 * sqrt(2.); //     полудиагональ среднего квадрата.

    result.points.emplace_back(Tbek_0);
    result.velocities.emplace_back(Ukph);

    std::vector<point> MT1; // точки квадратов:
    MT1.reserve(18);

    MT1.emplace_back(Vec{Tpk, R1, normalizeAngle(Kt), true}.e);                //1
    MT1.emplace_back(Vec{Tpk, R1_, normalizeAngle(Kt + M_PI / 4), true}.e);    //2
    MT1.emplace_back(Vec{Tpk, R1, normalizeAngle(Kt + M_PI / 2), true}.e);     //3
    MT1.emplace_back(Vec{Tpk, R1_, normalizeAngle(Kt + toRad(135)), true}.e);  //4
    MT1.emplace_back(Vec{Tpk, R1, normalizeAngle(Kt + toRad(180)), true}.e);   //5
    MT1.emplace_back(Vec{Tpk, R1_, normalizeAngle(Kt + toRad(225)), true}.e);  //6
    MT1.emplace_back(Vec{Tpk, R1, normalizeAngle(Kt + toRad(270)), true}.e);   //7
    MT1.emplace_back(Vec{Tpk, R1_, normalizeAngle(Kt + toRad(315)), true}.e);  //8
    MT1.emplace_back(Vec{Tpk, R11, normalizeAngle(Kt), true}.e);               //9
    MT1.emplace_back(Vec{Tpk, R11_, normalizeAngle(Kt + toRad(45)), true}.e);  //10
    MT1.emplace_back(Vec{Tpk, R11, normalizeAngle(Kt + toRad(90)), true}.e);   //11
    MT1.emplace_back(Vec{Tpk, R11_, normalizeAngle(Kt + toRad(135)), true}.e); //12
    MT1.emplace_back(Vec{Tpk, R11, normalizeAngle(Kt + toRad(180)), true}.e);  //13
    MT1.emplace_back(Vec{Tpk, R11_, normalizeAngle(Kt + toRad(225)), true}.e); //14
    MT1.emplace_back(Vec{Tpk, R11, normalizeAngle(Kt + toRad(270)), true}.e);  //15
    MT1.emplace_back(Vec{Tpk, R11_, normalizeAngle(Kt + toRad(315)), true}.e); //16

    double sMi_1 = Vec{Tbek_0, MT1.at(4)}.Abs(); // расстояние от Tbek_0 до т.5 малого квадрата

    auto dT_1st = t_pervich_petlya - (sMi_1 / Ukph);
    if (dT_1st < 0.f)
        throw std::runtime_error(std::string("dT_1st < 0, 1st search failed !!!!111"));

    double s_Perv = dT_1st * Ukp;        // Put' po malomu kvadratu za t == dT_1st
    double n_Perv = s_Perv / R1;         //= s_Perv/(0.3*D_gas);
    auto n_Perv_i = (int)n_Perv;         // Целая часть от n_Perv (сколько предстоит пройти точек по малому квадрату)
    auto n_Perv_rem = n_Perv - n_Perv_i; // Остаток ( ещё немного пройти, вдоль стороны малого квадрата)
    if (n_Perv_rem < 0)
        throw std::runtime_error(std::string("Remainder < 0"));
    if (std::abs(n_Perv_rem) > 1.f)
        throw std::runtime_error(std::string("|Remainder| > 1"));

    auto azimuth_5 = normalizeAngle(Vec{Tbek_0, MT1[4]}.getCourse() - normalizeAngle(Kbek)); // азимут от Tbek_0  -  на т.5
    index1stBegin = 1;
    int indFirstCycle = 4;

    //if(prin)
    //{
    //    std::cout<<"\n";
    //    std::cout<< "--- dT_1st           = " << dT_1st << "\n";
    //    std::cout<< "--- azimuth_5_deg    = " <<  toDeg(azimuth_5) << "\n" ;
    //    std::cout<< "--- t_pervich_petlya = " <<  t_pervich_petlya << "\n" ;
    //    std::cout<< "--- sMi_1            = " <<  sMi_1 << "\n" ;
    //    std::cout<< "--- s_Perv       = " <<  s_Perv << "\n" ;
    //    std::cout<< "--- n_Perv       = " <<  n_Perv << "\n" ;
    //    std::cout<< "--- n_Perv_i     = " <<  n_Perv_i << "\n" ;
    //}

    auto nextI = [&]() {
        // std::cout<< "\t\tindFirstCycle_begin = "<< indFirstCycle<<"\n";
        //
        auto d = azimuth_5 >= 0 ? 1 : -1;
        indFirstCycle += d;
        indFirstCycle %= 8;
        if (indFirstCycle < 0)
        {
            indFirstCycle += 8;
        }
        // std::cout<< "\t\tnext_indFirstCycle = "<< indFirstCycle<<"\n";
        // static int raz=0;
        // std::cout<< "\t\tnextI_raz: "<< raz++<< "\n";
    };

    for (auto i{0}; i < n_Perv_i; ++i, nextI())
    {
        result.points.emplace_back(MT1.at(indFirstCycle));
        result.velocities.emplace_back(Ukp);
        //wpList.emplace_back(wp{MT1.at(indFirstCycle), Ukp});
    }

    auto indBeforeLast = indFirstCycle;
    result.points.emplace_back(MT1.at(indFirstCycle));
    result.velocities.emplace_back(Ukp);
    //wpList.emplace_back(wp{MT1.at(indFirstCycle), Ukp});

    nextI(); //index of next after the last

    //if(prin)
    //{
    //    std::cout<< "----- ind_quad_BeforeLast = "<< indBeforeLast<<"\n";
    //    std::cout<< "----- ind_quad_AfterLast  = "<< indFirstCycle<<"\n";
    //}

    Vec LastSegm_1{MT1.at(indBeforeLast), MT1.at(indFirstCycle)};

    //if(prin)
    //{
    //    std::cout<< "----- LastSegm_1.Course(deg)          = " << toDeg(LastSegm_1.getCourse())<<"\n";
    //    std::cout<< "----- LastSegm_1.Abs/R1 (pre)      = " << LastSegm_1.length() / R1 <<"\n";
    //}

    // LastSegm_1 /= LastSegm_1.Abs(); // unit vector collinear to LastSegm_1
    // LastSegm_1 *= n_Perv_rem*R1;
    LastSegm_1 *= n_Perv_rem; // - можно проще.

    //if(prin)
    //{
    //    std::cout<< "----- LastSegm_1.Abs().final/R1 = " << LastSegm_1.Abs() / R1 << "\n";
    //}

    result.points.emplace_back(LastSegm_1.e);
    result.velocities.emplace_back(Ukph);
    //wpList.emplace_back(wp{LastSegm_1.e, Ukph});

    index1stEnd = result.points.size() - 1;
    //index1stEnd= wpList.size() - 1;

    if (if2ndSearchPossible) //Если будем во вторичный поиск.
    {
        Vec SegmLastTo10{LastSegm_1.e, MT1.at(9)};
        Vec SegmLastTo16{LastSegm_1.e, MT1.at(15)};
        int ind_mT2_0 = SegmLastTo10.Abs() < SegmLastTo16.Abs() ? 9 : 15; //chose MT1_10 : chose MT1_16S

        double dAngle;
        double S_Mi_3_Abs;

        switch (ind_mT2_0)
        {
        case 15:
        {
            // std::cout<< "case 15\n";
            S_Mi_3_Abs = SegmLastTo16.Abs();
            result.points.emplace_back(MT1.at(15));
            result.velocities.emplace_back(Ukph);
            //wpList.emplace_back(wp{MT1.at(15), Ukph});// first part of S_Mi_4
            dAngle = -M_PI / 2;
            break;
        }
        case 9:
        {
            // std::cout<< "case 9\n";
            S_Mi_3_Abs = SegmLastTo10.Abs();
            result.points.emplace_back(MT1.at(9));
            result.velocities.emplace_back(Ukph);
            //wpList.emplace_back(wp{MT1.at(9), Ukph});// first part of S_Mi_4
            dAngle = M_PI / 2;
            break;
        }
        default:
        {
            throw std::runtime_error(std::string("NEITHER 9 NOR 15 !!!!111"));
            break;
        }
        }

        auto dS_PL1_sbli = Ut * (S_Mi_3_Abs + Ut * t_pervich_petlya - R11) / (Ukph - Ut);
        auto s_PL1_vtor = Ut * t_pervich_petlya + dS_PL1_sbli;

        auto IRTM = Vec{Tpk, s_PL1_vtor, normalizeAngle(Kt), true}.e;
        auto TRTM = Vec{IRTM, D_gas * Ut / Ukp, normalizeAngle(Kt), true}.e;

        auto MT_2nd_0 = Vec{IRTM, R11, normalizeAngle(Kt + dAngle), true}.e;
        auto MT_2nd_1 = Vec{TRTM, R11, normalizeAngle(Kt + dAngle), true}.e;

        result.points.emplace_back(MT_2nd_0);
        result.velocities.emplace_back(Ukp);
        //wpList.emplace_back(wp{ MT_2nd_0, Ukp   });// second part of S_Mi_4

        index2ndBegin = result.points.size() - 1;
        //index2ndBegin = wpList.size() - 1;

        result.points.emplace_back(MT_2nd_1);
        result.velocities.emplace_back(Ukp_r);
        //wpList.emplace_back(wp{ MT_2nd_1, Ukp_r });// the last part S_Mi_5

        index2ndEnd = result.points.size() - 1;
        //index2ndEnd = wpList.size() - 1;

        //if(prin)
        //{
        //    std::cout<< "<<<<< ind_1st_begin = "  << index1stBegin <<"\n";
        //    std::cout<< "<<<<< ind_1st_end   = "  << index1stEnd <<"\n";
        //    std::cout<< "<<<<< ind_2nd_begin = "  << index2ndBegin <<"\n";
        //    std::cout<< "<<<<< ind_2nd_end   = "  << index2ndEnd <<"\n";
        //}
    }

    /*------------------------------------------------------------------------------------------------------------------------------------*/

    result.points.emplace_back(Tbek_exit);
    result.velocities.emplace_back(Ukph);
    //wpList.emplace_back(wp{Tbek_exit, Ukph});// roll away

    /*заполняем оставшиеся цифры в waypoint-aх */
    /*
    auto wpLen=wpList.size();
    // std::cout<< "\t\t\twpLen="<< wpLen<<"\n";
    for(size_t j{0}; j < wpLen; ++j )
    {
        if(j < wpLen-1)
        {
            auto X1=wpList.at(j).pnt.getX();
            auto Y1=wpList.at(j).pnt.getY();
            auto X2=wpList.at(j+1).pnt.getX();
            auto Y2=wpList.at(j+1).pnt.getY();

            Vec gals{ wpList.at(j).pnt, wpList.at(j+1).pnt}; //так мы вычисляем расстояние до следующего wp
            wpList.at(j).R_next = gals.Abs(); //

            wpList.at(j).crs = gals.getCourse(); // .. и курс на следующий wp
            wpList.at(j).complete=true;
        }
    }
    */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*
    auto T_1st_total = t_pervich_petlya;//-время от самого начала (момент потери контакта T=0 ), до окончания первичного поиска.
    //
    //
    double T_2nd_total = T_1st_total; //-время от самого начала (момент потери контакта T=0 ), до окончания вторичн. поиска.

    double dT_2nd =0.f;// dT_2nd - интервал времени на вторичный поиск.

    if(if2ndSearchPossible)
    {
        for(ty i=index1stEnd; i < index2ndEnd; ++i)
        {
            T_2nd_total += (wpList.at(i).R_next)/(wpList.at(i).vel) ;
        }

        dT_2nd = (wpList.at(index2ndBegin).R_next / wpList.at(index2ndBegin).vel) ;
    }
    */
    /************************** РАСЧЕТ ВЕРОЯТНОСТИ И ПОИСКОВОЙ ПРОИЗВОДИТЕЛЬНОСТИ *****************************************************/
    /*
    //найдем обследованный сектор в первичном поиске :

    ty k = 1;
    double CBK_obsl = 0.f;
    double CBK_naz = 2*M_PI;

    double CBK_obsl_ = 0.f;
    double Angle_1;
    double Angle_2;
    double dAngle;


    for( k = index1stBegin; k < index1stEnd; ++k ) // суммируем угловые расстояния между wp пройденные в первичном поиске, видимые из Тпк:
    {
        Angle_1 = Vec{Tpk, wpList.at( k ).pnt}.getCourse();
        Angle_2 = Vec{Tpk, wpList.at(k+1).pnt}.getCourse();
        dAngle  = Angle_2 - Angle_1;
        //
        // if(std::abs(dAngle) > M_PI) // no need to check, just use normalizeAngle(dAngle)
        //  {
        //      dAngle = std::fmod(dAngle, 2*M_PI);
        //      if(dAngle > M_PI)
        //          dAngle -= (2*M_PI);
        //      else if(dAngle < -M_PI)
        //          dAngle += (2*M_PI);
        //  }

        dAngle = normalizeAngle(dAngle);

        CBK_obsl_ += dAngle;
        //if(prin)printf(" dAngle  = %0.4f\n", dAngle);
    }
    CBK_obsl_ = std::abs(CBK_obsl_);

    // вычислим  СВК_обсл  другим способом :
    CBK_obsl =   (n_Perv_i & 1) ? (0.25*M_PI*(n_Perv_i + 1) - std::atan( 1.f - n_Perv_rem) ) : (0.25*M_PI*n_Perv_i  + std::atan(n_Perv_rem) );

    if( CBK_obsl > 2*M_PI )
    {
        if(prin)std::cout << "CBK_obsl > 2*Pi !!!!!111\n";
        CBK_obsl = 2.*M_PI;
    }
    auto Pukl_1 = 1.f - CBK_obsl/CBK_naz;


    //if(prin)
    //{
    //    printf(" CBK_obsl(deg)  = %0.4f\n", toDeg(CBK_obsl));
    //    printf(" CBK_obsl_(deg)  = %0.4f\n", toDeg(CBK_obsl_));
    //    // printf(" CBK_obsl_(deg) = %0.4f\n", toDeg(CBK_obsl_));
    //    printf(" P_ukl_1st = %0.4f\n", Pukl_1);
    //}
    // if(std::abs(CBK_obsl - CBK_obsl_ ) > EPS)
    //     throw std::runtime_error( std::string("|CBK_obsl - CBK_obsl_| > EPS !!!!111") );

    if(Pukl_1 > 1.f)
        throw std::runtime_error( std::string("Pukl_1 > 1 !!!!111") );
    if(Pukl_1 < 0.f)
        throw std::runtime_error( std::string("Pukl_1 < 0 !!!!111") );

    // auto S_obsl_2 = (T_2nd_total*Ukp + D_gas)*D_gas; //- as Kosolapoff said to do/

    auto S_obsl_2 = dT_2nd * Ukp * D_gas + M_PI * D_gas * D_gas/4 ;

    // auto S_t_ukl = M_PI * Ut * Ut * ( T_2nd_total * T_2nd_total - T_1st_total * T_1st_total ) ;
    auto S_t_ukl = M_PI * Ut * Ut * ( T_2nd_total * T_2nd_total - (T_2nd_total - dT_2nd) * (T_2nd_total - dT_2nd) ) ;

    if(S_t_ukl < 0)
        throw std::runtime_error( std::string("S_t_ukl < 0 !!!!111") );
    if(S_t_ukl < S_obsl_2)
        throw std::runtime_error( std::string("S_t_ukl < S_obsl_2 !!!!111") );


    auto Pukl_2 =  if2ndSearchPossible  ? (1.f - S_obsl_2/S_t_ukl) : 1.f ;


    auto Eff_1st = CBK_obsl / CBK_naz;

    auto Eff_2nd = if2ndSearchPossible ? S_obsl_2 / S_t_ukl : 0.f;

    // if(Pukl_2 > 1.f)
    //     throw std::runtime_error( std::string("Pukl_2 > 1 !!!!111") );

    auto Calc_P = [&]( float P_tab )
    {
        double  Pobn_1 = P_tab * ( 1.0f - Pukl_1 );
        double  Pobn_2 = P_tab * ( 1.0f - Pukl_2 );
        auto    P_all  = 1.f - (1.f - Pobn_1) * (1.f - Pobn_2);

        if(Pobn_1 > 1.f)
            throw std::runtime_error( std::string("Pobn_1 > 1 !!!!111") );
        if(Pobn_1 < 0.f)
            throw std::runtime_error( std::string("Pobn_1 < 0 !!!!111") );
        if(Pobn_2 > 1.f)
            throw std::runtime_error( std::string("Pobn_2 > 1 !!!!111") );
        if(Pobn_2 < 0.f)
            throw std::runtime_error( std::string("Pobn_2 < 0 !!!!111") );
        if(P_all > 1.f )
            throw std::runtime_error( std::string("P_all > 1 !!!!111") );
        if(P_all < 0.f )
            throw std::runtime_error( std::string("P_all < 0 !!!!111") );

        if(1)
        {
            //std::cout<< "\tPobn_1("<< P_tab <<") = " << Pobn_1 << "\n" ;
            //std::cout<< "\tPobn_2("<< P_tab <<") = " << Pobn_2 << "\n" ;
            //std::cout<< "\tP_all_("<< P_tab <<") = " << P_all << "\n" ;
            //std::cout<< "\tEff_1st = " << Eff_1st << "\n" ;
            //std::cout<< "\tEff_2nd = " << Eff_2nd << "\n" ;
        }

        return P_all;
    };

    //if(prin) std::cout <<"\t__Calm:____\n";
    auto P_all_calm = Calc_P( P_tab_calm )  ;
    //if(prin) std::cout <<"\t__Storm:____\n";
    auto P_all_storm = Calc_P( P_tab_storm );

    Effectiveness Ef { P_all_calm, P_all_storm, Eff_1st, Eff_2nd };

    RestoreResult RR{};

    //for (size_t i = 1; i < wpList.size(); i++) {
    //    point inversPoint {wpList.at(i).pnt.getY(), wpList.at(i).pnt.getX()};
    //    wpList.at(i).pnt = inversPoint;
    //}

    std::swap(RR.wpList, wpList);
    std::swap(RR.Ef,     Ef);

    //if(1)std::cout<<"~~~~/endof__Petlja_k__~~~~~~~~~~\n\n";

    return RR;
    */
    return result;
}

} // namespace Restore
} // namespace Schemes
