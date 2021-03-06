#include "CDateTime.h"
#include "CAutoLock.h"
#include "CInterval.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

using namespace Util;

int CDateTime::_localTimeZone = -8;
CDateTime CDateTime::_lastDt(0);


bool parseYMD(struct tm* time,
        unsigned long* millis,
        const char ch,
        const char* value,
        int index,
        int length,
        int count
        )
{
    char buf[5];
    int nvalue = 0;
    if ( index + count > length )
        return false;
    switch ( ch )
    {
        case 'Y' :
            {
                if ( count == 2 || count == 4 )
                {
                    memcpy( buf, value + index, count );
                    buf[count] = 0;
                    nvalue = atoi( buf );
                }
                else
                    return false;

                if ( count == 2 )
                    nvalue += 2000;
                if ( nvalue >= 1970 )
                    nvalue -= 1900;
                else
                    return false;
                time->tm_year = nvalue;
            }
            break;
        case 'M' :
            {
                if ( count > 2 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue > 12 || nvalue <= 0 )
                    return false;
                time->tm_mon = nvalue-1;
            }
            break;
        case 'D' :
            {
                if ( count > 2 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue > 31 || nvalue <= 0 )
                    return false;
                time->tm_mday = nvalue;
            }
            break;
        case 'h' :
            {
                if ( count > 2 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue > 23 || nvalue < 0 )
                    return false;
                time->tm_hour = nvalue;
            }
            break;
        case 'm' :
            {
                if ( count > 2 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue > 59 || nvalue < 0 )
                    return false;
                time->tm_min = nvalue;
            }
            break;
        case 's' :
            {
                if ( count > 2 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue > 59 || nvalue < 0 )
                    return false;
                time->tm_sec = nvalue;
            }
            break;
        case 'n' :
            {
                if ( count > 3 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue < 0 )
                    return false;
                *millis = nvalue;
            }
            break;
        default :
            return false;
    }
    return true;
}


CDateTime::CDateTime() :_timeZone(0),_showZone(0),_tm(NULL)
{
    *this = getThreadDt();
}

void CDateTime::update()
{
#ifdef _WIN32
    SYSTEMTIME time;
    GetLocalTime( &time );
    init( time.wYear , time.wMonth , time.wDay
            , time.wHour , time.wMinute , time.wSecond , time.wMilliseconds , 0 );
#endif
#ifdef _LINUX
    //struct tm *tt2;
    //struct timeval tstruct1;
    //struct timezone tzp;
    //::gettimeofday(&tstruct1,&tzp);
    //tt2 = localtime(&tstruct1.tv_sec);
    //init( tt2->tm_year+1900 , tt2->tm_mon+1 , tt2->tm_mday,
    //  tt2->tm_hour , tt2->tm_min , tt2->tm_sec , tstruct1.tv_usec / 1000 , 0 );
    struct timespec tm1;
    clock_gettime(CLOCK_REALTIME, &tm1);
    //std::cout << (int64_t)(1000 )*(tpend.tv_sec)+(tpend.tv_nsec) / ( 1000 * 1000 ) << std::endl;
    init( (int64_t)(1000 )*(tm1.tv_sec)+(tm1.tv_nsec) / ( 1000 * 1000 ) , 0 );
#endif
}

CDateTime::CDateTime(int year, int month, int day, int hour,
        int minute, int second,int millseconds,short timezone)
:_tm(NULL)
{
    init( year, month, day, hour, minute, second, millseconds, timezone );
}

bool CDateTime::init(int year, int month, int day, int hour, int minute,
        int second,int millseconds,short timezone)
{
    clearTm();

    struct tm ltm;
    ltm.tm_year = year - 1900;
    ltm.tm_mon = month - 1;
    ltm.tm_mday = day;
    ltm.tm_hour = hour;
    ltm.tm_min = minute;
    ltm.tm_sec = second;
    time_t tmV = mktime(&ltm);

    if ( millseconds < 0 || millseconds > 999 ||  tmV == -1 )
    {
        _timeZone = 0;
        _showZone = 0;
        _timeSpan = 0;
        return false;
    }
    _timeSpan = tmV;
    _timeSpan = _timeSpan * 1000 + millseconds;
    _timeZone = _showZone = timezone;
    return true;
}

bool CDateTime::init(
        int64_t millseconds ,
        short timezone /* = 0  */)
{

    clearTm();

    _timeSpan = millseconds;
    _timeZone = _showZone = timezone;
    return true;
}

    CDateTime::CDateTime(const CDateTime& other)
:_tm( NULL )
{
    this->_timeZone = other._timeZone;
    this->_timeSpan = other._timeSpan;
    this->_showZone = other._showZone;
}

    CDateTime::CDateTime( int64_t millseconds , short timezone )
:_tm( NULL )
{
    init( millseconds , timezone );
}

CDateTime::~CDateTime()
{
    delete _tm;
}

int CDateTime::getYear() const
{
    return getLocalTime()->tm_year+1900;
}

int CDateTime::getMonth() const
{
    return getLocalTime()->tm_mon + 1;
}

int CDateTime::getDay() const
{
    return getLocalTime()->tm_mday;
}

int CDateTime::getHour() const
{
    return getLocalTime()->tm_hour;
}

int CDateTime::getMinute() const
{
    return getLocalTime()->tm_min;
}

int CDateTime::getSecond() const
{
    return getLocalTime()->tm_sec;
}

int CDateTime::getDayOfWeek() const
{
    return getLocalTime()->tm_wday;
}

int CDateTime::getMillSecond() const
{
    return (int)(_timeSpan % 1000 );
}

void CDateTime::clearMillSecond()
{
    _timeSpan -= getMillSecond();
}

CDateTime& CDateTime::operator =(const CDateTime& other)
{
    clearTm();
    this->_timeZone = other._timeZone;
    this->_timeSpan = other._timeSpan;
    this->_showZone = other._showZone;
    return *this;
}

CInterval CDateTime::operator -(const CDateTime& other) const
{
    int64_t interval;
    if ( this->_timeZone == other._timeZone )
    {
        interval = this->_timeSpan - other._timeSpan;
    }
    else
    {
        interval = this->_timeSpan + this->getTimeZoneMills()
            - other._timeSpan - other.getTimeZoneMills();
    }
    return CInterval(interval);
}

CDateTime CDateTime::operator -(const CInterval& interval) const
{
    CDateTime datetime(*this);
    datetime -= interval;
    return datetime;
}

CDateTime CDateTime::operator +(const CInterval& interval) const
{
    CDateTime datetime(*this);
    datetime += interval;
    return datetime;
}

const CDateTime& CDateTime::operator -=(const CInterval& interval)
{
    this->_timeSpan -= interval._timeSpan;
    clearTm();
    return *this;
}

const CDateTime& CDateTime::operator +=(const CInterval& interval)
{
    this->_timeSpan += interval._timeSpan;
    clearTm();
    return *this;
}

bool CDateTime::operator==(const CDateTime& other) const
{
    if ( this->_timeZone == other._timeZone )
        return this->_timeSpan == other._timeSpan;
    else
        return ( this->_timeSpan + getTimeZoneMills() ) == other._timeSpan + other.getTimeZoneMills();
}

bool CDateTime::operator !=(const CDateTime& other) const
{
    return !(*this == other);
}


bool CDateTime::operator < (const CDateTime& other) const
{
    if ( this->_timeZone == other._timeZone )
        return this->_timeSpan < other._timeSpan;
    else
        return ( this->_timeSpan + getTimeZoneMills() ) < other._timeSpan + other.getTimeZoneMills();
}

bool CDateTime::operator <= (const CDateTime& other) const
{
    if( other < *this )
    {
        return false;
    }
    return true;
}

bool CDateTime::operator >(const CDateTime& other) const
{
    return other < *this;
}

bool CDateTime::operator >=(const CDateTime& other) const
{
    if( *this < other )
    {
        return false;
    }
    return true;
}

long CDateTime::getTimeZoneMills() const
{
    return _timeZone * 1000 * 3600;
}

CDateTime CDateTime::getAbsDt()
{
    return CDateTime();
}

void CDateTime::updateThreadDt()
{
    CAutoLock l(getLock());
    _lastDt.update();
}

CDateTime CDateTime::getThreadDt()
{
    CAutoLock l( getLock() );
    if( _lastDt.getTotalMill() == 0 )
    {
        CDateTime dateTime(0);
        dateTime.update();
        return dateTime;
    }
    return _lastDt;
}

void CDateTime::setLocalTimeZone( int localTimeZone )
{
    _localTimeZone = localTimeZone;
}

CLock* CDateTime::getLock()
{
    static CLock lock;
    return &lock;
}

void CDateTime::clearTm()
{
    //CAutoLock l( getLock() );
    delete _tm;
    _tm = NULL;
}

const struct tm* CDateTime::getLocalTime() const
{
    //CAutoLock l( getLock() );

    /*
       if( _tm )
       {
       return _tm;
       }
     *const_cast<cdf_tm**>(&_tm) = new cdf_tm();
     time_t tmV = getTotalSecond();
     *_tm = *localtime( &tmV );
     return _tm;
     */

    if( _tm )
    {
        return _tm;
    }
    *const_cast<tm**>(&_tm) = new tm();
    time_t tmV = getTotalSecond();
#ifdef _LINUX
    localtime_r( &tmV , _tm );
#else
    *_tm = *localtime( &tmV );
#endif
    return _tm;
}

void CDateTime::setShowTimeZone(short timeZone)
{
    this->_showZone = timeZone;
}

bool CDateTime::parse(const char* value, const char* format)
{
    int len = (int)strlen(format);
    int vallen = (int)strlen(value);
    int index = 0;
    struct tm ltm;
    unsigned long millis = 0;
    for ( index = 0 ; index < len ; )
    {
        char ch = format[index];
        switch ( ch )
        {
            case 'Y' :
            case 'M' :
            case 'D' :
            case 'h' :
            case 'm' :
            case 's' :
            case 'n' :
                {
                    int count = 1;
                    while ( format[++index] == ch )
                        count++;

                    if ( !parseYMD( &ltm, &millis, ch, value, index-count, vallen, count) )
                    {
                        return false;
                    }
                };
                break;
            default :
                index ++;
        }
    }
    time_t timeV = mktime(&ltm);
    if ( timeV == -1 )
    {
        return false;
    }
    int64_t mill = timeV;
    mill = mill * 1000 + millis;
    init( mill );

    return true;
}

std::string CDateTime::asString(const char* format) const
{
    char buf[100];
    sprintf(buf,"%s",format);
    int len = (int)strlen(format);
    int index = 0;
    time_t showtime;
    if ( this->_timeZone != _showZone )
    {
        showtime = getTotalSecond() + ( _showZone - _timeZone) * 3600;
    }
    else
    {
        showtime = getTotalSecond();
    }
    struct tm time = *localtime(&showtime);
    unsigned long mills = (long)(_timeSpan % 1000);
    for ( index = 0 ; index < len ; )
    {
        char ch = format[index];
        switch ( ch )
        {
            case 'Y' :
            case 'M' :
            case 'D' :
            case 'h' :
            case 'm' :
            case 's' :
            case 'n' :
                {
                    int count = 1;
                    while ( format[++index] == ch )
                        count++;

                    if ( !asYMD(&time,mills,ch,buf,index-count,count,sizeof(buf)) )
                        return std::string();
                        //throw CDateTimeException("CDateTime Format Error!");
                };
                break;
            default :
                index ++;
        }
    }
    return std::string(buf);
}

bool CDateTime::asYMD(struct tm* time, unsigned long mills, const char ch,
        char* buf, int index, int count, int bufSize) const
{
    if ( index + count >= bufSize )
        return false;
    char change;
    char format[5];
    sprintf(format,"%%0%dd",count);
    change = buf[index+count];
    switch ( ch )
    {
        case 'Y' :
            if ( count == 2 )
                sprintf( buf + index, format, time->tm_year);
            else if ( count == 4 )
                sprintf( buf + index, format, time->tm_year+1900 );
            else
                return false;
            break;
        case 'M' :
            if ( count > 2 )
                return false;
            sprintf( buf+index, format, time->tm_mon+1 );
            break;
        case 'D' :
            if ( count > 2 )
                return false;
            sprintf( buf + index, format, time->tm_mday );
            break;
        case 'h' :
            if ( count > 2 )
                return false;
            sprintf( buf + index, format, time->tm_hour );
            break;
        case 'm' :
            if ( count > 2 )
                return false;
            sprintf( buf + index, format, time->tm_min );
            break;
        case 's' :
            if ( count > 2 )
                return false;
            sprintf( buf + index, format, time->tm_sec );
            break;
        case 'n' :
            if ( count > 3 )
                return false;
            sprintf( buf + index, format, mills );
            break;
        default :
            return false;
    }
    buf[index + count] = change;
    return true;
}

int CDateTime::getTotalDay() const
{
    return int(( _timeSpan - _localTimeZone * 60 * 60 * 1000 ) / (24 * 60 * 60 * 1000));
}

int64_t CDateTime::getTotalMill() const
{
    return _timeSpan;
}


long CDateTime::getTotalSecond() const
{
    return (long)(_timeSpan / 1000);
}

