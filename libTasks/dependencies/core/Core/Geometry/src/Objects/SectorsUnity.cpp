#include <Geometry/Objects/SectorsUnity.hpp>

#include <Geometry/GeometryException.hpp>
//! MathTools
#include <MathTools/Constants.hpp> // точности сравнений
#include <MathTools/BasicMath/Range.hpp> // нормализация
//! std
#include <algorithm>
//==============================================================================
namespace Geometry
{
const Degree period = Degree(PERIGON);
//==============================================================================
/*! \brief Приведение границ сектора к целочисленным значениям
 * \param[in] sector - сектор в стандартном представлении
 * \return пара целых градусов
 */
std::pair <Degree, Degree> roundingSector(
  const std::pair <Degree, Degree>& sector)
{
  int left;
  int right;
  double eps = ACCURACY_DEGREE * 100;

  // приводим к диапазону [0, 360], обязательно включая правую границу
  double firstVal = sector.first.get();
  double secondVal = sector.second.get();

  //левая граница в большистве случаев смещается в большую сторону
  if( ( std::fabs( std::floor( firstVal ) - firstVal ) ) < eps )
    left = static_cast<int>( std::floor( firstVal ) );
  else 
    left = static_cast<int>( std::ceil( firstVal ) );

  //правая граница в большистве случаев смещается в меньшую сторону
  if( ( std::fabs( std::ceil( secondVal ) - secondVal ) ) < eps )
    right = static_cast<int>( std::ceil( secondVal ) );
  else 
    right = static_cast<int>( std::floor( secondVal ) );

  return std::make_pair( Degree(left), Degree(right) );
}
//==============================================================================
SectorsUnity::SectorsUnity( 
  const std::vector< Sector> &randSecsVect, const Radian eps )
: mEps (std::max(0., eps.toDegrees().get( ))),
mSortList( std::list <Degree>( ) ),
mFullCircle( false )
{
  addSectors( randSecsVect );
}
//==============================================================================
SectorsUnity::SectorsUnity( 
  const std::vector<Sector>& randSecsVect, const Degree eps )
: mEps( std::max( 0., eps.get( ))),
mSortList( std::list <Degree>( ) ),
mFullCircle( false )
{
  addSectors( randSecsVect );
}
//==============================================================================
void SectorsUnity::findBound(std::list<Degree>::iterator &head,
                        bool &insideCount,
                        const Degree& bound)
{
  while ((head != mSortList.end()) && (bound > *head))
  {
    ++head;
    insideCount ^= 1; //попали внутрь сектора или нет
  }
}
//==============================================================================
std::list<Degree>::iterator SectorsUnity::putBound
  (std::list<Degree>::iterator &nextBoundIt,
    const Degree& bound)
{
  nextBoundIt = mSortList.insert(nextBoundIt, bound);
  return nextBoundIt;
}
//==============================================================================
void SectorsUnity::applyGlue(std::list<Degree>::iterator &right,
                        std::list<Degree>::iterator &left,
                        const std::pair <Degree, Degree> &Vector,
                        bool &countL, bool &countR)
{
  if ( (!countL) && (left != mSortList.begin()) ) //левая граница между секторами
  {
    if (*(std::prev(left)) + mEps >= Vector.first) //новая граница "приклеивается" к предыдущей
    {
      countL ^= 1;
      --left;
    }
  }

  if ((!countR) && (right != mSortList.end())) //правая граница между секторами
  {
    if (*right - mEps <= Vector.second)
    {
      right++;
      countR ^= 1;
    }
  }

}
//==============================================================================
bool SectorsUnity::addSector( const Sector & sector )
{
  std::pair <Degree, Degree> newSector = std::make_pair(
    sector.getBegAngle().toDegrees(), 
    sector.getEndAngle().toDegrees());

  std::list<Degree>::iterator head = mSortList.begin();

  bool countL = false;
  auto left = head;
  bool countR = countL;
  auto right = left;

  if (mFullCircle) 
    return true;

  //обработка пересечения 0
  if (newSector.first > newSector.second)
  {//пересекает

    if (mSortList.empty())
    {
      mSortList.emplace_back(0);
      mSortList.emplace_back(newSector.second);
      mSortList.emplace_back(newSector.first);
      mSortList.emplace_back( period );
      return false;
    }

    // кладем в список часть до конца периода
    // (в текущей реализации - 360 градусов)
    std::pair <Degree, Degree> tempSector 
      = std::make_pair(newSector.first, period );
    findBound(left, countL, tempSector.first);
    right = mSortList.end();
    //т.к. граница может совпасть с существующей
    countR = ( mSortList.back() == period );
    applyGlue(right, left, tempSector, countL, countR);

    if (!countL) // левая граница попала на пустую область
      putBound(left, tempSector.first); 
    if (!countR) //если до вставки сектора не доходили до 360
      mSortList.emplace_back( period ); 
    right--; //чтобы итератор указывал на последний элемент (360 градусов)
    if (!countL) 
      ++left; //erase работает с границами как {)
    mSortList.erase(left, right);


    //кладем в список часть от 0
    tempSector = std::make_pair(Degree(0), newSector.second);
    head = mSortList.begin();
    left = head;
    right = head;
    countL = ( head->get() == 0 );//т.к. граница может совпасть с существующей
    countR = false; 
    findBound(right, countR, tempSector.second);
    applyGlue(right, left, tempSector, countL, countR);
    if (head->get() != 0)
      mSortList.emplace_front(0);
    left = mSortList.begin();

    if (!countR) // правая граница попала на пустую область
      putBound(right, tempSector.second); 
    if (left != right) 
      ++left; //erase работает как {)

    //удаляем элементы между левой и правой границей 
    mSortList.erase(left, right);
  }

  else
  { // добавляем простой сектор

    if (mSortList.empty()) //быстрое добавление одного сектора
    {
      mSortList.emplace_back(newSector.first);
      mSortList.emplace_back(newSector.second);
    }

    else //добавление сектора к уже имеющимся
    {
      findBound(left, countL, newSector.first);
      countR = countL;
      right = left;
      findBound(right, countR, newSector.second);

      applyGlue(right, left, newSector, countL, countR);

      if (!countL) // левая граница попала на пустую область
        putBound(left, newSector.first);
      if (!countR) // правая граница попала на пустую область
        putBound(right, newSector.second);
      if (!countL) 
        ++left; //erase работает с границами как {)

      //удаляем элементы между левой и правой границей 
      mSortList.erase(left, right);
    }
  }

  //проверяем, не получилась ли в итоге полная окружность
  head = mSortList.begin();
  auto tail = head;
  ++tail;
  int sum = 0;

  while (tail != mSortList.end())
  {
    sum += tail->get() - head->get();
    head = ++tail;
    if (tail != mSortList.end()) 
      ++tail;
  }

  if (( period - sum) <= mEps)
  {
    mSortList.clear();
    mSortList.emplace_back(0);
    mSortList.emplace_back( period );
    mFullCircle = true;
    return mFullCircle;
  }

  return false;
}
//==============================================================================
bool SectorsUnity::addSectors( const std::vector <Sector> &randSecsVect )
{
  bool fullsector = false;

  for( const auto &sector : randSecsVect )
  {
    fullsector = addSector( sector );
    if( fullsector )
      return true;
  }

  return false;
}
//==============================================================================
bool SectorsUnity::crossZero() const noexcept
{
  if (mFullCircle) 
    return false;

  if( !mSortList.size( ) )
    return false;

  //проверяем, лежат ли первый и последний сектор рядом на окружности
  auto head = mSortList.begin();
  auto tail = mSortList.end();
  --tail;
  if (*head <= mEps || *tail >= ( period - mEps))
  {
    if ((( period - *tail) + *head) <= mEps)
    {
      return true;
    }
  }
  return false;
}
//==============================================================================
std::list<Degree>::iterator SectorsUnity::findSector(int index)
{
  auto sizeList = mSortList.size();

  if ((index > (sizeList / 2)) || (index <= 0))
    throw GeometryFailure(GeometryEnumFailure::SectorIndexOutOfRange);

  std::size_t count = 1;

  //проверяем, есть ли сектор, пересекающий 0
  bool zeroCheck = crossZero();
  if( zeroCheck )
  {
    --count; //т.к. в начале лежит на 1 сектор больше
    if( index == ( sizeList / 2 ) )
      throw GeometryFailure(GeometryEnumFailure::SectorIndexOutOfRange);
  }

  auto ptr = mSortList.begin();

  for (; count <= sizeList; count++)
  {
    if (count < index)
    {
      ptr++;
      ptr++;
    }
    else
      return ptr;
  }

  //! Выход внутри "вечного" цикла, но возвращаемое значение должно быть во 
  //! всех ветках. 
  return std::list<Degree>::iterator( );
}
//==============================================================================
bool SectorsUnity::changeSector(int index, const Sector& sector) noexcept
{
  try
  {
    cutSector(index);
  }
  catch (...) {}

  return addSector( sector );
}
//==============================================================================
void SectorsUnity::cutSector(const int index)
{
  auto sizeList = mSortList.size() / 2;

  if ((index > sizeList) || (index <= 0))
    throw GeometryFailure(GeometryEnumFailure::SectorIndexOutOfRange);

  auto secHead = findSector(index);
  auto secTail = secHead;
  ++secTail; 
  ++secTail;

  //есть ли в контейнере сектор, пересекающий 0
  bool zeroCheck = crossZero();
  // лежит ли в самом начале или в самом конце
  if ( (zeroCheck) &&
       ( (secHead == mSortList.begin()) ||
         (secTail == mSortList.end()) ) )
  { // лежит! Значит удаляем оба сектора
    auto secDel = mSortList.end();
    --secDel; 
    --secDel;
    mSortList.erase(secDel, mSortList.end());
    secDel = mSortList.begin();
    secDel++; 
    secDel++;
    mSortList.erase(mSortList.begin(), secDel);
  }
  else 
    mSortList.erase(secHead, secTail);

  mFullCircle = false;

  return;
}
//==============================================================================
void SectorsUnity::getVector( std::vector<Sector>& secsVect ) const
{
  secsVect.clear();
  std::vector<std::pair<Degree, Degree>> sectorsVect;

  if (mSortList.empty()) 
    return;

  if (mFullCircle)
  {
    secsVect.emplace_back( Sector ( Degree(0), period ) );
    return;
  }

  auto head = mSortList.begin();
  auto tail = head;
  auto size = mSortList.size();
  for (auto i = 0; i <size; i += 2)
  {
    ++tail;
    sectorsVect.emplace_back(*head, *tail);
    head = ++tail;
  }

  //проверяем, лежат ли первый и последний сектор рядом на окружности
  //соединяем, если да
  head = mSortList.begin();
  tail = mSortList.end();
  --tail;
  auto last = sectorsVect.size() - 1; //индекс последнего элемента
  if (( sectorsVect.size() > 1) &&
      ( ( sectorsVect[0].first <= mEps ) || 
        ( sectorsVect[last].second >= ( period - mEps) ) ) )
  {
    if ((( period - sectorsVect[last].second) + sectorsVect[0].first) <= mEps)
    {
      //вставляем в конец списка второе значение первого элемента 
      //(конец первого сектора)
      sectorsVect[last].second = sectorsVect[0].second;
      //удаляем первый элемент
      sectorsVect.erase( sectorsVect.begin());
    }
  }

  for( auto elem : sectorsVect )
    secsVect.emplace_back(
      Sector( Degree( elem.first ), Degree( elem.second ) ) );
}
//==============================================================================
std::vector<Sector> SectorsUnity::getVector( ) const
{
  std::vector < Sector > retVal;
  getVector( retVal );
  return retVal;
}
//==============================================================================
std::vector<Sector> SectorsUnity::getIntegerVector()
{
  // преобразуем погрешность для объединения в целочисленную
  const Degree eps = mEps;
  mEps = Degree(int(eps.get() + 0.5));

  // сохраняем дробные сектора
  const std::list<Degree> curList(mSortList);
  mSortList.clear();

  for (auto iter = curList.begin(); iter != curList.end(); ++iter)
  {
    // получаем целочисленный сектор
    Degree first = *iter++;
    auto newSector = roundingSector(std::make_pair(first, *iter));
    Sector sector(Degree(newSector.first), Degree(newSector.second));
    // добавляем целочисленный сектор к списку секторов (чтобы
    // сектора объединились, если расстояние между ними меньше новой 
    // погрешности)
    addSector(sector);
  }

  // записываем сектора в вектор секторов
  std::vector <Sector> result;
  for (auto iter = mSortList.begin(); iter != mSortList.end(); ++iter)
  {
    Degree first = *iter++;
    result.emplace_back(Degree(first), Degree(*iter));
  }

  // возвращаем исходные значения переменных класса
  mEps = eps;
  mSortList = curList;

  return result;
}
//==============================================================================
}  //! Geometry
