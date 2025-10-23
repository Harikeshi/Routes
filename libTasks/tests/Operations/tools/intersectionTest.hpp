#pragma once

#include <SearchMath/Vector2D.hpp>
#include <gtest/gtest.h>

#include <iostream>

#include <fstream>

using point=Point2D;
using Vec=SearchMath::Vector2D<point>;


std::vector<float> parseFile(const char* fileName , int argc, char** argv)
{
    if(argc<=1)
    {
        std::cerr << "usage:\n " << argv[0] << " infile.dat\n";
        exit(1);
    }

  
    std::vector<float> DATA;
    
    if(!fileName)
    {
        fileName =argv[1];

    }
    std::ifstream inFile(fileName);

    if(!inFile)
    {
        std::cerr << "Cannot open input file..\n"
        <<"\tfile '"<< fileName <<"' is NOT FOUND!\nExiting...\n";
        exit(2);
    }
    else if(!inFile.is_open())
    {
        std::cerr<<"\tCouldn't open the file '"<< fileName <<"' !\nExiting...\n";
        exit(3);
    }
    
    std::move( std::istream_iterator<float>(inFile), std::istream_iterator<float>(), std::back_inserter(DATA) );
    // std::copy( std::istream_iterator<float>(inFile), std::istream_iterator<float>(), std::back_inserter(DATA) );

    inFile.close();
    return DATA;
}

void intersectionTest(int argc, char** argv)
{
    std::cout<< "\nHere the Intersection Test: \n"    ;
    auto myData= parseFile(argv[1],argc,argv) ;

    uint32_t j = 0;
    double x1,y1,x2,y2,x3,y3,x4,y4 ;

    for( auto i= 0; i < myData.size(); ++i )
    {
        j=i%8;

        switch(j)
        {
            case 0:
                x1=myData[i];
                break;
            case 1:
                y1=myData[i];
                break;
            case 2:
                x2=myData[i];
                break;
            case 3:
                y2=myData[i];
                break;
            case 4:
                x3=myData[i];
                break;
            case 5:
                y3=myData[i];
                break;
            case 6:
                x4=myData[i];
                break;
            case 7:
                y4=myData[i];

                // std::cout <<"x1= "<< x1 <<"\n";
                // std::cout <<"y1= "<< y1 <<"\n";
                // std::cout <<"x2= "<< x2 <<"\n";
                // std::cout <<"y2= "<< y2 <<"\n";
                // std::cout <<"x3= "<< x3 <<"\n";
                // std::cout <<"y3= "<< y3 <<"\n";
                // std::cout <<"x4= "<< x4 <<"\n";
                // std::cout <<"y4= "<< y4 <<"\n";
                
                point pnt1 {x1,y1};
                point pnt2 {x2,y2};
                point pnt3 {x3,y3};
                point pnt4 {x4,y4};

                std::cout<<"point1:\t{ "<<pnt1.getX() << ", "<<pnt1.getY()<< " };\n";
                std::cout<<"point2:\t{ "<<pnt2.getX() << ", "<<pnt2.getY()<< " };\n";
                std::cout<<"point3:\t{ "<<pnt3.getX() << ", "<<pnt3.getY()<< " };\n";
                std::cout<<"point4:\t{ "<<pnt4.getX() << ", "<<pnt4.getY()<< " };\n";

                // bool r = layoutOpts<point,Vec>::areIntersect( Vec{point{x1,y1}, point{x2,y2} } , Vec{point{x3,y3}, point{x4,y4} }   ); 
                bool r = SearchMath::layoutOpts<point,Vec>::areIntersect( Vec{ pnt1, pnt2 } , Vec{ pnt3, pnt4 } ); 
                std::cout << (r ? "Peresekaet\n" : "Ne peresekaet\n" ) ;

                
                break;
        }
    }
}
